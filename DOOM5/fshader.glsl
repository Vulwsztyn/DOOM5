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

vec2 parallaxTexCoords(vec4 v, vec2 t, float h, float s){
vec2 ti =-v.xy/s;
float hi=-v.z/s;
vec2 tc=t;
float hc=h;
float ht=h*texture(heightMap,tc).r;
if(v.z<=0)discard;
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
	//vec2 newCoords=parallaxTexCoords(v,iTexCoord0,0.1,100);
	vec4 viewDir=normalize(iviewDir);
    vec4 normalDir=normalize(vec4(texture(normalMap,iTexCoord0).rgb*2-1,0));
    vec4 lightDir=normalize(ilightDir);
    vec4 refclectDir=reflect(-lightDir,normalDir);

    vec4 ka=vec4(0,0,0,0); //Kolor obiektu w swietle otoczenia
    vec4 kd=texture(diffuseMap,iTexCoord0); //Kolor obiektu w swietle rozproszonym
    vec4 ks=vec4(1,1,1,0); //Kolor obiektu w swietle odbitym

    vec4 la=vec4(0,0,0,0); //Kolor swiatla otoczenia
    vec4 ld=vec4(1,1,1,1); //Kolor swiatla rozpraszanego
    vec4 ls=vec4(1,1,1,0); //Kolor swiatla odbijanego

    float nl=max(dot(normalDir,lightDir),0.2); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float rv=pow(max(dot(refclectDir,viewDir),0),32); //Kosinus kata pomiedzy wektorami do obserwatora i odbitym, podniesiony do wykladnika Phonga

	pixelColor=ka*la+kd*ld*vec4(nl,nl,nl,1)+ks*ls*vec4(rv,rv,rv,0);
}
