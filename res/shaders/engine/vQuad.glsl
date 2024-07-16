#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColour;
layout (location = 3) in float aTexSlot;

out vec2 fTexCoord;
out vec4 fColour;
out float fTexSlot;

uniform mat4 proj;
uniform mat4 view;

void main() {
    fTexCoord = aTexCoord;
    fColour   = aColour;
    fTexSlot  = aTexSlot;
    gl_Position = proj * view * vec4(aPos.x, aPos.y, 0.0, 1.0);
}
