#version 450

in vec2 position;

void main() {
    vec4 position4 = vec4(position.x, 0., position.y, 1.);
    gl_Position = position4;
}
