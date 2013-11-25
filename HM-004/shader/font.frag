#version 400

out vec4 o_color;

in vec2 f_tex;

uniform sampler2D u_2D;

void main( void )
{
	o_color = vec4( 1.0, 1.0, 1.0, texture( u_2D, f_tex ).a );
}
