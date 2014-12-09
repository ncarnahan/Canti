#version 440

in vec4 in_position;
in vec2 in_uv;

out vec2 v2f_uv;

void main() {
    v2f_uv = in_uv;
    gl_Position = in_position;
}