
#define Glow_hpp

#include <stdio.h>
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <iostream>
#include <assert.h>
#include "baseshader.h"
//https://learnopengl.com/Advanced-Lighting/Bloom
class GlowShader : public BaseShader {
public:
	GlowShader();
	void assignLocations();
	virtual void activate(const BaseCamera& Cam) const;

	mutable unsigned int UpdateState;
	int SCR_WIDTH, SCR_HEIGHT = 0;

	GLint m_uBlurHorizontalLocation;
	GLint m_uBlurImageLocation;
	GLint m_uBlurMaxLodLocation;
};

