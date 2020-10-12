#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 outTex;
	vec3 outPos;
	vec3 outNor;
}gs_in[];

out vec2 texCoord;
out vec3 pos;
out vec3 nor;

vec3 getNormal(){
	vec3 vecA = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 vecB = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 normal = normalize(cross(vecA, vecB));
	return normal;
}

void main()
{    
	float constant = 1.0;
	vec3 nor = getNormal();
	gl_Position = gl_in[0].gl_Position/* + vec4(nor * constant, 0.0)*/;
	texCoord = gs_in[0].outTex;            
	pos = gs_in[0].outPos;            
	nor = gs_in[0].outNor;            
	EmitVertex();                     
	gl_Position = gl_in[1].gl_Position/* + vec4(nor * constant, 0.0)*/;
	texCoord = gs_in[1].outTex;            
	pos = gs_in[1].outPos;            
	nor = gs_in[1].outNor;            
	EmitVertex();                     
	gl_Position = gl_in[2].gl_Position/* + vec4(nor * constant, 0.0)*/;
	texCoord = gs_in[2].outTex;
	pos = gs_in[2].outPos;
	nor = gs_in[2].outNor;
	EmitVertex();
	EndPrimitive();
}