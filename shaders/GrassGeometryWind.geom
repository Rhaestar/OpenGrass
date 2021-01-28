#version 450

#define PI 3.1415926538f
#define EPSILON 0.01f

layout (triangles) in;
layout (triangle_strip, max_vertices = 11) out;

uniform mat4 projection;
uniform mat4 view;
uniform float time;

uniform sampler2D noise;
out vec3 color;

void main() {
    float wind = radians(sin(time * 0.001f - PI / 2.0) * 3.f * PI + 3.f * PI);
    mat4 MV = projection * view;
    vec3 object_color = vec3(0.f, 0.25f, 0.f); 

    vec4 middle = ( gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position ) / 3.f;
    //middle += vec4(1.f,0.f,0.f,0.f) * wind * 0.1f;

    float n = texture(noise, middle.xz).b * 2 * PI;
    float c = cos(n);
    float s = sin(n);
    vec4 orientation = vec4(c, 0.0, s, 0.0);
    vec4 direction = orientation * 0.05f;
    vec4 normal = normalize(vec4(-orientation.z, 0.0, orientation.x, 0.0));
    //if (normal.x < 0)
      //  normal = -normal;

    vec2 light_dir = normalize(vec2(0.0, 1.0));
    color = object_color * clamp(abs(dot(light_dir, orientation.xz)),0.0,1.0);
    color += vec3(0.f, 0.1f, 0.f);
    double height = 0.0;
    int maxheight = 5;
    for (int i = 0; i < maxheight; ++i)
    {
        float x = float(i) / float(maxheight);
        float y = (x * x) / 2.f;
        color += vec3(0.05f, 0.07f, 0.f);
        gl_Position = middle + vec4( 0.0, height, 0.0, 0.0);
        gl_Position += normal * (0.003 - 0.0006 * i);
        gl_Position += direction * y;
        gl_Position.x = middle.x + (gl_Position.x - middle.x)*cos(wind) - (gl_Position.y - middle.y) * sin(wind);
        gl_Position.y = middle.y + (gl_Position.x - middle.x)*sin(wind) + (gl_Position.y - middle.y) * cos(wind);

        gl_Position = MV * gl_Position;
        EmitVertex();

        gl_Position = middle + vec4( 0.0, height, 0.0, 0.0);
        gl_Position += normal * -(0.003 - 0.0006 * i);
        gl_Position += direction * y;
        gl_Position.x = middle.x + (gl_Position.x - middle.x)*cos(wind) - (gl_Position.y - middle.y) * sin(wind);
        gl_Position.y = middle.y + (gl_Position.x - middle.x)*sin(wind) + (gl_Position.y - middle.y) * cos(wind);

        gl_Position = MV * gl_Position;
        EmitVertex();

        height += 0.015;
    }

    gl_Position = middle + vec4( 0.0, height, 0.0, 0.f);
    gl_Position += direction * 0.5f;
        gl_Position.x = middle.x + (gl_Position.x - middle.x)*cos(wind) - (gl_Position.y - middle.y) * sin(wind);
        gl_Position.y = middle.y + (gl_Position.x - middle.x)*sin(wind) + (gl_Position.y - middle.y) * cos(wind);
    gl_Position = MV * gl_Position;
    EmitVertex();
    EndPrimitive();
}  

