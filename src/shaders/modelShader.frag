#version 450 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

uniform Material material;
uniform Light light;

void main() {
    // Ambient Light
    vec3 ambientLight = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb * light.color;

    // Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = light.diffuse * diff * texture(material.texture_diffuse1, TexCoord).rgb * light.color;

    // Specular Light
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoord).rgb * light.color;

    FragColor = vec4((ambientLight + diffuseLight + specular), 1.0);
}
