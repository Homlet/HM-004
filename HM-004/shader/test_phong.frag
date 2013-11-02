#version 400

out vec4 o_color;

in vec3 f_tex;
in vec3 f_normal;

uniform sampler2D u_2D;

void main( void )
{
	vec3 dir = normalize( vec3( 1.0, 1.0, 0.7 ) );
	vec3 col = vec3( 0.9, 0.95, 0.7 );
	float diffuse = max( dot( f_normal, dir ), 0.25 ) * 3.0;

	o_color = vec4( diffuse * texture( u_2D, f_tex.xy ).rgb * col, 1.0 );
}
