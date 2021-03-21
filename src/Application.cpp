#include "Application.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Application::Application(GLFWwindow* pWin, UI& ui) :
	pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(UNIVERSE_SIZE, UNIVERSE_SIZE)
{

#if DEBUG == true
	string universeSize = to_string(UNIVERSE_SIZE);
	log(INFO, "Erstelle Welt mit Dimension " + universeSize + " x " + universeSize + " x " + universeSize);
	createGrid();
	createAxis();
#endif
	createEntities(ui);
	createEnvironment();
}
void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime, float totalTime, UI &ui, bool& shouldRestart)
{
	Cam.update();

	////////////////////////////
	//     Schiffbewegung     //
	////////////////////////////
	Matrix movement;
	Matrix shipTransform;

	// Wenn kein Treibstoff mehr -> kein Movement durch Spieler
	// Mauscoordinaten und tagetVector() dort wo sich die maus aktuell befindet
	player->cursorMovement(pWindow, mouseX, mouseY);

	// Boost berücksichtigen
	bool isBoosting = player->isPressingButton(pWindow, SHIP_BOOST);
	bool isDriving = true;
	float speed = ship->getSpeed();
	if (isBoosting) speed *= 2;
	movement = player->playerMoveMatrix(pWindow, dtime, speed, isDriving);

	// Wenn keine Spielerbewegung, dann ist Matrix = Identitätsmatrix!
	if (isDriving) {
		ship->drive(isBoosting);
		soundPlayer.playSound2D(SHIP_THRUST, true, true);
	}
	else if (ship->isEmpty() || !isDriving) soundPlayer.stopSound(SHIP_THRUST);
	shipTransform = ship->transform() * movement;
	
	////////////////////////////
	// Collision und Linecast //
	////////////////////////////
	if (stationHitPtr) {
		ship->receiveDamage(20);
		stationHitPtr = nullptr;
	}
	// Wenn Kollision mit Asteroid wir der HitPtr auf den jeweiligen Asteroiden gesetzt
	// Ein bereits zerstörter Asteroid hat nur dann Kollision, wenn die unterliegende Ressource noch nicht eingesammelt wurde
	if (shipHitAsteroid()) {

		// Wenn Asteroid zerstört ist und wir durchfliegen, nehmen wir die Ressource auf
		if (hitPtr->isDestroyed()) {

			// Wenn noch nicht collected
			if (!hitPtr->isCollected()) {

				// Aufnehmen und aus Liste löschen!
				if (ship->collectResource(hitPtr->getResource(), hitPtr->getSize())) {
#if DEBUG == true
					log(INFO, "Resource eingesammelt: ", hitPtr->getResource()->getName(), " Menge: ", hitPtr->getSize());
#endif
					hitPtr->setCollected(true);
				}
				else {
					ui.displayNotification(AMMO_LOW);
				}
			}
		}
		else  {
			// Bei Collision wenn weiter versucht wird zu fliegen -> Das Schiff leicht interpoliert abdriften lassen und Schaden zufügen
			Matrix force;
			force.identity();
			force.translation(Vector(5, 5, 5) * dtime);
			shipTransform = ship->transform() * movement *  force; 
			
			// Schaden je nach größe des Asteroids an das Schiff und den Asteroiden selbst
			ship->receiveDamage(hitPtr->getSize() / 2);
			hitPtr->receiveDamage(0.05);
		}
	}
	else {
		// Ansonsten einfach weiter fliegen bzw. idlen, wenn kein Treibstoff mehr
		// Hierbei wichtig: Im idle etwas schweben lassen, damit es nicht so statisch ist. Sinuskurven liegen zwischen -1 und 1, wenn wir das mit
		// random floats kombinieren können wir ziemlich gute Werte bekommen, damit es nicht zu berechnet aussieht.
		Matrix idleMovement, idleRotation;
		float roll = calcuateFrequency(totalTime, 0.0005, 0.5); // Diese Werte besser statisch lassen, da es sonst ruckelig werden würde. Dient nur zur Interpolation.
		float pitch = calcuateFrequency(totalTime, 0.0005, 0.5);
		float yaw = calcuateFrequency(totalTime, 0.0005, 0.5);
		float move = calcuateFrequency(totalTime, 0.01, 0.5);

		// Diese zufälligen Werte müssen zwischen 0.1 und n liegen, da bei 0 sonst kein Pitch/Yaw/Roll passieren würde -> Könnten wir natürlich machen!
		idleRotation.rotationYawPitchRoll(yaw * getRandomFloatInRange(0.1, 1), pitch * getRandomFloatInRange(0.1, 1), roll * getRandomFloatInRange(0.1, 1));
		idleMovement.translation(ship->transform().forward() * (move + dtime * getRandomFloatInRange(0.1, 1)));
		shipTransform = ship->transform() * idleMovement * idleRotation * movement;
	}

	// Immer das Ship transform drauf packen! Wenn der Treibstoff alle ist, wird die movement Matrix sowieso die Identitätsmatrix sein.
	ship->transform(shipTransform);

	////////////////////////////
	//     Camera setzen      //
	////////////////////////////
	Matrix view, z, r;

	z.identity();
	r.identity();
	z.translation(0, 2, -18);
	r.rotationX(-PI / 6);
	Matrix rs = ship->transform() * z * r;

	view.lookAt(rs.translation(), rs.up(), rs.translation() - rs.forward() + rs.up() * 15.0f);
	ui.setUICord(rs.translation());

	Cam.setViewMatrix(view);
	Matrix k;
	k.identity();
	k.rotationY(PI / 3);
	k.rotationX(-3.14 / 5);
	k *= Cam.getViewMatrix();
	Cam.setViewMatrix(k);

	////////////////////////////
	//     Spieler input      //
	////////////////////////////
	// Radio
	if (player->isPressingButton(pWindow, RADIO_NEXT)) soundPlayer.next();
	else if (player->isPressingButton(pWindow, RADIO_PREVIOUS)) soundPlayer.previous();
	else if (player->isPressingButton(pWindow, RADIO_SWITCH)) soundPlayer.toggleRadio();
	// Shoot
	else if (player->isClickingMouseButton(pWindow, SHOOT_PRIMARY)) {
		if (ship->isAmmoEmpty()) soundPlayer.playSound2D(SHIP_ERROR_SOFT, false, true);
		else {
			if (!ship->isShooting()) {
				// Partikel schießen  und Sound abspielen
				laserParticle = new LineParticleEmitter(ship->transform().translation(), ship->transform().translation() + ship->transform().forward() * 100, Color::green(), (60 / ship->getWeaponRPM())+1, 1.0f, true);
				particles.push_back(laserParticle);
				soundPlayer.playSound2D(SHIP_LASER, false, false);
				ship->shoot();

				// Check ray trace
				if (shipShotAsteroid()) shotPtr->receiveDamage(ship->getWeaponDamage());
			}
		}
	}
	// Game related
	else if (player->isPressingButton(pWindow, END_GAME)) exit(0);
	else if (player->isPressingButton(pWindow, RESTART_GAME)) shouldRestart = true;

	////////////////////////////
	//       Ship on Shop     //
	///////////////////////////
	//schaut ob des Schiff in der Nähe eines Shops/einer Station ist, 
	// wenn ja wird geschaut ob genug ressourcen und genzug geld vorhanden ist. 
	// wenn beides zutrifft wird das schiff upgregared
	if (shipOnshop()) tryUpgradeShip();

	////////////////////////////
	// UI updaten & zeichnen  //
	////////////////////////////
	ui.updateUI(ship->getShield(), ship->getHealth(), ship->getAmmo(), ship->getMaxAmmo(), player->getMoney(), ship->getFuel(), ship->getCargo(), asteroids);
	ui.PrintUI(); 
	if (ship->getAmmo() - 100 <= 0) ui.displayNotification(AMMO_LOW);
	if (ship->getShield() - 10 <= 0) ui.displayNotification(SHIELD_LOW);
	if (ship->getHealth() - 10 <= 0) ui.displayNotification(HEALTH_LOW);

	////////////////////////////
	// 		Game over	  	  //
	////////////////////////////
	if (ship->isDestroyed()) {
		ui.displayGameOver();
		shouldRestart = true;
	}
}

void Application::draw(float dtime)
{
	ShadowGenerator.generate(Models);

	// 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
    // 2. setup shaders and draw models

	drawBaseModels();
	drawAsteroids();
	drawParticles(dtime);
	drawStations();

	ShaderLightMapper::instance().deactivate();
	
    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}


void Application::drawStations() {
	for (list<Station*>::iterator it = stations.begin(); it != stations.end(); ++it) 
	{
		(*it)->draw(Cam);
	}
}

void Application::drawBaseModels() {
	for (list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it) 
	{
		(*it)->draw(Cam);
	}
}

void Application::drawAsteroids() {
	for (list<Asteroid*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it) 
	{
		if ((*it)->isDestroyed() && (*it)->isCollected()) {
			delete* it;
			asteroids.erase(it);
			continue;
		}
		(*it)->draw(Cam);
	}
}

void Application::drawParticles(float dtime) {
	for (list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		if ((*it)->getYolo() && (*it)->getDead()) {
			particles.erase(it);
			continue;
		}
		else {
			(*it)->draw(Cam, dtime);
		}
	}
}

void Application::end()
{
	// Base models löschen
	for (list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it) 
	{
		delete* it;
	}
	// Asteroiden löschen
	for (list<Asteroid*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it)
		delete* it;

	// Partikel löschen
	for (list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); ++it)
		delete* it;
	
	//Stationen löschen
	for (list<Station*>::iterator it = stations.begin(); it != stations.end(); ++it)
		delete* it;

    Models.clear();
	asteroids.clear();
	particles.clear();
	stations.clear();
}

bool Application::shipHitAsteroid() 
{
	for (list<Asteroid*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it)
	{
		if ((*it)->collides(ship, 1)) {
			hitPtr = (*it);
			return true;
		}
	}
	return false;
}

bool Application::shipShotAsteroid()
{
	for (list<Asteroid*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it)
	{
		// laserParticle hält den Line emitter für den Laser
		if (!(*it)->isDestroyed() && (*it)->collides(laserParticle)) {
			shotPtr = (*it);
			return true;
		}
	}
	return false;
}

bool Application::shipOnshop()
{
	for (list<Station*>::iterator it = stations.begin(); it != stations.end(); ++it)
	{
		if ((*it)->collides(ship)) {
			cout << "inBuyZone" << endl;
			stationHitPtr = (*it);
			if (ship->hasCargo()){
				float value = stationModel->sellItems(ship->getCargo());
				player->setMoney(value);
			}
			return true;
		}
		return false;
	}
}

void Application::tryUpgradeShip()
{
	Ship* temp = ship;
	if (Fighter* fighter = dynamic_cast<Fighter*> (temp)) {
		 temp = new Viper();
	}else if (Viper* fighter = dynamic_cast<Viper*> (temp)) {
		temp = new Blaster();	
	}
	if (player->getMoney() > temp->getBasePreis()) {
		delete ship;
		ship = temp;
		ship->shader(new PhongShader(), true);
	}
}

void Application::createEntities(UI& ui)
{
	// Initialer Spieler
	player = new Player(10000.0);
#if DEBUG == true
	log(INFO, "Spieler wurde erstellt.");
#endif

	// Initiales Schiff
	ship = new Fighter();
	ship->shader(new PhongShader(), true);
	Models.push_back(ship);
	ui.setMaxTreibstoff(ship->getFuel());
#if DEBUG == true
	log(INFO, "Standardschiff wurde erstellt.");
#endif
	
}

void Application::createEnvironment() 
{
	// Initiale Skybox
	Matrix skybox;
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	pModel->shader(new PhongShader(), true);
	skybox.scale(UNIVERSE_SIZE);
	pModel->transform(skybox);
	Models.push_back(pModel);

	// Initiale Asteroiden
    createAsteroids();

	// Initiales Licht (aus Richtung der Sonne)
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(2, 2, -2));
	dl->color(Color(0.5, 0.5, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	// Initiale Station
	createStations();
}

void Application::createStations() 
{
	int maxStations = UNIVERSE_SIZE * 0.02;
	Matrix aScale, aRotation, aTranslation, aTransform;
	for (int i = 0; i < maxStations; ++i)
	{
		stationModel = new Station(ASSET_DIRECTORY "models/stations/Drift/drift.dae", false);
		PhongShader* stationShader = new PhongShader();
		stationModel->shader(stationShader, true);
		aScale.scale(0.3);
		aRotation.rotationYawPitchRoll(Vector(getRandomIntInRange(0, 360), getRandomIntInRange(0, 360), getRandomIntInRange(0, 360)));
		aTranslation.translation(getRandomIntInRange(-UNIVERSE_SIZE, UNIVERSE_SIZE), getRandomIntInRange(-UNIVERSE_SIZE, UNIVERSE_SIZE), getRandomIntInRange(-UNIVERSE_SIZE, UNIVERSE_SIZE));
		aTransform = stationModel->transform() * aTranslation * aRotation * aScale;
		stationModel->transform(aTransform);
		stations.push_back(stationModel);

		/*
		// Bounding box zeichnen
		Vector x = stationModel->boundingBox().size();
		pModel = new LineBoxModel(x.X, x.Y, x.Z);
		ConstantShader* cs = new ConstantShader();
		pModel->shader(cs, true);
		pModel->transform(aTransform);
		Models.push_back(pModel);

		*/
	}
#if DEBUG == true
	log(INFO, maxStations, " Stationen gespawnt.");
#endif

}

void Application::createAsteroids() 
{
	int maxAsteroids = getRandomIntInRange(UNIVERSE_SIZE, UNIVERSE_SIZE * 1.5);
	if (maxAsteroids > 3000) maxAsteroids = 3000; // So viele können nicht gerendert werden...

	// Erst die Ressourcen generieren, die ein Asteroid annehmen kann.
	resources = {
		new Resource("Kohlenstoff", 100.0, COMMON, 25.0, true, Color::red()),
		new Resource("Phosphor", 50.0, COMMON, 10.0, false, Color::cyan()),
		new Resource("Eisen", 100.0, COMMON, 30.0, false, Color::gray()),
		new Resource("Plutonium", 75.0, RARE, 20.0, true, Color::green()),
		new Resource("Gold", 250.0, UNCOMMON, 50.0, false, Color::gold()),
		new Resource("Bronze", 40.0, COMMON, 30.0, false, Color::brown()),
		new Resource("Titan", 350.0, RARE, 70.0, false, Color::purple()),
		new Resource("Platin", 400.0, LEGENDARY, 75.0, false, Color::orange())
	};
	Resource* dust = new Resource("Asteroidenstaub", 1.0, COMMON, 0.1, false, Color::white());

	// Asteroiden und Transformationen erstellen
	Matrix aTransform, aScale, aRotation, aTranslation;
	for (int i = 0; i < maxAsteroids; ++i)
	{
		// Matrizen zurücksetzen
		aTransform.identity();
		aScale.identity();
		aRotation.identity();
		aTranslation.identity();

		// Zufällige Ressource bestimmen die der Asteroid hält -> Wenn Chance verpasst stattdessen Asteroidenstaub hinzufügen
		Resource* randomResource = nullptr;
		for (int i = 0; i < resources.size(); ++i)
		{
			if (getRandomIntInRange(1, (100-resources[i]->getRarity())/10) == 1) {
				randomResource = resources[i];
			}
		}
		if (!randomResource) randomResource = dust;

		// Asteroid setzen und transformieren
		string name = ASSET_DIRECTORY "models/asteroids/asteroid" + to_string(getRandomIntInRange(1, ASTEROID_MODEL_NUM)) + ".dae";
		float scale = getRandomFloatInRange(0.55, 5);

		asteroidModel = new Asteroid(name.c_str(), scale, randomResource); // TODO: Damit wird jedes mal ein neues Objekt erstellt...kann man das nicht auch umgehen?
		asteroidModel->shader(new PhongShader(), true);

		// Asteroid transformation
		aScale.scale(scale);
		aRotation.rotationYawPitchRoll(Vector(getRandomIntInRange(0, 360), getRandomIntInRange(0, 360), getRandomIntInRange(0, 360)));
		aTranslation.translation(getRandomIntInRange(-UNIVERSE_SIZE, UNIVERSE_SIZE), getRandomIntInRange(-UNIVERSE_SIZE, UNIVERSE_SIZE), getRandomIntInRange(-UNIVERSE_SIZE, UNIVERSE_SIZE));
		aTransform = asteroidModel->transform() * aTranslation * aRotation * aScale;
		asteroidModel->transform(aTransform);
		asteroids.push_back(asteroidModel);

#if DEBUG == true
		
		// Bounding box zeichnen
		Vector x = asteroidModel->boundingBox().size();
		pModel = new LineBoxModel(x.X, x.Y, x.Z);
		ConstantShader* cs = new ConstantShader();
		cs->color(randomResource->getColor());
		pModel->shader(cs, true);
		pModel->transform(aTransform);
		Models.push_back(pModel);
		
#endif
	}

#if DEBUG == true
	log(INFO, maxAsteroids, " Asteroiden gespawnt.");
#endif

}

void Application::createGrid()
{
	// Nur für debug Zwecke!
	pModel = new LinePlaneModel(UNIVERSE_SIZE, UNIVERSE_SIZE, UNIVERSE_SIZE, UNIVERSE_SIZE);
	pModel->shader(new ConstantShader(), true);
	Models.push_back(pModel);
	log(INFO, "Grid erstellt.");
}

void Application::createAxis() {
	// Brainfuck Moment: Die Farben ensprechen genau den Werten eines Vectors im XYZ = RGB Format.
	// WICHTIG: OpenGL Z Achse ist in der anderen Richtung
	// WICHTIG: Durch die Kamera sehen wir die X Achse gespiegelt (links positiv, rechts negativ)

	// X Achse
	pModel = new LineModel(Vector(0, 0, 0), Vector(1000, 0, 0));
	ConstantShader* csX = new ConstantShader();
	csX->color(Color(1, 0, 0));
	pModel->shader(csX, true);
	Models.push_back(pModel);

	// Y Achse
	pModel = new LineModel(Vector(0, 0, 0), Vector(0, 1000, 0));
	ConstantShader* csY = new ConstantShader();
	csY->color(Color(0, 1, 0));
	pModel->shader(csY, true);
	Models.push_back(pModel);

	// Z Achse
	pModel = new LineModel(Vector(0, 0, 0), Vector(0, 0, 1000));
	ConstantShader* csZ = new ConstantShader();
	csZ->color(Color(0, 0, 1));
	pModel->shader(csZ, true);
	Models.push_back(pModel);

}
