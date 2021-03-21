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
#include "vector.h"
#include "Camera.h"
#include "BaseShader.h"
class ShaderHelper : public BaseShader {


public:
	ShaderHelper() {};
	virtual void activate(const BaseCamera& Cam) const;

	void initBloom();
	void bind();

	// Init Bloom
	//https://github.com/torresf/gltf-viewer-tutorial/blob/master/apps/gltf-viewer/ViewerApplication.cpp

	GLsizei m_nWindowWidth = 780;
	GLsizei m_nWindowHeight = 600;
	unsigned int m_hdrFBO;
	unsigned int m_colorBuffers[2];
	unsigned int m_pingpongFBO[2];
	unsigned int m_pingpongBuffer[2];
	// Bloom parameters
	bool m_useBloom = true;
	bool m_showBloomOnly = false;
	int m_bloomQuality = 2;
	int m_maxLod = 4;
	float m_bloomThreshold = 1.f;
	float m_bloomIntensity = 2.5f;
	Vector m_bloomTint = Vector(1.0f, 1.0f, 1.0f);
	float m_exposure = 1.f;

	// Bloom Blur Uniforms Locations
	GLint m_uBlurHorizontalLocation;
	GLint m_uBlurImageLocation;
	GLint m_uBlurWeightLocation;
	GLint m_uBlurMaxLodLocation;

	// Bloom Final Uniforms Locations
	GLint m_uSceneLocation;
	GLint m_uBloomBlurLocation;
	GLint m_uUseBloomLocation;
	GLint m_uBloomIntensityLocation;
	GLint m_uBloomTintLocation;
	GLint m_uExposureLocation;
	GLint m_uShowBloomOnlyLocation;

};
void ShaderHelper::bind() {
	glCopyImageSubData(
		m_colorBuffers[1], GL_TEXTURE_2D, 0, 0, 0, 0,
		m_pingpongBuffer[0], GL_TEXTURE_2D, 0, 0, 0, 0,
		m_nWindowWidth, m_nWindowHeight, 1
	);

	bool horizontal = true, first_iteration = true;

	glUniform1i(m_uBlurMaxLodLocation, m_maxLod);

	for (unsigned int i = 0; i < 2 * m_bloomQuality; i++)
	{
		glUniform1i(m_uBlurHorizontalLocation, horizontal);

		glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[horizontal]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pingpongBuffer[!horizontal]);  // Bind texture of other framebuffer
		glUniform1i(m_uBlurImageLocation, 0);

		
		glGenerateMipmap(GL_TEXTURE_2D);

		horizontal = !horizontal;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffers[0]);
	glUniform1i(m_uSceneLocation, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pingpongBuffer[0]);
	glUniform1i(m_uBloomBlurLocation, 1);

	glUniform1i(m_uUseBloomLocation, m_useBloom);
	glUniform1f(m_uBloomIntensityLocation, m_bloomIntensity);
	glUniform3f(m_uBloomTintLocation, m_bloomTint[0], m_bloomTint[1], m_bloomTint[2]);
	glUniform1f(m_uExposureLocation, m_exposure);
	glUniform1f(m_uShowBloomOnlyLocation, m_showBloomOnly);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShaderHelper::initBloom() {
	// Set up floating point framebuffer to render scene to
	glGenFramebuffers(1, &m_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	glGenTextures(2, m_colorBuffers);
	for (unsigned int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_nWindowWidth, m_nWindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);
		glGenerateMipmap(GL_TEXTURE_2D);
		// Attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
			GL_TEXTURE_2D, m_colorBuffers[i], 0);
	}

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// PingPong
	glGenFramebuffers(2, m_pingpongFBO);
	glGenTextures(2, m_pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, m_pingpongBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_nWindowWidth, m_nWindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);
		glGenerateMipmap(GL_TEXTURE_2D);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongBuffer[i], 0);
		// Also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
}
