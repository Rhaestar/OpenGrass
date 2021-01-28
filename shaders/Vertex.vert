#version 450

in vec2 position;

uniform mat4 view;
uniform mat4 projection;


void main() {
    //2D Plane to 3D
    vec4 position4 = vec4(position.x, 0., position.y, 1.);
    gl_Position = projection * view * position4;
}
