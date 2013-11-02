#version 400

out vec4 o_color;

in vec3 f_pos;
in vec3 f_tex;

void main( void )
{
	o_color = vec4( f_tex, 1.0 );
}
