#version 410

in vec3 Position;
in vec3 Normal;

uniform struct LightInfo {
    vec4 Position;
    vec3 La; // ambient light intensity
    vec3 L; // diffuse and specular light intensity
} Light;

uniform struct MaterialInfo {
    vec3 Ka; // Ambient reflectivity
    vec3 Kd; // Difuse reflectivity
    vec3 Ks; // specular reflectivity
    float Shininess;
}Material;

layout(location = 0) out vec4 FragColor;

vec3 blinnPhong(vec3 position, vec3 n)
{
    vec3 ambient = Light.La * Material.Ka;
    vec3 s = normalize(Light.Position.xyz - position);
    float sDotN = max(dot(s, n), 0.0);
    vec3 diffuse = Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if (sDotN > 0.0)
    {
        vec3 v = normalize(-position.xyz);
        vec3 h = normalize(v + s);
        spec = Material.Ks * pow(max(dot(h, n), 0.0), Material.Shininess);
    }
    return ambient + Light.L * (diffuse + spec);
}

void main()
{
    FragColor = vec4(blinnPhong(Position, normalize(Normal)), 1.0);
}
