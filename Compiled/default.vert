#version 330 core
layout(location=0)in vec2 pos;
layout(location=1)in vec2 text;
out vec2 textCoord;
uniform vec2 sc;
uniform vec2 sg;
uniform vec2 ps;
uniform float ds;

void main()
{
	gl_Position=vec4((ps+pos)*(sc*sg),0.F,1.F);
	gl_Position+=sin(gl_Position*120.F)*ds;
	textCoord=text;
}
