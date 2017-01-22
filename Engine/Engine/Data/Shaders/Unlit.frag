#version 330 core

in vec4 v2f_color;
in vec2 v2f_texCords;

layout(location = 0) out vec4 out_color;

uniform sampler2D tex;

void main()
{
	vec4 colorTex = texture(tex, v2f_texCords);

	if( colorTex.r == 0 &&
		colorTex.g == 0 &&
		colorTex.b == 0) 
	{
		out_color = v2f_color;
	}
	else {
		out_color = colorTex * v2f_color;
	}
}
