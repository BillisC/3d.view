#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoord;
out TANGENT {
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} tng;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    TexCoord = aTexCoord;

    vec3 FragPos = vec3(model * vec4(aPos, 1.0));

    mat3 NormalMat = mat3(transpose(inverse(model)));

    vec3 T = normalize(NormalMat * aTangent);
    vec3 B = normalize(NormalMat * aBitangent);
    vec3 N = normalize(NormalMat * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    tng.TangentLightPos = TBN * lightPos;
    tng.TangentViewPos  = TBN * viewPos;
    tng.TangentFragPos  = TBN * FragPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
