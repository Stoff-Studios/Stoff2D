#version 450 core
out vec4 FragColor;

in vec2 fTexCoord;
in vec4 fColour;

uniform sampler2D uTexture; 

void main() {
    FragColor = fColour * texture(uTexture, fTexCoord);
} 
