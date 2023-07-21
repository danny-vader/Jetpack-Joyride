#version 330 core
in vec2 TexCoords;
out vec4 color;
in vec4 pos;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    float x = pos.x - 0.5f;
    float y = pos.y - 0.5f;
    float distance = sqrt((x*x) + (y*y)) * 2.0f;
    float colour = smoothstep(1.0, 0.0, distance);    
    color = vec4(1.0f, colour, colour, colour) * texture(image, TexCoords);
}  