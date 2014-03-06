#version 400

out vec4 o_color;

in vec3  f_tex;
in vec3  f_normal;
in float f_fog;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;

uniform sampler2DArray u_2DArray; // Unit 0.

void main( void )
{
	// Ambient part:
	float ambient = 0.6;

	// Diffuse part:
	float diffuse = max( dot( f_normal, u_lightDir ), 0.0 );

	o_color = vec4(
		mix(
			vec3( 0.529, 0.808, 0.922 ),
			( ambient + diffuse ) *
			u_lightColor *
			texture( u_2DArray, vec3( f_tex.st, 0 ) ).rgb,
			f_fog
		),
		1.0
	);
}
