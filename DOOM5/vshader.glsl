#version 330
 
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
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