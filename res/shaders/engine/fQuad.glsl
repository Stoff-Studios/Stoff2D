#version 450 core
out vec4 FragColor;

in vec2 fTexCoord;
in vec4 fColour;
flat in float fTexSlot;

uniform sampler2D uTextures[32]; 

void main() {
    vec4 result;

    int texSlot = int(fTexSlot);
    if (texSlot == 0) {
        result = fColour;
    } else {
        result = fColour * texture(uTextures[texSlot], fTexCoord);
    }

    FragColor = result;
} 
