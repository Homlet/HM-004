#version 400

out vec4 o_color;

in vec3 f_pos;
in vec3 f_tex;
in vec3 f_normal;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;

uniform sampler2D u_2D;

#define SPECULAR_EXPONENT   20.0
#define SPECULAR_BRIGHTNESS 0.3

void main( void )
{
	// Ambient part:
	float ambient = 0.1;

	// Diffuse part:
	float diffuse = max( dot( f_normal, u_lightDir ), 0.0 );

	// Specular part:
	vec3 mid = -normalize( normalize( f_pos ) - u_lightDir );
	float specular = pow( max( dot( f_normal, mid ), 0.0 ), SPECULAR_EXPONENT ) * SPECULAR_BRIGHTNESS;

	// Combined:
	o_color = vec4(
		specular + (
			( ambient + diffuse ) *
			u_lightColor *
			texture( u_2D, f_tex.xy ).rgb
		),
		1.0
	);
}
