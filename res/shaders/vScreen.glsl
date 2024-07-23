#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColour;

out vec2 fTexCoord;
out vec4 fColour;

uniform mat4 proj;

void main() {
    fTexCoord = aTexCoord;
    fColour   = aColour;
    gl_Position = proj * vec4(aPos.x, aPos.y, 0.0, 1.0);
}
