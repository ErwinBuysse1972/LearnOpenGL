#version 430 core

// in
in vec3 inPosition ;
in vec2 inTexcoord0 ;
in vec3 inNormal ;

// out
out vec4 color_fs ;
out vec2 texcoord0_fs ;
out vec3 world_pos_fs ;

// fog out
out vec4 voAdditiveColor;
out vec4 voColor0;
out vec4 vov4MiscData_Mrn;
out vec4 voSkyLightFogParameters;
out vec4 voSkylight_ambient;
out vec4 voSkylight_diffuse;
out vec4 voVertexNormal;
out vec3 voOutPosEye;
out vec3 voPosClip;



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


// textures
uniform sampler2D txColor ;					// color, diffuse

// ocean surface
uniform sampler2D txsampler_dz[3];			// height
uniform sampler2D txsampler_dxy[3];			// displacement
uniform sampler2D txsampler_slope[3];		// slope
uniform sampler2D txsampler_ddxy[3];		// jacobian

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
}VP;
// properties
uniform float m_tile_size ;
uniform mat4 m_vrTransform_ModelViewMatrixInverseTranspose;
uniform mat4 m_vrTransform_ModelViewProjectionMatrix;


const float tx_lod_bias = 0.0 ;
vec2 mul(vec2 v, mat2 m) { return v * m; }
vec3 mul(mat3 m, vec3 v) { return m * v; }
vec3 mul(vec3 v, mat3 m) { return v * m; }
vec4 mul(mat4 m, vec4 v) { return m * v; }
vec4 mul(vec4 v, mat4 m) { return v * m; }
mat3 mul(mat3 m1, mat3 m2) { return m1 * m2; }
mat4 mul(mat4 m1, mat4 m2) { return m1 * m2; }
float saturate(float x) { return clamp(x, 0.0, 1.0); }
vec3 saturate(vec3 x) { return clamp(x, 0.0, 1.0); }
float DisplacementZ(vec2 uv)
{
    float dz = texture(txsampler_dz[0], uv, tx_lod_bias).x;
    dz += texture(txsampler_dz[1], uv * 8, tx_lod_bias).x;
    dz += texture(txsampler_dz[2], uv * 8 * 8, tx_lod_bias).x;
    return dz;
}
vec2 DisplacementXY(vec2 uv)
{
    vec2 dxy = texture(txsampler_dxy[0], uv, tx_lod_bias).xy;
    dxy += texture(txsampler_dxy[1], uv * 8, tx_lod_bias).xy;
    dxy += texture(txsampler_dxy[2], uv * 8 * 8, tx_lod_bias).xy;
    return 0.5 * dxy;
}
vec3 Displacement(vec2 uv)
{
    return vec3(DisplacementXY(uv), DisplacementZ(uv));
}
void skyLightAmbientDiffuseFog(vec4 vEyeSpacePos, out vec4 vLightDiffuse, out vec4 vLightAmbient, out vec4 vFogAdditiveColorSW, out vec3 OutSkyLightFogParams)
{
    // Shared variables
    vec4 light0Position = VP.m_vrLight_Position_0;
    vec3 skylightTemp1 = vec3(0.0, 0.0, 0.0);
    vec4 vlVpEnv_ambientColor_0 = VP.m_vpEnv_ambientColor_0;

    //---- skyLightAmbientDiffuseFog stage Begin

    //---- ffpSkyLightAmbientAndDiffuseLightColorAndFogColorComputation Atom Snippet Begin
    {

        vec3 densityMultiplier = vec3(VP.m_vpEnv_densityMultiplier_0, VP.m_vpEnv_blueWeight_0.a, VP.m_vpEnv_blueHorizon_0.a);
        vec3 distanceMultiplier = vec3(VP.m_vpEnv_distanceMultiplier_0, VP.m_vpEnv_hazeWeight_0.a, VP.m_vpEnv_hazeHorizon_0.a);

        // vEyeSpacePos is a vector from the eye to the vertex being shaded
        vec3 P = vEyeSpacePos.xyz;
        vec3 Pn = normalize(P);

        // pAltitude is the length of projection of the P vector onto
        // the vertical axis of the scene - the difference in height
        // between the observer's eye and the vertex being shaded.
        float pAltitude = dot(P, VP.m_vsChannel_AxisZEyeSpace);

        // Scale the P vector so that its vertical component falls
        // within the range of -vpEnv_maxY_0 to vpEnv_maxY_0.
        if (pAltitude > VP.m_vpEnv_maxY_0) P *= (VP.m_vpEnv_maxY_0 / pAltitude);
        if (pAltitude < -VP.m_vpEnv_maxY_0) P *= (-VP.m_vpEnv_maxY_0 / pAltitude);

        // Compute the skylight fog factor in three components.
        skylightTemp1.rgb = VP.m_vpEnv_temp1_0.rgb * P.z * densityMultiplier.rgb * distanceMultiplier.rgb;

        // Approximate the sun path distance and attenuate the sunlight
        // color which is stored in vpEnv_sunlight_0.rgb
        // sunVar controls the amount that attenuation color affects the
        // sunlight color (both diffuse and additive components).
        // Each sunlight color is affected by the vertical component of
        // the normalized direction from the observer to the dominant
        // celestial light source (vp_Env_sunlight_0.w).
        float sunVar = VP.m_vpEnv_sunlight_0.w;
        sunVar = 1.0 / max(0.0001, sunVar);
        vec3 sunlightDiffuse = VP.m_vpEnv_illumSunlightColor_0.rgb * exp(-VP.m_vpEnv_sunlight_0.rgb * sunVar);

        // The additive sunlight color is additionally affected by the
        // positive vertical component of a normalized vector from the
        // observer to the vertex in question.
        sunVar = max(0.0, dot(Pn, VP.m_vsChannel_AxisZEyeSpace)) + VP.m_vpEnv_sunlight_0.w;
        sunVar = 1.0 / max(0.0001, sunVar);
        vec3 sunlightAdditive = VP.m_vpEnv_illumSunlightColor_0.rgb * exp(-VP.m_vpEnv_sunlight_0.rgb * sunVar);

        vlVpEnv_ambientColor_0.rgb += (vlVpEnv_ambientColor_0.rgb) * VP.m_vpEnv_cloudShadow_0 * 0.5;

        vec3 cameraPos = vec3(0.0, -VP.m_vpEnv_cameraAltitude_0, 0.0);
        pAltitude = dot(cameraPos, VP.m_vsChannel_AxisZEyeSpace);
        P = vEyeSpacePos.xyz;
        P.y -= pAltitude;
        Pn = normalize(P);
        sunVar = 1.0 - dot(Pn, light0Position.xyz);
        sunVar = max(sunVar * VP.m_vpEnv_glow_0.x, 0.001);
        sunVar = pow(sunVar, VP.m_vpEnv_glow_0.z) + 0.25;

        // Compute the skylight ambient and diffuse colors
        vLightDiffuse.rgb = sunlightDiffuse * 0.5;
        vLightAmbient.rgb = vlVpEnv_ambientColor_0.rgb * 0.25;

        // Compute the additive fog color
        vFogAdditiveColorSW.a = 1.0;
        vFogAdditiveColorSW.rgb = VP.m_vpEnv_blueHorizon_0.rgb * VP.m_vpEnv_blueWeight_0.rgb * (sunlightAdditive * (1.0 - VP.m_vpEnv_cloudShadow_0) +
            vlVpEnv_ambientColor_0.rgb) +
            (VP.m_vpEnv_hazeHorizon_0.rgb * VP.m_vpEnv_hazeWeight_0.rgb) * (sunlightAdditive * (1.0 - VP.m_vpEnv_cloudShadow_0) *
                sunVar +
                vlVpEnv_ambientColor_0.rgb);

    }
    //---- ffpSkyLightAmbientAndDiffuseLightColorAndFogColorComputation Atom Snippet End

    //---- ffpSkyLightFogParametersPerFragment Atom Snippet Begin
    {

        vFogAdditiveColorSW.a = skylightTemp1.r;
        vLightAmbient.a = skylightTemp1.g;
        vLightDiffuse.a = skylightTemp1.b;

    }
    //---- ffpSkyLightFogParametersPerFragment Atom Snippet End

    //---- ffpPassSkyLightFogParamViaParam Atom Snippet Begin
    {
        OutSkyLightFogParams.rgb = skylightTemp1.rgb;

    }
    //---- ffpPassSkyLightFogParamViaParam Atom Snippet End

    //---- skyLightAmbientDiffuseFog stage End



}
vec4 lit(float NdotL, float NdotH, float shininess)
{
    float specular = (NdotL < 0.0) || (NdotH <= 0.0) ? 0.0 : pow(NdotH, shininess);
    return vec4(1.0, max(0.0, NdotL), specular, 1.0);
}

void main()
{
    vec4 OutPosEye = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 vlNormal = vec4(inNormal, 1.0);
    vec4 light0Ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);//VP.m_vrLight_Ambient_0;
    vec4 light0Diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);//VP.m_vrLight_Diffuse_0;
    vec3 SkyLightFogParams = vec3(0.0, 0.0, 0.0);
    vec4 skylightLight = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 outColor0 = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 materialDiffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);//VP.m_vrMaterial_Diffuse_Front;
    vec3 OutNormalEye = vec3(0.0, 1.0, 0.0);
    mat4 modelViewIT = m_vrTransform_ModelViewMatrixInverseTranspose;
    vec4 OutPosClip = vec4(0.0, 0.0, 0.0, 1.0);
    mat4 modelViewProjection = m_vrTransform_ModelViewProjectionMatrix;


	// fragment position
	vec4 world_pos = ModelMatrix * vec4( inPosition, 1.0 ) ;
	world_pos /= world_pos.w ;

	vec2 pos_world_uv = world_pos.xy / m_tile_size ;
	vec3 displacement = Displacement( pos_world_uv );

    gl_Position = ( ProjectionMatrix * ViewMatrix ) * ( world_pos + vec4( displacement, 0 ) ) ;

	// world normal
	vec3 normal = normalize( ModelMatrix * vec4( inNormal, 0 ) ).xyz ;

	// light
	float n_dot_l = dot( normal, m_LightDir );
	float light_diffuse = max( 0, n_dot_l );

	color_fs = ( m_LightAmbientColor + light_diffuse * m_LightDiffuseColor ) * m_Color ;
    color_fs = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    world_pos_fs = world_pos.xyz / world_pos.w ;
	texcoord0_fs = inTexcoord0 ;



    OutPosEye = ModelMatrix * ViewMatrix * vec4(inPosition, 1.0);
    OutPosClip = mul(modelViewProjection, vec4(inPosition, 1.0));
    outColor0 = vec4(0.0, 0.0, 0.0, materialDiffuse.a);
    OutNormalEye.xyz = normalize(mul(mat3(modelViewIT), vlNormal.xyz));

    skyLightAmbientDiffuseFog(OutPosEye, light0Diffuse, light0Ambient, voAdditiveColor, SkyLightFogParams);

    // Within a 400 m radius around the observer, white caps appear
   // at full strength.  From 400 to 800 m from the observer, the
   // vertex's white cap intensity fades out to 0.0.  Beyond 800 m
   // white caps are no longer visible.
   // A provision is made for low range scales, which when set low
   // push the white cap falloff too far away from the eye.

   // vpMarine_v4Params0VS.x is the range scale.
    float fRangeScale = saturate(VP.m_vpMarine_v4Params0VS.x + 0.25);
    float fFogDistance = -OutPosEye.z * fRangeScale;
    const float fDenominator = (1.0 / 400.0);
    vov4MiscData_Mrn.y = 1.0 - saturate((fFogDistance - 400.0) * fDenominator);

    skylightLight = outColor0;
    outColor0 = vec4(0.0, 0.0, 0.0, outColor0.a);
    outColor0.rgb = saturate(outColor0.rgb);
    outColor0.rgb = (outColor0.rgb * 0.5) + skylightLight.rgb;

    // Fogging parameters are adjusted based on the range scale. This
 // prevents blending to the fog color too early on the water for 
 // narrow fields of view.
 // Whether the ffpSkyLightFogParameters or the per-fragment variant
 // is used, ensure voSkyLightFogParameters gets updated.
    voSkyLightFogParameters.rgb = SkyLightFogParams.rgb;
    voSkyLightFogParameters.rgb *= clamp(VP.m_vpMarine_v4Params0VS.x * 10.0, 0.5, 1.0);

    // VL -     FragmentLightingVertexNormal: pass on to fragment shader
        // Note that we need to pack normal into [0,1] range supported by color
    voVertexNormal.xyz = (OutNormalEye.xyz + vec3(1.0, 1.0, 1.0)) * 0.5;
    voOutPosEye = OutPosEye.xyz;
    voColor0 = outColor0;
    voPosClip = OutPosClip.xyw;

    voSkylight_diffuse = light0Diffuse;
    voSkylight_ambient = light0Ambient;
}