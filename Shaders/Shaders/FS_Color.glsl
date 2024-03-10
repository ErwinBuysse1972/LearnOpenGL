#version 430 core

// in
in vec4 color_fs ;
in vec2 texcoord0_fs ; // uv
in vec3 world_pos_fs ;
// out
out vec4 outFragColor ;


// observer
uniform vec3 m_EyePos ;

// material
uniform vec4 m_Color ;

// light
uniform vec4 m_LightAmbientColor ;
uniform vec4 m_LightDiffuseColor ;
uniform vec3 m_LightDir ;


void main()
{
	vec3 eye_dir = world_pos_fs - m_EyePos ;
	float eye_len = length( eye_dir );

	float fog_density = 0.001f ;
	float fog_factor = clamp( exp(-pow( fog_density * eye_len, 2.0 )), 0, 1 );
	vec3 fog_color = vec3( 0.0f, 1.0f, 0.0f );

	//if( eye_len < 100.0f ) discard ;
    outFragColor.xyz = mix( fog_color, color_fs.xyz, fog_factor ) ;
	outFragColor.a = color_fs.a ;
	outFragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}