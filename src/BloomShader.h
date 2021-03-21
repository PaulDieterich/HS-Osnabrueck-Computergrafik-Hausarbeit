//
//  BloomShader.h
//
//  @author Paul Dieterich
//

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


class BloomShader : public BaseShader
{
public:
	BloomShader();

	virtual void activate(const BaseCamera& Cam) const;
protected:
	void assignLocations();

private: 
	int SCR_WIDTH; //with of the txture img
	int SCR_HEIGHT;//height of the txture img

	GLint TextureCoordsLoc;

	Vector TextureCoords;
	Color FragColor;

	GLint m_uSceneLocation;
	GLint m_uBloomBlurLocation;
	GLint m_uUseBloomLocation;
	GLint m_uBloomIntensityLocation;
	GLint m_uBloomTintLocation;
	GLint m_uExposureLocation;
	GLint m_uShowBloomOnlyLocation;
};