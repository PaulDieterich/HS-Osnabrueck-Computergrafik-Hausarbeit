#include "ParticleShader.h"
#include "Utility.h"

using namespace Logger;
ParticleShader::ParticleShader(const Texture* sprite, Color color, Vector panning, Vector offset) : texture(sprite), particleColor(color), panning(panning), offset(offset)
{
	bool loaded = load(SHADER_DIRECTORY "vsparticle.glsl", SHADER_DIRECTORY "fsparticle.glsl");
	if (!loaded)
		throw std::exception();

	// Assign locations, Werte werden sp�ter gesetzt -> Diese hier sind die, die im Shader mit uniform gekennzeichnet werden!
	// Die in und out sind nur die, die �ber die shader geschleift werden

	// Vertexshader
	viewMatrixLoc = glGetUniformLocation(ShaderProgram, "viewMatrix");
	projectionMatrixLoc = glGetUniformLocation(ShaderProgram, "projectionMatrix");
	modelViewProjMatrixLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
	particleColorLoc = glGetUniformLocation(ShaderProgram, "particleColor");
	panningLoc = glGetUniformLocation(ShaderProgram, "panning");
	offsetLoc = glGetUniformLocation(ShaderProgram, "offset");
	lifeLoc = glGetUniformLocation(ShaderProgram, "lifetime");
	positionLoc = glGetUniformLocation(ShaderProgram, "position");

	// Fragmentshader
	spriteLoc = glGetUniformLocation(ShaderProgram, "sprite");
}

ParticleShader::~ParticleShader()
{
}


void ParticleShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	// Sprite Textur für Fragment shader setzen
	int TexSlotIdx = 0;
	texture->activate(TexSlotIdx++);
	glUniform1i(spriteLoc, TexSlotIdx - 1);

	// Projektionsmatrizen und andere Werte für Vertexshader setzen
	Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, Cam.getProjectionMatrix().m);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, Cam.getViewMatrix().m);
	glUniformMatrix4fv(modelViewProjMatrixLoc, 1, GL_FALSE, ModelViewProj);

	// Updaten der Werte, die geändert werden können
	glUniform4f(particleColorLoc, particleColor.R, particleColor.G, particleColor.B, particleColor.a);
	glUniform2f(panningLoc, panning.X, panning.Y);
	glUniform2f(offsetLoc, offset.X, offset.Y);
	glUniform1f(lifeLoc, life);
	glUniform3f(positionLoc, position.X, position.Y, position.Z);
}

void ParticleShader::setPanning(const Vector& panning)
{
	this->panning = panning;
}

void ParticleShader::setOffset(const Vector& offset)
{
	this->offset = offset;
}


void ParticleShader::setColor(const Color& color)
{
	this->particleColor = color;
}

void ParticleShader::setLife(const float life)
{
	this->life = life;
}

void ParticleShader::setPosition(const Vector& position)
{
	this->position = position;
}

