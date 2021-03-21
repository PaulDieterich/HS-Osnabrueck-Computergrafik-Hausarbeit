#include "ParticleEmitter.h"
ParticleEmitter::ParticleEmitter(Vector minLoc, Vector maxLoc, Vector minSize, Vector maxSize, int maxLife, int numParticles, float velocity, bool yolo) :
	numParticles(numParticles),
	minLoc(minLoc),
	maxLoc(maxLoc),
	minSize(minSize),
	maxSize(maxSize),
	maxLife(maxLife),
	velocity(velocity),
	yolo(yolo),
	dead(false)
{
	// Erstelle Partikel und lege sie initial im VertexBuffer an.
	// Dadurch, dass wir die Buffer in den Partikeln lagern haben wir sogar eine For schleife gespart!
	particles = new Particle[numParticles];
	for (int i = 0; i < numParticles; ++i)
	{
		// Partikel initialisieren: Jeder Partikel startet an derselben Stelle
		particles[i].position = minLoc;
		particles[i].lifetime = getRandomFloatInRange(0.1, maxLife);
		particles[i].size = minSize;

		// Vertices hinzufügen
		particles[i].VB.begin();
		addBuffer(particles[i]);
		particles[i].VB.end();


	}
	// Indizes hinzufügen
	IB.begin();
	IB.addIndex(0);
	IB.addIndex(1);
	IB.addIndex(2);
	IB.addIndex(2);
	IB.addIndex(3);
	IB.addIndex(0);
	IB.end();

	// Boundary calc und Transformation
	BoundingBox.Min = minLoc-(minLoc/2);
	BoundingBox.Max = maxLoc+(maxLoc/2);

	// Timer starten, wenn yolo, danach löschen
	if (yolo) {
		thread t1(&ParticleEmitter::startTimer, this);
		t1.detach();
	}
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::startTimer() {
	this_thread::sleep_for(chrono::milliseconds((int)maxLife * 1000));
	dead = true;

}

void ParticleEmitter::updateParticles(const float dt)
{
	// Partikel durchgehen und auf Life überprüfen
	for (int i = 0; i < numParticles; ++i)
	{
		Particle& p = particles[i];

		// Deltatime von der Lebenszeit abziehen
		p.lifetime -= dt * velocity;

		// Partikel nur rendern, wenn sie noch am Leben sind
		// Tote Partikel werden gerespawnt
		if (p.lifetime <= 0.0f)
		{
			// Reset
			p.position = minLoc;
			p.lifetime = getRandomFloatInRange(0.1, maxLife);
			p.size = minSize;
			//_sleep(getRandomFloatInRange(1000, 2000));
		}
		else {

			// Partikel über die Zeit verkleinern (entsprechend der lifetime, sonst stirbt der Partikel noch bevor er sein Ziel erreicht)
			p.position = (maxLoc).lerp(minLoc, (p.lifetime / maxLife));
			p.size = maxSize.lerp(minSize, (p.lifetime / maxLife));

			// Neue Position dieses Partikels (später über velocity * dtime zu targetPos)
			// Größe wird interpoliert
			updateBuffer(p);

			//_sleep(getRandomIntInRange(1000, 2000));
		}

		// Die neuen Werte dieses Partikels für den Sprite an den Shader übergeben
		//updateShaderParameters(p);


	}

}

void ParticleEmitter::updateShaderParameters(const Particle& p)
{
	if (typeid(pShader) != typeid(ParticleShader*)) {
		log(ERROR, "Partikelshader nicht gefunden. Parameter können nicht gesetzt werden.");
		return;
	}
	ParticleShader* ps = dynamic_cast<ParticleShader*>(pShader);
	ps->setLife(p.lifetime);
	ps->setPosition(p.position);
}

void ParticleEmitter::draw(const BaseCamera& Cam, float dtime)
{
	// Partikel auf Leben prüfen und Positionen, Farbe etc. updaten!
	//updateParticles(dtime);
	updateParticles(dtime);

	// Hier wird auch der Shader aktiviert!
	BaseModel::draw(Cam);

	// Wie gewohnt das Quad als 2 Dreiecke zeichnen
	for (int i = 0; i < numParticles; ++i)
	{
		particles[i].VB.activate();
		IB.activate();
		glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), NULL);
		IB.deactivate();
		particles[i].VB.deactivate();
	}
}

void ParticleEmitter::addBuffer(Particle& particle)
{
	// Quad als Billboard zeichnen
	float x = particle.position.X;
	float y = particle.position.Y;
	Vector v0 = Vector(x + particle.size.X, y - particle.size.Y, 1); // Unten rechts
	Vector v1 = Vector(x + particle.size.X, y + particle.size.Y, 1); // Oben rechts
	Vector v2 = Vector(x - particle.size.X, y + particle.size.Y, 1); // Oben links
	Vector v3 = Vector(x - particle.size.X, y - particle.size.Y, 1); // Unten links


	// Normalen sind eh egal weil es immer zur Cam zeigt
	particle.VB.addTexcoord0(0, 1);
	particle.VB.addVertex(v0);
	particle.VB.addTexcoord0(0, 0);
	particle.VB.addVertex(v1);
	particle.VB.addTexcoord0(1, 0);
	particle.VB.addVertex(v2);
	particle.VB.addTexcoord0(1, 1);
	particle.VB.addVertex(v3);

}

void ParticleEmitter::updateBuffer(Particle& particle) {

	// Quad als Billboard zeichnen
	float x = particle.position.X;
	float y = particle.position.Y;
	Vector v0 = Vector(x + particle.size.X, y - particle.size.Y, 1); // Unten rechts
	Vector v1 = Vector(x + particle.size.X, y + particle.size.Y, 1); // Oben rechts
	Vector v2 = Vector(x - particle.size.X, y + particle.size.Y, 1); // Oben links
	Vector v3 = Vector(x - particle.size.X, y - particle.size.Y, 1); // Unten links

	particle.VB.updateVertex(0, v0);
	particle.VB.updateVertex(1, v1);
	particle.VB.updateVertex(2, v2);
	particle.VB.updateVertex(3, v3);


	// Buffer aktualisieren!
	particle.VB.end();

}

LineParticleEmitter::LineParticleEmitter(Vector minLoc, Vector maxLoc, Color color, int maxLife, float velocity, bool yolo) :
	ParticleEmitter(minLoc, maxLoc, Vector(), Vector(), maxLife, 0, velocity, yolo)
{
	l = new LineModel(minLoc, maxLoc);
	l->shader(new ConstantShader(color), true);


}


void LineParticleEmitter::draw(const BaseCamera& cam, float dtime)
{
	// Linie bewegen und dann zeichnen
	l->draw(cam);

}
