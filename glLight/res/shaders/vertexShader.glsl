#version 430 core

out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoords;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vFragPos = vec3(model * vec4(position, 1.0));
    vNormal = normal;
    vTexCoords = texCords;
}