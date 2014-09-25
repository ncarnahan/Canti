#version 440

uniform mat4 in_modelview;

in vec4 in_position;
in vec3 in_normal;
in vec2 in_uv;

out vec3 normal;
out vec2 uv;

void main() {
    normal = in_normal;
    uv = in_uv;
    gl_Position = in_modelview * in_position;
}
