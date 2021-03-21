//
//  ParticleShader.h
//  Stellt einen Partikelshader im Format des von Prof. Lensing erstellten PhongShaders zur 
//  Erzeugung von Partikeln dar. Dient somit also auch als Emitter/Generator!
//
//  NOTE: Ist leider noch nicht fertig...
//
//  @author Justin Falkenstein
//
#pragma once
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "BaseShader.h"
#include "Texture.h"
#include "color.h"
#include "Utility.h"

using namespace std;
using namespace Math;
using namespace Logger;

class ParticleShader : public BaseShader
{
private:

	// Vertex shader params
	GLuint viewMatrixLoc;
	GLuint projectionMatrixLoc;
	GLuint modelViewProjMatrixLoc;
	GLuint particleColorLoc;
	GLuint panningLoc;
	GLuint offsetLoc;
	GLuint lifeLoc;
	GLuint positionLoc;

	// Fragment shader params
	GLuint spriteLoc;

	// Shader Eigenschaften
	const Texture* texture; // Die Textur/Sprite f�r den Shader
	Color particleColor;	// Die Farbe, die mit der Textur multipliziert wird. Standardm��ig wei�, sodass die Textur 100% enthalten ist
	Vector panning;			// 2D Vector fürs Panning -> FÜr Bewegung in der Textur
	Vector offset;			// 2D Vector fürs Offset

	Vector position;		// Durch den Emitter weitergegebene position value
	float life;				// Durch den Emitter weitergegebene Life value

public:
	ParticleShader(const Texture* sprite, Color color = Color(1, 1, 1, 1), Vector panning = Vector(0, 0, 0), Vector offset = Vector(0.5, 0.5, 0));
	~ParticleShader();

	/**
	 * Wird jeden Frame aufgerufen. Hier werden alte Partikel gel�scht und neue hinzugef�gt
	 * Neu gesetzte Parameter werden automatisch im Shader geupdated -> evtl. f�r mehr Effizienz so wie im PhongShader pr�fen
	 * @param Cam Das Kamerareferenzobjekt, auf das die Partikel projeziert werden sollen
	 */
	virtual void activate(const BaseCamera& Cam) const;

	// 
	// Setter für den Emittor
	//
	void setPanning(const Vector& panning);
	void setOffset(const Vector& offset);
	void setColor(const Color& color);
	void setLife(const float life);
	void setPosition(const Vector& position);
};
