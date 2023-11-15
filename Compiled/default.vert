#version 330 core
layout(location=0)in vec2 pos;
layout(location=1)in vec2 text;
out vec2 textCoord;
uniform vec2 sc;

void main()
{
	gl_Position=vec4(pos*sc,0.F,1.F);
	textCoord=text;
}
