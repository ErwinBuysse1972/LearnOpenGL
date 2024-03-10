#version 410

in vec3 Position;
in vec3 Normal;

in vec4 voAdditiveColor;
//in vec4 voColor0;
//in vec4 vov4MiscData_Mrn;
in vec4 voSkyLightFogParameters;
in vec4 voSkylight_ambient;
in vec4 voSkylight_diffuse;
//in vec4 voVertexNormal;
//in vec3 voOutPosEye;
//in vec3 voPosClip;

uniform struct LightInfo {
    vec4 Position;  // Light position in eye coords.
    vec3 La;        // Ambient light intesity
    vec3 L;         // Diffuse and specular light intensity
} Light;

uniform struct MaterialInfo {
    vec3 Ka;            // Ambient reflectivity
    vec3 Kd;            // Diffuse reflectivity
    vec3 Ks;            // Specular reflectivity
    float Shininess;    // Specular shininess factor
} Material;

uniform struct FogInfo {
    float MaxDist;
    float MinDist;
    float Density;
    vec3 Color;
} Fog;

layout(location = 0) out vec4 outFragColor;

vec3 blinnPhong(vec3 position, vec3 n) {
    vec3 ambient = Light.La * Material.Ka;
    vec3 s = normalize(Light.Position.xyz - position);
    float sDotN = max(dot(s, n), 0.0);
    vec3 diffuse = Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if (sDotN > 0.0) {
        vec3 v = normalize(-position.xyz);
        vec3 h = normalize(v + s);
        spec = Material.Ks *
            pow(max(dot(h, n), 0.0), Material.Shininess);
    }
    return ambient + Light.L * (diffuse + spec);
}
vec3 saturate(vec3 x)
{
    return clamp(x, 0.0, 1.0);
}

void main() {

    vec3 SkyLightFogParams = voSkyLightFogParameters.rgb;
    //vec3 SkyLightFogParams = vec3(0.0f, 0.0f, 0.0f);
    vec4 flVoAdditiveColor = voAdditiveColor;
    vec3 SkyLightFogFactor = vec3(0.0, 0.0, 0.0);

    vec3 shadeColor = blinnPhong(Position, normalize(Normal));
    outFragColor = vec4(shadeColor, 1.0);


    float fInvEmissiveScale = 2.0f;
    vec3 v3FogParams;
    v3FogParams.rgb = SkyLightFogParams.rgb * fInvEmissiveScale;
    v3FogParams = exp(-abs(v3FogParams.rgb));

    SkyLightFogParams.rgb = vec3(flVoAdditiveColor.a, voSkylight_ambient.a, voSkylight_diffuse.a);
    SkyLightFogFactor = exp(-abs(SkyLightFogParams.rgb));

    outFragColor.rgb = mix(flVoAdditiveColor.rgb, outFragColor.rgb, SkyLightFogFactor.rgb);
    outFragColor.rgb = 1.0 - saturate(2.0 * outFragColor.rgb);
    outFragColor.rgb = 1.0 - (outFragColor.rgb * outFragColor.rgb);


}