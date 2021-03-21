//
//  ParticleEmitter.h
//  Stellt ein Partikelmodell dar, welches als Emitter für Partikelsysteme in Verbindung
//  mit dem Partikelshader verwendet werden kann
//
//
//  NOTE: Leider noch nicht fertig. Es gibt noch zu viele Probleme, als das die Partikel ansprechend genug aussähen...besser erstmal ohne
//
//  @author Justin Falkenstein
//
#pragma once
#include <chrono>
#include <thread>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ParticleShader.h"
#include "BaseModel.h"
#include "Camera.h"
#include "Utility.h"

using namespace Math;
struct Particle
{
	VertexBuffer VB;	// Ja, ist nicht sehr speichereffizient.
	Vector position;	// Gibt die momentane Position des Mittelpunkts des Quads dieses Partikels an
	Vector size;		// Gibt die Abstände vom Mittelpunkt zu den XY Seiten an, die die Größe des Partikels ausmachen
	float lifetime;		// Gibt die Lebensspanne des Partikels an
};


class ParticleEmitter : public BaseModel
{
public:
	ParticleEmitter(Vector minLoc, Vector maxLoc, Vector minSize, Vector maxSize, int maxLife, int numParticles = 10, float velocity = 1.0f, bool yolo = false);
	~ParticleEmitter();

	/**
	 * Startet einen Timer in eigenem Thread bei Erzeugung des Partikelsystems.
	 * Nach Beendigung können Aktionen ausgeführt werden, z.B. die Zerstörung, wenn yolo = true
	 */
	void startTimer();

	/**
	 * Updated die Lifetime und Position jeden Partikels und die Vertices
	 * im VertexBuffer
	 */
	void updateParticles(const float dt);

	/**
	 * Updated die von dem hinterlegten Shader benötigten Parameter
	 */
	void updateShaderParameters(const Particle& p);

	/**
	 * Überschriebene Funktion, wird jeden Frame aufgerufen, um neu zu zeichnen
	 */
	virtual void draw(const BaseCamera& cam, float dtime);

	/**
	 * Berechnet die Vertices für den Partikel an dem �bergebenen Mittelpunkt und
	 * fügt sie in den VertexBuffer ein
	 */
	void addBuffer(Particle& particle);

	/**
	 * Berechnet die neuen Vertices für den Partikel an dem �bergebenen Mittelpunkt und
	 * updated sie in dem VertexBuffer
	 */
	void updateBuffer(Particle& particle);

	//
	// Getter
	//
	const bool getYolo() { return yolo; }
	const bool getDead() { return dead; }
	Vector getMinLoc() { return minLoc; }
	Vector getMaxLoc() { return maxLoc; }

	//
	// Setter
	//
	void setMinLoc(Vector loc) { minLoc = loc; }
	void setMaxLoc(Vector loc) { maxLoc = loc; }

protected:
	Vector minLoc;			// Anfangsposition des Partikels (Mittelpunkt)
	Vector maxLoc;			// Endposition des Partikels (Mittelpunkt)
	Vector minSize;			// Mindestabstände/Größe vom Mittelpunkt am Anfang
	Vector maxSize;			// Mindestabstände/Größe vom Mittelpunkt am Ende
	float maxLife;			// Maximale Lebenszeit in Sekunden
	float velocity;			// Gibt an wie schnell die Partikel von minLoc zu maxLoc fliegen und größer werden, wenn minSize != maxSize
	bool yolo;				// Gibt an, ob das Partikelsystem nur einmal emittieren soll
	bool dead;				// Gibt an, ob das Partikelsystem tot ist, wenn yolo gesetzt wurde

private:
	// Partikelbuffer Quad
	IndexBuffer IB;
	Particle* particles;
	int numParticles;


};


class LineParticleEmitter : public ParticleEmitter {
private:
	LineModel* l;
	thread t1;
public:
	LineParticleEmitter(Vector minLoc, Vector maxLoc, Color color, int maxLife, float velocity = 1.0f, bool yolo = false);

	/**
	 * Überschriebene Funktion, wird jeden Frame aufgerufen, um neu zu zeichnen
	 */
	virtual void draw(const BaseCamera& cam, float dtime);
};

