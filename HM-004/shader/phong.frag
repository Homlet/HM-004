#version 400

out vec4 o_color;

in vec3 f_pos;
in vec3 f_tex;
in vec3 f_normal;
in vec4 f_shadow;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;

uniform sampler2D       u_2D;       // Unit 0.
uniform sampler2DShadow u_2DShadow; // Unit 1.

#define SPECULAR_EXPONENT   20.0
#define SPECULAR_BRIGHTNESS 0.3

#define SHADOW_BIAS 0.005

vec2 pDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random( vec4 seed )
{
	float d = dot( seed, vec4( 12.9898, 78.233, 45.164, 94.673 ) );
	return fract( sin( d ) * 43758.5453 );
}

void main( void )
{
	// Cosine of angle between normal and light.
	// Used in shadow and diffuse parts.
	float NdotL = dot( f_normal, u_lightDir );

	// Shadow occlusion part:
	float shade = 1.0;
	// float bias = clamp( 0.005 * tan( acos( NdotL ) ), 0.0, 0.01 );
	for ( int i = 0; i < 4; i++ )
	{
		int index = int( mod( 16 * random( vec4( f_pos, i ) ), 16 ) );
		shade -= 0.15 * texture(
				u_2DShadow, vec3( f_shadow.xy + pDisk[index] / 1000.0, ( f_shadow.z - SHADOW_BIAS ) / f_shadow.w )
		);
	}

	// Ambient part:
	float ambient = 0.15;

	// Diffuse part:
	float diffuse = max( NdotL, 0.0 ) * 2.0;

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
