#version 330

uniform sampler2D diffuseMap; //Zmienna reprezentujaca jednostke teksturujaca
uniform sampler2D normalMap;  //Zmienna reprezentujaca jednostke teksturujaca
uniform sampler2D heightMap;  //Zmienna reprezentujaca jednostke teksturujaca

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 i_l; //wektor do swiatla(przestrzen oka)
in vec4 i_v; //wektor do obserwatora(przestrzen oka)
in vec4 i_n; //wektor normalny (przestrzen oka)
in vec2 iTexCoord0; //wspolrzedne teksturowania

vec2 parallaxTexCoords(vec4 v, vec2 t, float h, float s){
float temph=h;
vec2 tc=t;
vec2 ti =-h*v.xy/s;
float hi=-h/s;
//while(h*texture(heightMap,tc).r<temph)
//{
//	tc=tc+ti;
//	if(tc.x>1 || tc.y>1 || tc.y<0 || tc.x<0) discard;
//	temph=temph+hi;
//}
return tc;
}

void main(void) {
	vec4 v=normalize(i_v);
	vec2 newCoords=parallaxTexCoords(v,iTexCoord0,0.05,100);
    //vec4 n=normalize(vec4(texture(normalMap,iTexCoord0).rgb*2-1,0));
	vec4 n=normalize(i_n);
    vec4 l=normalize(i_l);
    vec4 r=reflect(-l,n);

    vec4 ka=vec4(0,0,0,0); //Kolor obiektu w swietle otoczenia
    vec4 kd=texture(diffuseMap,iTexCoord0); //Kolor obiektu w swietle rozproszonym
    vec4 ks=vec4(1,1,1,0); //Kolor obiektu w swietle odbitym

    vec4 la=vec4(0,0,0,0); //Kolor swiatla otoczenia
    vec4 ld=vec4(1,1,1,1); //Kolor swiatla rozpraszanego
    vec4 ls=vec4(1,1,1,0); //Kolor swiatla odbijanego

    float nl=max(dot(n,l),0); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float rv=pow(max(dot(r,v),0),10); //Kosinus kata pomiedzy wektorami do obserwatora i odbitym, podniesiony do wykladnika Phonga

	pixelColor=ka*la+kd*ld*vec4(nl,nl,nl,1)+ks*ls*vec4(rv,rv,rv,0);
}
