#version 330

uniform sampler2D diffuseMap; //Zmienna reprezentujaca jednostke teksturujaca
uniform sampler2D normalMap;  //Zmienna reprezentujaca jednostke teksturujaca
uniform sampler2D heightMap;  //Zmienna reprezentujaca jednostke teksturujaca

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 lightDist;
in vec4 ilightDir; //wektor do swiatla(przestrzen oka)
in vec4 iviewDir; //wektor do obserwatora(przestrzen oka)
in vec4 inormalDir; //wektor normalny (przestrzen oka)
in vec2 iTexCoord0; //wspolrzedne teksturowania

vec2 parallaxTexCoords(vec4 viewDir, vec2 t, float h, float s){
vec2 ti =-h*viewDir.xy/s;
float hi=-h/s;
vec2 tc=t;
float hc=h;
float ht=h*texture(heightMap,tc).r;
if(viewDir.z<=0)discard;
while(hc>ht)
{
	tc=tc+ti;
	if(tc.x>1 || tc.y>1 || tc.y<0 || tc.x<0) discard;
	hc=hc+hi;
	ht=h*texture(heightMap,tc).r;
}
return tc;
}

void main(void) {
	vec4 viewDir=normalize(iviewDir);
	vec2 newCoords=parallaxTexCoords(viewDir,iTexCoord0,0.1,100);
    vec4 normalDir=normalize(vec4(texture(normalMap,newCoords).rgb*2-1,0));
    vec4 lightDir=normalize(ilightDir);
    vec4 refclectDir=reflect(-lightDir,normalDir);

    vec4 ambientColor=vec4(0,0,0,0); //Kolor obiektu w swietle otoczenia
    vec4 diffuseColor=texture(diffuseMap,newCoords); //Kolor obiektu w swietle rozproszonym
    vec4 specColor=vec4(1,1,1,0); //Kolor obiektu w swietle odbitym

    vec4 lightColor=vec4(1,0.6,0.6,1); //Kolor swiatla rozpraszanego

	float shininess = 64.0;
	float lightPower = 100.0;
	float screenGamma = 1.5;

  float distance = length(lightDist);
  distance = distance * distance;

  float lambertian = max(dot(lightDir,normalDir), 0.0);
  float specular = 0.0;

  if(lambertian > 0.0) {

    vec4 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normalDir), 0.0);
    specular = pow(specAngle, shininess);
       
  }
  vec4 colorLinear = ambientColor +
                     diffuseColor * lambertian * lightColor * lightPower / distance +
                     specColor * specular * lightColor * lightPower / distance;
  // apply gamma correction (assume ambientColor, diffuseColor and specColor
  // have been linearized, i.e. have no gamma correction in them)
  vec4 colorGammaCorrected = pow(colorLinear, vec4(1.0/screenGamma));
  // use the gamma corrected color in the fragment
  pixelColor=colorGammaCorrected;
}
