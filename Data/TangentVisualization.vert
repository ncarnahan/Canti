#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;

in vec4 in_position;
in vec3 in_normal;
in vec3 in_tangent;

out vec3 v2g_position;
out vec3 v2g_normal;
out vec3 v2g_tangent;

void main() {
    mat3x3 normalMatrix = mat3x3(in_matrixModel);
    v2g_normal = normalize(normalMatrix * in_normal);
    v2g_tangent = normalize(normalMatrix * in_tangent);
    v2g_position = (in_matrixModel * in_position).xyz;
    gl_Position = in_matrixPVM * in_position;
}
