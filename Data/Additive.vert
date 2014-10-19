#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;

in vec4 in_position;
in vec2 in_uv;

out vec2 v2f_uv;

void main() {
    v2f_uv = in_uv;
    gl_Position = in_matrixPVM * in_position;
}
