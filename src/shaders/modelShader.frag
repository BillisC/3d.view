#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in TANGENT {
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} tng;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_normal1;
    sampler2D texture_specular1;
};

struct Light {
    vec3 ambient;
    vec3 specular;
    vec3 color;
};

uniform Material material;
uniform Light light;

void main() {
    // Ambient Light
    vec3 ambientLight = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb * light.color;

    // Normal Map
    vec3 norm = texture(material.texture_normal1, TexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    // Diffuse Light
    vec3 lightDir = normalize(tng.TangentLightPos - tng.TangentFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * texture(material.texture_diffuse1, TexCoord).rgb * light.color;

    // Specular Light
    vec3 viewDir = normalize(tng.TangentViewPos - tng.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoord).rgb * light.color;

    FragColor = vec4((ambientLight + diffuseLight + specular), 1.0);
}
