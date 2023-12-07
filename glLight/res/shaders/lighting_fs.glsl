#version 430 core

out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texImg;

void main() {
    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Texture
    vec3 textureColor = vec3(texture(texImg, vTexCoords));

    vec3 result = (ambient + diffuse + specular) * textureColor;
    FragColor = vec4(result, 1.0f);
}
