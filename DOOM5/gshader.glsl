#version 330

layout (triangles) in;
layout (line_strip) out;
layout (max_vertices=4) out;

in vec4 gColor[];
in vec3 pozycja[];
out vec4 iColor;

void main(void) {
	int i;
//	vec2 p=vec2(pozycja[1].x,pozycja[1].z);
//	vec2 t[3];
//	for(i=0;i<3;i++){
//	t[i]=vec2(gColor[i].x,gColor[i].z);
//	}
//	
//	iColor=vec4(0.6,0.6,0.6,1);
//
//	float a=(p.x - t[1].x) * (t[0].y - t[1].y) - (p.y - t[1].y) * (t[0].x - t[1].x);
//	float b=(p.x - t[2].x) * (t[1].y - t[2].y) - (p.y - t[2].y) * (t[1].x - t[2].x);
//	float c=(p.x - t[0].x) * (t[2].y - t[0].y) - (p.y - t[0].y) * (t[2].x - t[0].x);
//	bool q=a<0.0f;
//	bool w=b<0.0f;
//	bool e=c<0.0f;
//
//	if((q==w)&&(q==e)){
//	iColor=vec4(0,0.2,1,1);
//	}
	if (gColor[0].x>gColor[1].x) iColor=vec4(0,0.2,1,1);
	else iColor=vec4(1,0.2,0,1);
    for (i=0;i<gl_in.length();i++) {
        gl_Position=gl_in[i].gl_Position;
        EmitVertex();
    }
	    gl_Position=gl_in[0].gl_Position;
        EmitVertex();
    EndPrimitive();
}
