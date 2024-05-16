#version 450 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 baseColor;

uniform vec3 lightColor;

void main() {
    // Ambient Light
    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lightColor;

    // Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * lightColor;

    // Specular Light
    float specularStrength = 0.6;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    FragColor = texture(ourTexture, TexCoord) * vec4((ambientLight + diffuseLight + specular) * baseColor, 1.0);
}
