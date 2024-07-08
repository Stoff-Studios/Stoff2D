#version 330 core
layout(location = 0) out vec4 result;

in vec2 fTexCoord;
in vec4 fColour;

uniform sampler2D spriteSheet; 

void main() {
    result = fColour * texture(spriteSheet, fTexCoord);
} 
