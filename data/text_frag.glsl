#version 330

uniform sampler2D text_sampler;

in vec2 texcoord;
in vec4 color;

out vec4 out_color;

void main()
{
	out_color = vec4(1.0, 1.0, 1.0, texture(text_sampler, texcoord).a) * color;
}