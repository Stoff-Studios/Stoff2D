#version 450 core
layout(location = 0) out vec4 result;

in vec2 fTexCoord;
in vec4 fColour;
in float fTexSlot;

uniform sampler2D uTextures[32]; 

void main() {
    int texSlot = int(fTexSlot);
    result = fColour * texture(uTextures[texSlot], fTexCoord);
} 
