#version 400

out float o_depth;

void main( void )
{
	o_depth = gl_FragCoord.z;
}
