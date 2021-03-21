#include "BlurShader.h"
#include "BaseShader.h"

BlurShader::BlurShader() {
	//ShaderProgram = createShaderProgram(VertexShaderCode, FragmentShaderCode);
	bool loaded = load(SHADER_DIRECTORY"vsblur.glsl", SHADER_DIRECTORY"fsblur.glsl");
	if (!loaded)
		throw std::exception();
}

void BlurShader::assignLocations() {

}
void BlurShader::activate(const BaseCamera& Cam) const {
    BaseShader::activate(Cam);


    bool horizontal = true;
    bool first_iteration = true;
    int amount = 10;
    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffer[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
    }
}