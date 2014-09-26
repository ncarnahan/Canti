#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;

in vec4 in_position;
in vec3 in_normal;
in vec2 in_uv;

out vec3 position;
out vec3 normal;
out vec2 uv;

void main() {
    position = (in_matrixModel * in_position).xyz;

    mat3x3 normalMatrix = mat3x3(in_matrixModel);
    normal = normalize(normalMatrix * in_normal);
    uv = in_uv;
    gl_Position = in_matrixPVM * in_position;
}
