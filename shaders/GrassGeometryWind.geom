#version 450

#define PI 3.1415926538f
#define EPSILON 0.01f

layout (triangles) in;
layout (triangle_strip, max_vertices = 11) out;

uniform mat4 projection;
uniform mat4 view;
uniform float time;
uniform float wave;

uniform sampler2D noise;

out vec3 color;

vec2 rot2D(vec2 point, vec2 center, float angle)
{
    //Translate to origin
    vec2 origin = point - center;
    //Apply rotation
    vec2 rot = vec2(origin.x * cos(angle) - origin.y * sin(angle), origin.x * sin(angle) + origin.y * cos(angle));
    //Translate back
    rot += center;
    return rot;
}

void main() {

    mat4 MV = projection * view;
    vec3 object_color = vec3(0.f, 0.25f, 0.f); 

    vec4 middle = ( gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position ) / 3.f;

    //Compute and offset wind value
    float wind = radians(sin(time * 0.001f - PI / 2.0) * 3.f * PI + 3.f * PI);
    //Add position information for waves
    if (wave > 0.5f)
        wind *= abs(sin(middle.x * 2.5f + time * 0.001f));

    //Get blade orientation from noise texture
    float n = texture(noise, middle.xz).b * 2 * PI;
    float c = cos(n);
    float s = sin(n);
    vec4 orientation = vec4(c, 0.f, s, 0.f);
    vec4 direction = orientation * 0.05f;
    vec4 normal = normalize(vec4(-orientation.z, 0.f, orientation.x, 0.f));

    //Compute Grass base color (Ambient and Diffuse)
    vec2 light_dir = normalize(vec2(0.f, 1.f));
    color = object_color * clamp(abs(dot(light_dir, orientation.xz)),0.f,1.f);
    color += vec3(0.f, 0.1f, 0.f);

    //Create Triangle Strip
    float height = 0.f;
    int maxheight = 5;
    float half_width = 0.003f;
    for (int i = 0; i < maxheight; ++i)
    {
        //Compute curvature y = x^2 / 2
        float x = float(i) / float(maxheight);
        float y = (x * x) / 2.f;

        //Lighten color based on height
        color += vec3(0.05f, 0.07f, 0.f);

        vec4 mid = middle + vec4( 0.0, height, 0.0, 0.0);
        gl_Position = mid + normal * (half_width - half_width * x) + direction * y;
        gl_Position.xy = rot2D(gl_Position.xy, middle.xy, wind);
        gl_Position = MV * gl_Position;
        EmitVertex();

        gl_Position = mid + normal * -(half_width - half_width * x) + direction * y;
        gl_Position.xy = rot2D(gl_Position.xy, middle.xy, wind);

        gl_Position = MV * gl_Position;
        EmitVertex();

        height += 0.015f;
    }

    //Add last vertex
    gl_Position = middle + vec4( 0.f, height, 0.f, 0.f) + direction * 0.5f;
    gl_Position.xy = rot2D(gl_Position.xy, middle.xy, wind);
    gl_Position = MV * gl_Position;
    EmitVertex();
    EndPrimitive();
}  

