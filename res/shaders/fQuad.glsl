#version 330 core
out vec4 FragColor;

in vec2 fTexCoord;

uniform sampler2D spriteSheet; 

void main() {
    FragColor = texture(spriteSheet, fTexCoord);
} 
