#version 330 core
out vec4 fc;
in vec2 textCoord;
uniform sampler2D mainTex;
uniform float rf;

void main()
{
	fc=texture(mainTex,textCoord);
	fc=vec4(fc.x,rf*fc.y,rf*fc.z,fc.w);
}
