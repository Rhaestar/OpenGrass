#version 450

in vec2 position;

out vec2 UV;

uniform mat4 view;
uniform mat4 projection;


void main() {
    vec4 position4 = vec4(position.x, 0., position.y, 1.);
    gl_Position = projection * view * position4;
    UV = vec2((position.x + 1.)/2., (position.y + 1.)/2.);
}
