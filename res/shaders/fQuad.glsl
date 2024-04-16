#version 330 core
out vec4 FragColor;

in vec2 fTexCoord;
in vec4 fColour;

uniform sampler2D spriteSheet; 

void main() {
    vec4 result = fColour * texture(spriteSheet, fTexCoord);
    FragColor = result;
} 
