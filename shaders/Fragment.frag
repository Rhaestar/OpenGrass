#version 450

in vec2 UV;

uniform sampler2D text;

layout (location=0) out vec4 output_color;

void main()
{
    //output_color = texture(text, UV);
    output_color = vec4(0.f, 0.3f, 0.f, 1.f);
}
