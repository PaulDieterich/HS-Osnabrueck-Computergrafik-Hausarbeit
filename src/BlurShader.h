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
#include "color.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"
#include "baseshader.h"
#include "texture.h"
#include "ShaderLightMapper.h"


class BlurShader : public BaseShader
{
public:
	BlurShader();

	virtual void activate(const BaseCamera& Cam) const;
protected:
	void assignLocations();

private:
	int SCR_WIDTH, SCR_HEIGHT;
	//größe der texture
	GLuint uHorizontalLoc; 


	bool uHorizontal;
	int lod;
	
	


};