#version 430

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;
layout(location = 3) in vec4 VertexTangent;

uniform struct LightInfo 
{
    vec4 Position;  // light position in commera coord
    vec3 L;         // specular and diffuse light intensity
    vec3 La;        // ambient light intensity
}Light;

out vec3 LightDir;
out vec2 TexCoord;
out vec3 ViewDir;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    vec3 tang = normalize(NormalMatrix * vec3(VertexTangent));
    vec3 binnormal = normalize(cross(norm, tang)) * VertexTangent.w;

    // Matrix for transformation to tangent space
    mat3 toObjectLocal = mat3(
        tang.x, binnormal.x, norm.x,
        tang.y, binnormal.y, norm.y,
        tang.z, binnormal.z, norm.z );

    // Transform light direction and view direction to tangent space
    vec3 pos = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
    LightDir = toObjectLocal * (Light.Position.xyz - pos);
    ViewDir = toObjectLocal * normalize(-pos);
    TexCoord = VertexTexCoord;

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
