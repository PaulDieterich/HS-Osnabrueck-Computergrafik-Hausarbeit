#version 400
layout(location=0) in vec4 VertexPos;
layout(location=2) in vec2 VertexTexcoord;
layout ( location = 4 ) in vec4 position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 ModelViewProjMat;
uniform vec2 panning;
uniform vec2 offset;
out vec2 texCoords;

// Von Stackoverflow: https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
  // Als Billboard immer in Richtung der Kamera viewmatrix ausrichten
  vec4 position_viewspace = viewMatrix * vec4( position.xyz, 1);
  
  // Position des Sprites mit Skalierung berechnen. Offset verschiebt die Sprites.
  position_viewspace.xy += 0.5 * (VertexPos.xy - offset);
  //gl_Position = projectionMatrix * position_viewspace + 0.5* vec4(position.xyz, 1);
  gl_Position = ModelViewProjMat * position_viewspace;

  // Das hier sind die Werte, die durch out an den Fragment shader gehen!
  // Hab ich auch erst jetzt verstanden ¯\_(ツ)_/¯
  // Texturkoordinaten berechnen
  vec2 baseCoords = (VertexPos.xy * vec2(0.2, 0.2)) + vec2(0.5, 0.5);
  //vec2 moved = vec2(baseCoords.x+sin(lerp.x*0.001), baseCoords.y+sin(lerp.y*0.001));
  texCoords = baseCoords;
  
}