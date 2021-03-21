#version 400

layout(location = 0) in sampler2D scene; 
layout(location = 1) in sampler2D bloomBlur; 

out vec2 vTexCoords; 

void main(void){

    vTexCoords = aTexCoords; 
    gl_Position = vec4(aPosition, 1.0f);
}