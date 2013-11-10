#version 400 core

out vec4 o_color;

in vec3 f_pos;
in vec3 f_tex;

uniform sampler2D u_2D;

void main( void )
{
	o_color = texture( u_2D, f_tex.xy );
}
