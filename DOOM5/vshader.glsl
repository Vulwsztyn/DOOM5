#version 330
 
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
<<<<<<< HEAD

struct Light {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	vec4 lightColor;
};
uniform Light light;  
uniform vec4 viewPos; 


//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wektor normalny w wierzcholku w przestrzeni modelu
in vec2 texCoord0; //wspolrzedne teksturowania
in vec4 c1; //Pierwsza kolumna macierzy TBN^-1 (na razie ignorowana)
in vec4 c2; //Druga kolumna macierzy TBN^-1 (na razie ignorowana)
in vec4 c3; //Trzecia kolumna macierzy TBN^-1 (na razie ignorowana)

//Zmienne interpolowane
out vec4 FragPos; 
out vec4 Normal; 
out vec2 iTexCoord0; //wspolrzedne teksturowania
out vec4 TangentLightPos;
out vec4 TangentViewPos;
out vec4 TangentFragPos;


void main(void) {
	vec4 T = normalize(vec4(M * c1));
   vec4 B = normalize(vec4(M * c2));
   vec4 N = normalize(vec4(M * c3));
   mat4 TBN = transpose(mat4(T, B, N,vec4(0,0,0,1)));

   TangentLightPos = TBN * light.position;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * M * vertex;

    FragPos = M * vertex;
    Normal = mat4(transpose(inverse(M))) * c3;  

    iTexCoord0=texCoord0;
	gl_Position = P * V * M * vertex;
}


=======
uniform vec3 debug1;
//uniform vec3 debug2;
//uniform vec3 debug3;
//
in vec4 vertex;
in vec4 color;
in vec4 normal;
 

out vec4 gColor;
out vec3 pozycja;
void main(void) {
    gColor=vertex;
	pozycja=debug1;
	//if(vertex.x==debug1.x && vertex.y==debug1.y && vertex.z==debug1.z)gColor=vec4(1,0,0,1);
	//if(vertex.x==debug2.x && vertex.y==debug2.y && vertex.z==debug2.z)gColor=vec4(1,0,0,1);
	//if(vertex.x==debug3.x && vertex.y==debug3.y && vertex.z==debug3.z)gColor=vec4(1,0,0,1);
    gl_Position=P*V*M*vertex;
}
>>>>>>> A-Kolizja
