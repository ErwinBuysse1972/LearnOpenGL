#version 430 core
layout( vertices=4 ) out;


// in
in vec2 texcoord0_tcs[];
in int is_edge_tcs[];
// out
out vec2 texcoord0_tes[];

// transformations
uniform mat4 ModelViewProjectionMatrix ;
uniform mat4 ProjectionMatrix ;
uniform mat4 ViewMatrix ;
uniform mat4 ModelMatrix ;
     
// observer
uniform vec3 m_EyePos ;
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
// variables
uniform float m_Tessellation_Outer ;
uniform float m_Tessellation_Inner ;



// subdivide
void main() 
{
    // pass-through: copy the vertex unmodified
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    texcoord0_tes[gl_InvocationID] = texcoord0_tcs[gl_InvocationID] ;
	
    vec4 pos = gl_out[0].gl_Position ;   
    vec4 world_pos = ModelMatrix * vec4( pos.xy, 0, 1 ) ;
    world_pos /= world_pos.w ;    
    vec3 eye_pos = world_pos.xyz - m_EyePos ;
    
	float reduc = 1 ; // length( eye_pos ) > 200.0f ? 4 : 1 ; // WARN: stitching required for this transition band   	
	float inner = m_Tessellation_Inner / reduc, outer = m_Tessellation_Outer / reduc ;
        
    // Inner subdivision
    gl_TessLevelInner[0] = inner ;
    gl_TessLevelInner[1] = inner ;

	// Outer subdivision edge
    gl_TessLevelOuter[0] = outer ;
    gl_TessLevelOuter[1] = outer ;
    gl_TessLevelOuter[2] = outer ;
    gl_TessLevelOuter[3] = outer ;

	// stiching
	if( is_edge_tcs[0] + is_edge_tcs[1] == 2 ) gl_TessLevelOuter[1] *= 2 ; else
	if( is_edge_tcs[1] + is_edge_tcs[2] == 2 ) gl_TessLevelOuter[2] *= 2 ; else
	if( is_edge_tcs[2] + is_edge_tcs[3] == 2 ) gl_TessLevelOuter[3] *= 2 ; else
	if( is_edge_tcs[3] + is_edge_tcs[0] == 2 ) gl_TessLevelOuter[0] *= 2 ; 

}