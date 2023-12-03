#version 330 core
out vec4 fragColor;
in vec2 textCoord;
uniform sampler2D mainTex;
uniform float rf;

void main()
{
	vec4 tmp=texture(mainTex,textCoord);
	fragColor=vec4(tmp.x,rf*tmp.y,rf*tmp.z,tmp.w);
}
