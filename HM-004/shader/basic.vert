#version 400

layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_tex;
layout(location = 2) in vec3 i_normal;

out vec3 f_pos;
out vec3 f_tex;

uniform mat4 u_MV;
uniform mat4 u_P;

void main( void )
{
	f_pos = i_pos;
	f_tex = i_tex;

	gl_Position = u_P * u_MV * vec4( i_pos, 1.0 );
}
