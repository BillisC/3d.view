#version 450 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 baseColor;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;

void main() {
    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 

    float specularStrength = 0.6;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    FragColor = texture(ourTexture, TexCoord) * vec4((ambientLight + diffuseLight + specular) * baseColor, 1.0);
}
