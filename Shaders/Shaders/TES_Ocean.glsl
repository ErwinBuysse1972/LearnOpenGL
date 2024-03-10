#version 430 core
layout( quads, equal_spacing, ccw ) in;


// in
in vec2 texcoord0_tes[];
// out
out vec4 color_fs ;
out vec2 texcoord0_fs ;
out vec3 world_pos_fs ;


// transformations
uniform mat4 ModelViewProjectionMatrix ;
uniform mat4 ProjectionMatrix ;
uniform mat4 ViewMatrix ;
uniform mat4 ModelMatrix ;

// observer
uniform vec3 m_EyePos ;

// material
uniform vec4 m_Color ;

// light
uniform vec4 m_LightAmbientColor ;
uniform vec4 m_LightDiffuseColor ;
uniform vec3 m_LightDir ;

// vegaprime fog
uniform struct VegaPrime
{
    vec4 m_vpEnv_ambientColor_0;
    vec4 m_vpEnv_blueHorizon_0;
    vec4 m_vpEnv_hazeHorizon_0;
    vec4 m_vpEnv_sunlight_0;
    vec4 m_vpEnv_blueWeight_0;
    vec4 m_vpEnv_hazeWeight_0;
    vec4 m_vpEnv_temp1_0;
    vec4 m_vpEnv_glow_0;
    vec4 m_vpEnv_cameraPos_0;
    vec4 m_vpEnv_illumSunlightColor_0;
    vec4 m_vpEnv_lightNormal_0;
    vec4 m_vpMarine_v4Params0VS;
    vec4 m_vrLight_Position_0;
    vec4 m_vrMaterial_Diffuse_Front;
    vec3 m_vsChannel_AxisZEyeSpace;
    float m_vpEnv_cloudShadow_0;
    float m_vpEnv_densityMultiplier_0;
    float m_vpEnv_distanceMultiplier_0;
    float m_vpEnv_maxY_0;
    float m_vpEnv_cameraAltitude_0;
    float m_vpEnv_minColor_0;
    uint m_unLastWrite;
    mat4 m_vrTransform_ModelViewMatrixInverseTranspose;
    mat4 m_vrTransform_ModelViewProjectionMatrix;
    vec4 m_vrLight_Ambient_0;
    vec4 m_vrLight_Diffuse_0;
}VP;


// textures
uniform sampler2D txColor ;					// color, diffuse

// number of spectral texture layers
#define NLAYERS 3
// ocean surface
uniform sampler2D txsampler_dz[NLAYERS];		// height
uniform sampler2D txsampler_dxy[NLAYERS];		// displacement
uniform sampler2D txsampler_slope[NLAYERS];		// slope
uniform sampler2D txsampler_ddxy[NLAYERS];		// jacobian


// properties
uniform float m_tile_size ;
uniform float m_tile_downscale[NLAYERS] ;

const float tx_lod_bias = 0.0 ;


float DisplacementZ( vec2 uv )
{
	float dz = 0 ;
	for( int i = 0 ; i < NLAYERS ; ++i ) dz += texture( txsampler_dz[i], uv * m_tile_downscale[i], tx_lod_bias ).x ;
	return dz ;
}

vec2 DisplacementXY( vec2 uv )
{
	vec2 dxy = vec2( 0, 0 );
	for( int i = 0 ; i < NLAYERS ; ++i ) dxy += texture(txsampler_dxy[i], uv * m_tile_downscale[i], tx_lod_bias).xy;
	return dxy ;
}

vec3 Displacement( vec2 uv )
{
	return vec3( DisplacementXY( uv ), DisplacementZ( uv ) );
}





vec2 lerp( vec2 p[4], float u, float v ) { return p[0] * (1-u) * (1-v) + p[1] * u * (1-v) + p[3] * v * (1-u) + p[2] * u * v; }
vec3 lerp( vec3 p[4], float u, float v ) { return p[0] * (1-u) * (1-v) + p[1] * u * (1-v) + p[3] * v * (1-u) + p[2] * u * v; }
vec4 lerp( vec4 p[4], float u, float v ) { return p[0] * (1-u) * (1-v) + p[1] * u * (1-v) + p[3] * v * (1-u) + p[2] * u * v; }

vec2 lerp( vec2 p0, vec2 p1, vec2 p2, vec2 p3, vec2 uv ) { float u = uv.x, v = uv.y ; return p0 * (1 - u) * (1 - v) + p1 * u * (1 - v) + p3 * v * (1 - u) + p2 * u * v; }
vec3 lerp( vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec2 uv ) { float u = uv.x, v = uv.y ; return p0 * (1 - u) * (1 - v) + p1 * u * (1 - v) + p3 * v * (1 - u) + p2 * u * v; }
vec4 lerp( vec4 p0, vec4 p1, vec4 p2, vec4 p3, vec2 uv ) { float u = uv.x, v = uv.y ; return p0 * (1 - u) * (1 - v) + p1 * u * (1 - v) + p3 * v * (1 - u) + p2 * u * v; }






void main()
{
	vec4 pos = lerp( gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.xy );
	vec2 texcoord0 = lerp( texcoord0_tes[0], texcoord0_tes[1], texcoord0_tes[2], texcoord0_tes[3], gl_TessCoord.xy );

	// fragment position
	vec4 world_pos = ModelMatrix * vec4( pos.xy, 0, 1 ) ;
	world_pos /= world_pos.w ;

	vec2 pos_world_uv = world_pos.xy / m_tile_size ;
	vec3 displacement = Displacement( pos_world_uv );

    gl_Position = ( ProjectionMatrix * ViewMatrix ) * ( world_pos + vec4( displacement, 0 ) ) ;
	
	color_fs = /*( m_LightAmbientColor + light_diffuse * m_LightDiffuseColor ) **/ m_Color;
    color_fs = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    world_pos_fs = vec3( world_pos.xy / world_pos.w, displacement.z ) ;
	texcoord0_fs = texcoord0 ;
}