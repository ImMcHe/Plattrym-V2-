#version 330 core
out vec4 fragColor;
in vec2 textCoord;
uniform sampler2D mainTex;

void main()
{
	fragColor=texture(mainTex,textCoord);
}
