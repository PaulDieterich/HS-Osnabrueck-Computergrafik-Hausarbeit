#version 400
//https://learnopengl.com/Advanced-Lighting/Bloom
//https://www.youtube.com/watch?v=5VxmoVru-Ws
in vec2 vTexCoords; 
uniform sampler2D uImage; 
uniform bool uHorizontal;

out vec4 FragColor; 

//TODO: nicht ganz sicher wieso !
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main(void){

    vec2 texOffset = 1.0f / textureSize(uImage/lod);
    /**gvec4 textureLod(gsampler2D sampler,vec2 P,float lod)
    * gsampler2D sampler: Specifies the sampler to which the texture from which texels will be retrieved is bound.
    * vec2 P: Specifies the texture coordinates at which texture will be sampled. 
    * float lod: Specifies the explicit level-of-detail 
    */
    result += = texture(image, vTexCoords, lod).xyz * wight[0];
    if(uHorizontal){
        //Horizontal blur
        for(int i = 0; i < weight.length(); ++i) {
            result += texture(uImage, vTexCoords + vec2(texOffset * i, 0.0)).xyz * weight[i];
            result += texture(uImage, vTexCoords - vec2(texOffset * i, 0.0)).xyz * weight[i];
        }
    }else{
        //Vertical blur
          for(int i = 0; i < weight.length(); ++i) {
            result += texture(uImage, vTexCoords + vec2( 0.0,texOffset * i)).xyz * weight[i];
            result += texture(uImage, vTexCoords - vec2( 0.0,texOffset * i)).xyz * weight[i];
        }
    }
   
     FragColor = vec4(result,1.0); 
}