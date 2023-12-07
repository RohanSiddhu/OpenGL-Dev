#version 420 core

out vec3 fragPos;
out vec3 normal;
out vec3 LightPos;
out vec2 texCoords;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vsNormal;
layout (location = 2) in vec2 tCoords;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    fragPos = vec3(view * model * vec4(position, 1.0f));
    normal = mat3(transpose(inverse(view * model))) * vsNormal;
    //normal = vsNormal;
    LightPos = vec3(view * vec4(lightPos, 1.0f));
    texCoords = tCoords;
}
