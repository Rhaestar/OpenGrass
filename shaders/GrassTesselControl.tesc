#version 450

layout(vertices = 3) out;

void main() {
    
    gl_TessLevelOuter[0] = 42;
    gl_TessLevelOuter[1] = 30;
    gl_TessLevelOuter[2] = 25;

    gl_TessLevelInner[0] = 50;
    gl_TessLevelOuter[0] = 2000;
    gl_TessLevelOuter[1] = 1200;
    gl_TessLevelOuter[2] = 1000;

    gl_TessLevelInner[0] = 2000;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
