#include "BloomShader.h"
#include "BaseShader.h"

BloomShader::BloomShader() {
	//ShaderProgram = createShaderProgram(VertexShaderCode, FragmentShaderCode);
	bool loaded = load(SHADER_DIRECTORY"vsbloom.glsl", SHADER_DIRECTORY"fsbloom.glsl");
	if (!loaded)
		throw std::exception();
}

void BloomShader::assignLocations() {
    m_uSceneLocation = glGetUniformLocation(ShaderProgram, "uScene");
    m_uBloomBlurLocation = glGetUniformLocation(ShaderProgram, "uBloomBlur");
    m_uUseBloomLocation = glGetUniformLocation(ShaderProgram, "uUseBloom");
    m_uBloomIntensityLocation = glGetUniformLocation(ShaderProgram, "uBloomIntensity");
    m_uBloomTintLocation = glGetUniformLocation(ShaderProgram, "uBloomTint");
    m_uExposureLocation = glGetUniformLocation(ShaderProgram, "uExposure");
    m_uShowBloomOnlyLocation = glGetUniformLocation(ShaderProgram, "uShowBloomOnly");
}
void BloomShader::activate(const BaseCamera& Cam) const {
	BaseShader::activate(Cam);
}