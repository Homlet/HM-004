#version 400 core

layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_tex;
layout(location = 2) in vec3 i_normal;

out vec2 f_tex;

uniform mat4 u_M;

void main( void )
{
	f_tex = i_tex.st;

	gl_Position = u_M * vec4( i_pos.xy, 0.0, 1.0 );
}
