#version 450 core
out vec4 FragColor;

in vec2 fTexCoord;
in vec4 fColour;
in float fTexSlot;

uniform sampler2D spriteSheet; 

void main() {
    vec4 result = fColour * texture(spriteSheet, fTexCoord);
    FragColor = result;
} 
