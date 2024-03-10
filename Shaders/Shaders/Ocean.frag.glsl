#version 430 core

// constants
#define PI		3.1415926535897932384626433832795
#define PI_2	(PI/2)
#define PI_4	(PI/4)

// DX vs OGL
#define lerp mix

// saturate
float saturate( float value ) { return clamp( value, 0, 1 ); }
vec2 saturate( vec2 value ) { return clamp( value, 0, 1 ); } //{ return vec2( saturate( value.x ), saturate( value.y ) ); }
vec3 saturate( vec3 value ) { return clamp( value, 0, 1 ); } //{ return vec3( saturate( value.x ), saturate( value.y ), saturate( value.z ) ); }
vec4 saturate( vec4 value ) { return clamp( value, 0, 1 ); } //{ return vec4( saturate( value.x ), saturate( value.y ), saturate( value.z ), saturate( value.w ) ); }



//*****************************************************************************************************************************************************************************************************
// Reflection
//

// refractive index water
const float n_water = 1.33f ; 
// specular intensity: when the direction to the light source, the direction to the viewer, and the halfway vector are all identical
const float R0_water = pow( (n_water - 1)/(n_water + 1), 2 ); // = 0.02


// nVidia WP-01401-001-v01
// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
// http://kylehalladay.com/blog/tutorial/2014/02/18/Fresnel-Shaders-From-The-Ground-Up.html
// https://en.wikibooks.org/wiki/GLSL_Programming/Unity/Specular_Highlights_at_Silhouettes
// Schlick Approximation: R = R0 + (1 - R0)(1 - cos_r)^5, cos_r = n.v
// Empricial Approximation: R = max(0, min(1, bias + scale * (1.0 + I.N)power))
// grazing light reflection: cos_r = 1
// kt = 1 - kr = RefractFactor = 1 - ReflectFactor
float FresnelReflectFactor_Schlick( float R0, float cos_r )
{
	return R0 + (1 - R0) * pow( abs( 1 - cos_r ), 5.0 ); 
	return lerp( R0, 1, pow( abs( 1 - cos_r ), 5.0 ) ); 
}


float FresnelReflectFactor( float ni, float nt, float cosi )
{
    // Snell's law: ni.sini = nt.sinr
	float sini = sqrt( 1 - cosi * cosi );
	float sint = ni*sini / nt ;
	float cost = sqrt( 1 - sint * sint ); 

	// p-polarized light = parallel
	float rp = ( ni * cost - nt * cosi ) / ( ni * cost + nt * cosi ); 
	// s-polarized light = perpedicular
	float rs = ( ni * cosi - nt * cost ) / ( ni * cosi + nt * cost ); 
	// ratio of reflected light
	return ( rp*rp + rs*rs ) / 2 ; // = kr 
}




// forward declarations
vec3 atmosphere( vec3 eye_dir, vec3 eye_location, vec3 sun_dir );





// in
in vec4 color_fs ;
in vec2 texcoord0_fs ; // uv
in vec3 world_pos_fs ;

// fog in
in vec4 voAdditiveColor;
in vec4 voColor0;
in vec4 vov4MiscData_Mrn;
in vec4 voSkyLightFogParameters;
in vec4 voSkylight_ambient;
in vec4 voSkylight_diffuse;
in vec4 voVertexNormal;
in vec3 voOutPosEye;
in vec3 voPosClip;

// out
out vec4 outFragColor ;


// observer
uniform vec3 m_EyePos ;

// material
uniform vec4 m_Color ;
const float m_SpecularHardness = 500.0f ; // demo

// light
uniform vec4 m_LightAmbientColor ;
uniform vec4 m_LightDiffuseColor ;
uniform vec3 m_LightDir ; // [TOWARDS]

// fog
uniform vec4 m_FogColor ;
uniform float m_FogDensity ;

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


vec2 Slope( vec2 uv )
{
	vec2 dxy = vec2( 0, 0 );
	for( int i = 0 ; i < NLAYERS ; ++i ) dxy += texture( txsampler_slope[i], uv * m_tile_downscale[i], tx_lod_bias ).xy ;
	return dxy ;
}

vec3 Normal( vec2 uv )
{
    // normal
	vec2 dxy = Slope( uv );
    vec3 normal = normalize( vec3( -dxy, 1.0f ) ); // Z = UP, SWAP XY!?

	// show single cell
	//if( uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1 ) return vec3( 0, 0, 0 );

	return normal ;
}

float Jacobian( vec2 uv )
{
	vec4 ddxy = vec4( 0, 0, 0, 0 );
	for( int i = 0 ; i < NLAYERS ; ++i ) ddxy += texture( txsampler_ddxy[i], uv * m_tile_downscale[i], tx_lod_bias ) ;
	return ( 1 + ddxy.x )*( 1 + ddxy.w ) - ddxy.y * ddxy.z  ;
}


void main()
{   

//    vec3 SkyLightFogParams = vec3(0.0, 0.0, 0.0);
    vec3 SkyLightFogParams = vec3(voSkyLightFogParameters.x, voSkyLightFogParameters.y, voSkyLightFogParameters.z);
    vec4 flVoAdditiveColor = voAdditiveColor;
    vec3 SkyLightFogFactor = vec3(0.0f, 0.0f, 0.0f);

    // uv
    vec2 pos_world_uv = world_pos_fs.xy / m_tile_size ;
	vec3 normal = Normal( pos_world_uv ) ; //vec3( 0, 0, 1 );

	vec3 eye_dir = world_pos_fs - m_EyePos ;
	float eye_len = length( eye_dir );
	eye_dir = normalize( eye_dir );

	// reflection
	// WARN curved earth 
	// limits the mimimum angle = reflection always a bit more upward
	// light direction changes
	vec3 reflect_dir = reflect( eye_dir /*incident*/, normal );
	reflect_dir.z = abs( reflect_dir.z );
	// WARN: sampling atmosphere at observer, not at reflected point = world_pos_fs
	vec3 reflect_color = atmosphere( reflect_dir.xzy, vec3( m_EyePos.x, world_pos_fs.z, m_EyePos.y ), m_LightDir.xzy );

	// Blinn-Phong
	// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
	float n_dot_l = dot( normal, m_LightDir );
	float diffuse = max( 0, n_dot_l );

	vec3 halfway = normalize( -eye_dir + m_LightDir );
	//vec3 halfway = reflect( m_LightDir, normal );
	float n_dot_h = dot( normal, halfway );
	float specular = n_dot_l < 0 ? 0 : pow( max( 0, n_dot_h ), m_SpecularHardness );

	// Fresnel
	float n_dot_v = max( 0, -dot( eye_dir, normal ) ) ; // cos_r, positive only
	//float kr = FresnelReflectFactor_Schlick( R0_water, n_dot_v );
	float kr = FresnelReflectFactor( 1, n_water, n_dot_v );
	float kt = 1.0 - kr ;

	// Water
	const vec3 water_color = vec3( 0.6, 1.0, 0.8 );

	// peak
	const float z_max = 2.0f ;
	float z_norm = world_pos_fs.z / z_max ;
	float peak_factor = 0.35 * smoothstep( -0.5, 1.0, z_norm );
	float ocean_light = dot( m_LightDir, vec3( 0, 0, 1 ) );
	vec3 peak_color = 0.5 * ocean_light * water_color ;
	//outFragColor.rgb = mix( outFragColor.rgb, peak_color, peak_factor );

	// mixing water colors
	const vec3 deep_water_color = 0.05 * water_color ;
	vec3 refract_color = ( m_LightAmbientColor.rgb + m_LightDiffuseColor.rgb * max( 0, m_LightDir.z ) ) * mix( deep_water_color, peak_color, peak_factor ) ;
	vec3 fresnel_color = lerp( refract_color, reflect_color, kr );

	//if( eye_len < 100.0f ) discard ;
	outFragColor.rgb = fresnel_color /*+ 0.5 * specular * sun_color*/ ; // mix( fog_color, color_fs.xyz, fog_factor );
	outFragColor.rgb += 0.01 * diffuse * vec3( 1, 1, 1 );
	//outFragColor.rgb = normal.z > 0.999 ? vec3( 0, 0, 1 ) : normal.y < 0 ? vec3( -normal.y, 0, 0 ) : vec3( 0, normal.y, 0 ) ;
	outFragColor.a = color_fs.a ;

	// foam
	const float foam_min = 0.4, foam_max = 0.8 ;
	float jacobian = Jacobian( pos_world_uv );
	float foam_factor = smoothstep( foam_min, foam_max, 1 - jacobian );
	// foam color
	const vec3 foam_color0 = vec3( 1, 1, 1 );
	vec3 foam_color = 0.95 * foam_color0 * diffuse ;
	outFragColor.rgb = mix( outFragColor.rgb, foam_color, foam_factor );

    float fInvEmissiveScale = 2.0f;
    vec3 v3FogParams;
    v3FogParams.rgb = SkyLightFogParams.rgb * fInvEmissiveScale;
    v3FogParams = exp(-abs(v3FogParams.rgb));

    SkyLightFogParams.rgb = vec3(flVoAdditiveColor.a, voSkylight_ambient.a, voSkylight_diffuse.a);
    SkyLightFogFactor = exp(-abs(SkyLightFogParams.rgb));

    outFragColor.rgb = mix(flVoAdditiveColor.rgb, outFragColor.rgb, SkyLightFogFactor.rgb);
    outFragColor.rgb = 1.0 - saturate(2.0 * outFragColor.rgb);
    outFragColor.rgb = 1.0 - (outFragColor.rgb * outFragColor.rgb);
    
	// apply gamma correction
	// https://learnopengl.com/Advanced-Lighting/Gamma-Correction
    const float gamma_inv = 1.0f / 2.2f ;
    //outFragColor.rgb = pow( outFragColor.rgb, vec3( gamma_inv ) );
	outFragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}












//*****************************************************************************************************************************************************************************************************
// Math
//

// Ray-Sphere intersection (quadratic equation)
// assumes the sphere is centered at the origin
vec2 rsi( vec3 r0, vec3 rd, float sr ) 
{
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b*b) - 4.0*a*c;
    if( d < 0 ) return vec2( 1e5, -1e5 ); // DUMMY, no intersection result.x > result.y
    return vec2(
        (-b - sqrt(d))/(2.0*a),
        (-b + sqrt(d))/(2.0*a)
    );
}




//*****************************************************************************************************************************************************************************************************
// Atmosphere
//

// https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/simulating-sky/simulating-colors-of-the-sky

#define iSteps 16
#define jSteps 8


// r = eye_dir,								    // normalized ray/eye direction
// r0 = eye_location,							  // ray/eye origin
// pSun = sun_dir,								    // direction/position of the sun
// iSun = g_sun_intensity,						// intensity of the sun
// g_planet_radius,						// radius of the planet in meters
// g_planet_radius + g_atmosphere_thickness,		// radius of the atmosphere in meters (used for ray intersection)
// g_beta_rayleigh,								   // Rayleigh scattering coefficient
// g_beta_mie,								         // Mie scattering coefficient: betaM(21e-6f)
// g_atmosphere_thickness_rayleigh,	 // Rayleigh scale height
// g_atmosphere_thickness_mie,				 // Mie scale height
// g_scattering_direction_mie         // Mie preferred scattering direction
vec3 atmosphere( vec3 r, vec3 r0, vec3 pSun, float iSun, float rPlanet, float rAtmos, vec3 kRlh, float kMie, float shRlh, float shMie, float g )
{
    // Normalize the sun and view directions.
    pSun = normalize(pSun);
    r = normalize(r);

    // Calculate the step size of the primary ray.
    vec2 p = rsi( r0, r, rAtmos );
    if( p.x > p.y ) return vec3(0,0,0); // no intersection with atmosphere
    p.y = min( p.y, rsi( r0, r, rPlanet ).x );
    float iStepSize = (p.y - p.x) / float(iSteps);

    // Initialize the primary ray time.
    float iTime = 0.0;

    // Initialize accumulators for Rayleigh and Mie scattering.
    vec3 totalRlh = vec3(0,0,0);
    vec3 totalMie = vec3(0,0,0);

    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {

        // Calculate the primary ray sample position.
        vec3 iPos = r0 + r * (iTime + iStepSize * 0.5);

        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;

        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Calculate the step size of the secondary ray.
        float jStepSize = rsi(iPos, pSun, rAtmos).y / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0;

        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {

            // Calculate the secondary ray sample position.
            vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;

            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}



// eye_location local Earth surface
vec3 atmosphere( vec3 eye_dir, vec3 eye_location, vec3 sun_dir )
{
	// sun
	const float g_sun_intensity = 22.0f ; // intensity of the sun
  
	// settings
	float g_planet_radius = 6371e3 ; // [m] Earth = 6371 km
	float g_atmosphere_thickness = 60e3 ; // [m] (used for ray intersection)
	float g_atmosphere_thickness_rayleigh = 7994 ; // [m] Rayleigh scale height, Thickness of the atmosphere if density was uniform (Hr) = 7994m (used for optical depth)
	float g_atmosphere_thickness_mie = 1200 ; // [m] Mie scale height, " (Hm) = 1200m
	vec3 g_beta_rayleigh = vec3( 5.5e-6, 13.0e-6, 22.4e-6 ); // Rayleigh scattering coefficient (= molecule, per color): betaR(3.8e-6f, 13.5e-6f, 33.1e-6f)
	float g_beta_mie = 21e-6f ; // Mie scattering coefficient [>= 2e-6] (= dust, dominant haze/sun, ): betaM()
	float g_scattering_direction_mie = 0.758f; // 0.758f ; // Mie preferred scattering direction (0.65[haze] .. 1[clear], more blue, sharper sun geometry, no haze around sun)

	// translate
	eye_location.y += g_planet_radius ; // ray/eye origin, above earth surface, Y = UP

	return atmosphere
	(
		eye_dir,								    // normalized ray/eye direction
		eye_location,							  // ray/eye origin
		sun_dir,								    // position of the sun
		g_sun_intensity,						// intensity of the sun
		g_planet_radius,						// radius of the planet in meters
		g_planet_radius + g_atmosphere_thickness,		// radius of the atmosphere in meters (used for ray intersection)
		g_beta_rayleigh,								   // Rayleigh scattering coefficient
		g_beta_mie,								         // Mie scattering coefficient: betaM(21e-6f)
		g_atmosphere_thickness_rayleigh,	 // Rayleigh scale height
		g_atmosphere_thickness_mie,				 // Mie scale height
		g_scattering_direction_mie         // Mie preferred scattering direction
	);
}