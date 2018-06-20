#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 player;


//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wektor normalny w wierzcholku w przestrzeni modelu
in vec2 texCoord0; //wspolrzedne teksturowania
in vec4 c1; //Pierwsza kolumna macierzy TBN^-1 (na razie ignorowana)
in vec4 c2; //Druga kolumna macierzy TBN^-1 (na razie ignorowana)
in vec4 c3; //Trzecia kolumna macierzy TBN^-1 (na razie ignorowana)

//Zmienne interpolowane
out vec4 lightDist;
out vec4 ilightDir; //wektor do swiatla(przestrzen oka)
out vec4 iviewDir; //wektor do obserwatora(przestrzen oka)
out vec4 inormalDir; //wektor normalny (przestrzen oka)
out vec2 iTexCoord0; //wspolrzedne teksturowania


void main(void) {

	mat4 itbn=mat4(c1,c2,c3,vec4(0,0,0,1));
	//itbn=mat4(vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(0,0,0,1));

    vec4 lp=vec4(player,1); //Wspolrzedne swiatla w przestrzeni swiata

    ilightDir=normalize(itbn*inverse(M)*lp-itbn*vertex);
	lightDist=itbn*inverse(M)*lp-itbn*vertex;
    iviewDir=normalize(itbn*inverse(V*M)*vec4(0,0,0,1)-itbn*vertex);
    inormalDir=normalize(itbn*normal);

    iTexCoord0=texCoord0;

	gl_Position=P*V*M*vertex;
} 


