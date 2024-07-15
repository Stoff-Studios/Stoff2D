#version 450 core
out vec4 FragColor;

in vec2 fTexCoord;
in vec4 fColour;
in float fTexSlot;

uniform sampler2D tex0; 

void main() {
    vec4 result = fColour * texture(tex0, fTexCoord);
    FragColor = result;
} 
