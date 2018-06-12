#version 330
 
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
 
in vec4 vertex;
in vec4 color;
in vec4 normal;
 
out vec4 gColor;
 
void main(void) {
    gColor=vec4(0.6,0.6,0.6,1);
    gl_Position=P*V*M*vertex;
}