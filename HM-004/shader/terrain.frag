#version 400

out vec4 o_color;

in vec3  f_tex;
in vec3  f_normal;
in float f_fog;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;

uniform sampler2D u_2D; // Unit 0.

float random( vec4 seed )
{
	float d = dot( seed, vec4( 12.9898, 78.233, 45.164, 94.673 ) );
	return fract( sin( d ) * 43758.5453 );
}

void main( void )
{
	// Ambient part:
	float ambient = 0.6;

	// Diffuse part:
	float diffuse = max( dot( f_normal, u_lightDir ), 0.0 );

	o_color = vec4(
		mix(
			vec3( 0.0 ),
			( ambient + diffuse ) *
			u_lightColor *
			texture( u_2D, f_tex.st ).rgb,
			f_fog
		),
		1.0
	);
}
