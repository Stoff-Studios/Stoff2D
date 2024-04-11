#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 fTexCoord;

uniform mat4 proj;
uniform mat4 view;

void main() {
    fTexCoord = aTexCoord;
    gl_Position = proj * view * vec4(aPos.x, aPos.y, 0.0, 1.0);
}
