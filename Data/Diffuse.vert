#version 440

in vec4 in_position;
in vec4 in_normal;
out vec4 normal;

void main() {
    normal = in_normal;
    gl_Position = in_position;
}