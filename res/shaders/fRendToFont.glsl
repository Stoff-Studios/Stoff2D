#version 450 core

in vec2 fTexCoords;
layout(location = 0) out vec4 colour;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, fTexCoords).r);
    colour = vec4(textColor, 1.0) * sampled;
    //colour = vec4(1.0, 1.0, 1.0, 1.0);
} 
