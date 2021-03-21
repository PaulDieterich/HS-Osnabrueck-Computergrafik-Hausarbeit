//
//  Application.h
//  Stellt die Anwendung und die dafür benötigten wichtigsten Funktionen zum Aufbau des Spiels dar
//  Wir haben sie so viel angepasst, dass sie kaum noch der Version aus dem Praktikum entspricht
//
//  @author Justin Falkenstein & Paul Dieterich
//

#pragma once

#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif

#include <stdio.h>
#include <list>
#include "ShaderLightmapper.h"
#include "Ship.h"
#include "Utility.h"
#include "Asteroid.h"
#include "Station.h"
#include "Player.h"
#include "Camera.h"
#include "PhongShader.h"
#include "ParticleEmitter.h"
#include "ConstantShader.h"
#include "GlowShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BaseModel.h"
#include "ShadowMapGenerator.h"
#include "Asteroid.h"
#include "SoundPlayer.h"
#include "ui.h"

#define DEBUG false
#define SKYBOX_MEASURE 200
#define UNIVERSE_SIZE 1024

using namespace Math;
using namespace Logger;
using namespace std;

class Application
{
public:
    Application(GLFWwindow* pWin, UI& ui );
    void start();
    void draw(float dtime);
    void update(float dtime, float totalTime, UI &ui, bool& shouldRestart);
    void end();
protected:

    /**
     * Erstellung der sichtbaren und nicht sichtbaren, direkt steuerbaren Entitäten
     * Nicht zwangsläufig Meshes, aber zumindest die zu der Spiellogik gehörenden Objekte
     */
    void createEntities(UI& ui);

    /**
     * Universum mit allen initial benötigten Objekten erstellen!
     * Hier werden alle statischen, der Umgebung zugehörigen Objekte erstellt
     */
    void createEnvironment();

    /**
     * Erstellt die Stationen, an denen das Schiff die Ressourcen verkauft und das Schiff aufgewertet werden kann
     * 
     */
    void createStations();

    /**
     * Asteroiden erstellen
     * Jeder Asteroid kann mit einer gewissen Chance eine Ressource halten, die beim Zerstören freigesetzt wird
     */
    void createAsteroids();

    /**
     * Prüft Kollision des Schiffs mit allen Asteroiden und speichert bei Kollision den Zeiger auf den ersten gefundenen
     * Asteroiden in den hitPtr
     */
    bool shipHitAsteroid();

    /**
     * Prüft, ob Laserstrahl mit einem der Asteroiden kollidiert und gibt setzt den Zeiger auf den Asteroiden
     * in der Variable shotPtr
     */
    bool shipShotAsteroid();

    /**
     * Verkauf alle items im Inventar des Spielers
     */
    bool shipOnshop();

    /**
     * change Ship Model
     */
    void tryUpgradeShip();

    /**
     * Funktionen zum Zeichnen von Models
     */
    void drawStations();
    void drawBaseModels();
    void drawAsteroids();
    void drawParticles(float dtime);

    /**
     * Debug Funktion, erzeugt ein Line grid entlang der Universe Größe
     */
    void createGrid();

    /**
     * Debug Funktion, erzeugt Linien entlang der Achsen
     */
    void createAxis();

    Player* player;
    Camera Cam;
    GLFWwindow* pWindow;
    ShadowMapGenerator ShadowGenerator;

    // Temporäre Pointer-Variablen
	BaseModel* pModel;
    Asteroid* asteroidModel;
    Station* stationModel;
    Ship* ship;

    // Sound engine
    SoundPlayer soundPlayer;
    vector<Resource*> resources;

    // Mausinput
    double mouseX, mouseY;

    // Pointers for collision and stuff
    ParticleEmitter* laserParticle;
    Asteroid* hitPtr = nullptr;
    Station* stationHitPtr = nullptr;
    Asteroid* shotPtr = nullptr;

    // Model listen
    list<BaseModel*> Models;
    list<Asteroid*> asteroids;
    list<ParticleEmitter*> particles;
    list<Station*> stations;
};
