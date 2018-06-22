#version 330

struct Material {
    vec4 ambient;
    sampler2D diffuseMap;
    sampler2D specMap;
	sampler2D normalMap;
	sampler2D heightMap;
    vec4 shininess;
}; 
struct Light {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform Light light;  
  
uniform Material material;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 i_l; //wektor do swiatla(przestrzen oka)
in vec4 i_v; //wektor do obserwatora(przestrzen oka)
in vec4 i_n; //wektor normalny (przestrzen oka)
in vec2 iTexCoord0; //wspolrzedne teksturowania


void main(void) {
	vec4 v=normalize(i_v);
    vec4 n=normalize(i_n);
    vec4 l=normalize(i_l);
    vec4 r=reflect(-l,n);

    vec4 ka=material.ambient; //Kolor obiektu w swietle otoczenia
    vec4 kd=texture(material.diffuseMap,iTexCoord0); //Kolor obiektu w swietle rozproszonym
    vec4 ks=material.shininess*texture(material.specMap,iTexCoord0); //Kolor obiektu w swietle odbitym

    vec4 la=light.ambient; //Kolor swiatla otoczenia
    vec4 ld=light.diffuse; //Kolor swiatla rozpraszanego
    vec4 ls=light.specular; //Kolor swiatla odbijanego

    float nl=max(dot(n,l),0); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float rv=pow(max(dot(r,v),0),32); //Kosinus kata pomiedzy wektorami do obserwatora i odbitym, podniesiony do wykladnika Phonga

	pixelColor=ka*la+kd*ld*vec4(nl,nl,nl,1)+ks*ls*vec4(rv,rv,rv,0);
}
