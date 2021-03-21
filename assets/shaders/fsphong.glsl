#version 400

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec4 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;

const int MAX_LIGHTS=14;
struct Light
{
	int Type;
	vec4 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius;
	int ShadowIndex;
};

uniform Lights 
{
	int LightCount;
	Light lights[MAX_LIGHTS];
};

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}

//0 = POINT 1= DIRECTIONAL 2=SPOT
void main()
{
     vec4 DiffTex = texture( DiffuseTexture, Texcoord);
    if(DiffTex.a <0.3f) discard;
    vec3 N = normalize(Normal);
    vec3 L = normalize(LightPos-Position);
    vec3 E = normalize(EyePos-Position);
    vec3 R = reflect(-L,N);


    vec3 Color = vec3(0,0,0);
    for(int i=0; i<LightCount;i++){
	//	Color += lights[i].Color.rgb;
        vec3 Dist = lights[i].Position - Position;
        float DistLen = length(Dist);
              
        //Li.SpotRadius = lights[i].SpotRadius; 
        float Attenuation = 1;
        //vec3 psps = vec3( Li.Attenuation.x  * ((Li.SpotRadius.x * 3.1415926538)/180),  Li.Attenuation.y  * ((Li.SpotRadius.y * 3.1415926538)/180),Li.Attenuation.z);
        //Li.Color = lights[i].Color.rgb * psps;
        if(lights[i].Type == 0){ 
            Attenuation = 1.0/(lights[i].Attenuation.x + (lights[i].Attenuation.y * DistLen) + (lights[i].Attenuation.z * DistLen * DistLen));
            L = normalize(lights[i].Position-Position);
        }
        else if(lights[i].Type == 1){

            L = normalize( - lights[i].Direction);
        }
	    
        else if(lights[i].Type == 2)   {
          Attenuation = 1.0/(lights[i].Attenuation.x + (lights[i].Attenuation.y * DistLen) + (lights[i].Attenuation.z * DistLen * DistLen));
            L = normalize(lights[i].Position-Position);
            vec3 m = - lights[i].Direction;
            float omega = acos(dot(normalize(m) , L));
            float s = ((omega -lights[i].SpotRadius.x) / (lights[i].SpotRadius.y-lights[i].SpotRadius.x));
            float li = (Attenuation/ length(L)*length(L))*(1-sat(s));
            Attenuation = li;
        } 
        vec3 LColor = lights[i].Color.rgb * Attenuation;
        vec3 H = normalize(E+L); // Winkelhalbierende
        vec3 DiffuseComponent = LColor *DiffuseColor * DiffTex.rgb * sat(dot(N,L) ) ;
        vec3 SpecularComponent = LColor *SpecularColor * pow( sat(dot(N, H)), SpecularExp ) ;
        Color += DiffuseComponent + SpecularComponent;
    }


    // vec3 NormalXYZ(Normal.R*2-1, Normal.G*2-1, Normal.B*2-1);
    //mat3 mat(Tangent, -Bitangent, normalize(NormalXYZ);
    // TODO: Matrix auf Normale aus Normal Map anwenden? Was?
    
   

    // Fehler in der Vorlesung: E + L normiert (durch Betrag teilen, nicht durch 2)
    
  
    FragColor = vec4(Color + AmbientColor*DiffTex.rgb,DiffTex.a);
}
