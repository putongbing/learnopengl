#version 430 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec3 outNor[];

void main()
{    
	float constant = 0.5;
	gl_Position = gl_in[0].gl_Position;       
	EmitVertex();  
	gl_Position = gl_in[0].gl_Position + vec4(outNor[0] * constant, 0.0);       
	EmitVertex();
	EndPrimitive();
	gl_Position = gl_in[1].gl_Position;        
	EmitVertex();                       
	gl_Position = gl_in[1].gl_Position + vec4(outNor[1] * constant, 0.0);        
	EmitVertex();
	EndPrimitive();
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position + vec4(outNor[2] * constant, 0.0);
	EmitVertex();
	EndPrimitive();
}