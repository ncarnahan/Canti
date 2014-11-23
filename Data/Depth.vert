#version 440

uniform mat4 in_matrixPVM;

in vec4 in_position;

void main() {
    gl_Position = in_matrixPVM * in_position;
}
