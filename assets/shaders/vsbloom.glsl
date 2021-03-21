#version 400

layout(location = 0) out vec3 FragColor;
layout(location = 1) out vec3 BrightColor;
out vec4 out_Colors; 


void main(void){
    vec4 colors = texture(colorTexture, textureCords);
    //https://en.wikipedia.org/wiki/Grayscale#Luma_coding_in_video_systems
    //(color.r + color.g + color.b) / 3.0f;
    //((color.r * 0.2126) + (color.g * 0.7152)+ (color.b * 0.0622);
    float brightness = dot(FragColor, vec3(0.2126,0.7152,0.0622));
    if(brightness > 0.7){
        BrightColor = FragColor;
    }else{
        BrightColor = vec3(0,0,0);
    }
}