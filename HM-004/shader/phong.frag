#version 400

out vec4 o_color;

in vec3 f_pos;
in vec3 f_tex;
in vec3 f_normal;
in vec3 f_shadow;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;

uniform sampler2D u_2D;       // Unit 0.
uniform sampler2D u_2DShadow; // Unit 1.

#define SPECULAR_EXPONENT   20.0
#define SPECULAR_BRIGHTNESS 0.3

#define SHADOW_BIAS 0.005

void main( void )
{
	// Shadow occlusion part:
	float shade = 1.0;
	if ( texture( u_2DShadow, f_shadow.xy ).z > f_shadow.z - SHADOW_BIAS )
	{
		shade = 0.25;
	}

	// Ambient part:
	float ambient = 0.15;

	// Diffuse part:
	float diffuse = max( dot( f_normal, u_lightDir ), 0.0 ) * 2.0;

	// Specular part:
	vec3 mid = -normalize( normalize( f_pos ) - u_lightDir );
	float specular = pow( max( dot( f_normal, mid ), 0.0 ), SPECULAR_EXPONENT ) * SPECULAR_BRIGHTNESS;

	// Combined:
	o_color = vec4(
		specular + (
			shade * 
			( ambient + diffuse ) *
			u_lightColor *
			texture( u_2D, f_tex.xy ).rgb
		),
		1.0
	);
}
