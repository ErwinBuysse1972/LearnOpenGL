#version 400

in vec3 GPosition;
in vec3 GNormal;
flat in int GIsEdge;

layout(location = 0) out vec4 FragColor;

struct LightInfo {
    vec4 Position;  // Light position in eye coords.
    vec3 Intensity; // A,D,S intensity
};
struct MaterialInfo {
    vec3 Ka;            // Ambient reflectivity
    vec3 Kd;            // Diffuse reflectivity
    vec3 Ks;            // Specular reflectivity
    float Shininess;    // Specular shininess factor
};
uniform LightInfo Light;
uniform MaterialInfo Material;
uniform vec4 LineColor;

const int levels = 3;
const float scaleFactor = 1.0 / levels;
vec3 toonShade()
{
    vec3 s = normalize(Light.Position.xyz - GPosition.xyz);
    vec3 ambient = Material.Ka;
    float cosine = dot(s, GNormal);
    vec3 diffuse = Material.Kd * ceil(cosine * levels) * scaleFactor;

    return Light.Intensity * (ambient + diffuse);
}

void main() {
    if (GIsEdge == 1) {
        FragColor = LineColor;
    }
    else {
        FragColor = vec4(toonShade(), 1.0);
    }

}