#version 430 core

// in
in vec3 inPosition ;
in vec2 inTexcoord0 ;
in vec3 inNormal ;

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


void main()
{
	// fragment position
	vec4 world_pos = ModelMatrix * vec4( inPosition, 1.0 ) ; 
    gl_Position = ( ProjectionMatrix * ViewMatrix ) * world_pos ;
	// world normal
	vec3 normal = normalize( ModelMatrix * vec4( inNormal, 0 ) ).xyz ;

	// light
	float n_dot_l = dot( normal, m_LightDir );
	float light_diffuse = max( 0, n_dot_l );

	color_fs = ( m_LightAmbientColor + light_diffuse * m_LightDiffuseColor ) * m_Color ;
	world_pos_fs = world_pos.xyz / world_pos.w ;    
	texcoord0_fs = inTexcoord0 ;
}