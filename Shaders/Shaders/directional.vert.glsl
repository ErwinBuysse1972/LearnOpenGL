#version 410
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;


out vec3 Color;

uniform struct LightInfo {
    vec4 Position; // light position in eye coords
    vec3 La; // ambient light intensity
    vec3 L; // diffuse and specular light intensity
} Light;

uniform struct MaterialInfo {
    vec3 Ka;            // ambient reflectivity
    vec3 Kd;            // diffuse reflectivity
    vec3 Ks;            // specular reflectivity
    float Shininess;    // specular shininess factor
} Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec3 phongModel(vec3 position, vec3 n)
{
    vec3 ambient = Light.La * Material.Ka;
    vec3 s;
    if (Light.Position.w == 0.0)
        s = normalize(Light.Position.xyz);
    else
        s = normalize(Light.Position.xyz - position);
    float sDotN = max(dot(s, n), 0.0);
    vec3 diffuse = Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if (sDotN > 0.0)
    {
        vec3 v = normalize(-position.xyz);
        vec3 r = reflect(-s, n);
        spec = Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
    }
    return ambient + Light.L * (diffuse + spec);
}

void main()
{
    vec3 camNorm = normalize(NormalMatrix * VertexNormal);
    vec3 camPosition = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

    // Evaluate the lighting equation for each light
    Color = phongModel(camPosition, camNorm);

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
