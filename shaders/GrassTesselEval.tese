#version 450

layout(triangles, fractional_odd_spacing, cw) in;

void main() {

    vec4 p1 = gl_TessCoord.x * gl_in[0].gl_Position;
    vec4 p2 = gl_TessCoord.y * gl_in[1].gl_Position;
    vec4 p3 = gl_TessCoord.z * gl_in[2].gl_Position;
    gl_Position = normalize(p1+p2+p3);
}
