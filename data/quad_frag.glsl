#version 330

in	vec4 color;
in  vec2 texcoord;

out vec4 out_color;

uniform sampler2D diffuse_sampler;

void main ()
{
	out_color = texture(diffuse_sampler, texcoord) + (color * 0.0);
}