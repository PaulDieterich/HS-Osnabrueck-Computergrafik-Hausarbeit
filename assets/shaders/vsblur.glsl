// mithilfe von: Florian Torres Youtube 
//https://www.youtube.com/watch?v=5VxmoVru-Ws

#version 400

layout(location = 0) in vec3 aPosition; 
layout(location = 1) in vec2 aTexCoords; 

out vec2 vTexCoords; 

void main(void){

    vTexCoords = aTexCoords; 
    gl_Position = vec4(aPosition, 1.0f);
}