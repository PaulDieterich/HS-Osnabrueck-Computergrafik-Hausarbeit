#version 400

in vec2 texCoords;

out vec4 fragColor;

uniform vec4 particleColor;
uniform sampler2D sprite;
uniform float lifetime;

void main() {
  vec4 spriteTexture = texture(sprite, texCoords) * particleColor;
  fragColor = spriteTexture;
  //fragColor = vec4(1, 1, 0, 1);
}