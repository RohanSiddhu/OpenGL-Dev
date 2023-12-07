#version 420 core

out vec4 fragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    // vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


in vec3 fragPos;
in vec3 normal;
in vec3 LightPos;
in vec2 texCoords;

uniform Material material;
uniform Light light;

void main() {
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(LightPos - fragPos);
    // vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    // specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    float distance = length(LightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0f);
}
