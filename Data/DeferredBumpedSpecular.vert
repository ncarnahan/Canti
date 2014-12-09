#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;

in vec4 in_position;
in vec3 in_normal;
in vec4 in_tangent;
in vec2 in_uv;

out vec3 v2f_normal;
out vec3 v2f_tangent;
out vec3 v2f_bitangent;
out vec2 v2f_uv;

void main() {
    mat3x3 normalMatrix = mat3x3(in_matrixModel);
    v2f_normal = normalize(normalMatrix * in_normal);
    v2f_tangent = normalize(normalMatrix * in_tangent.xyz);
    v2f_bitangent = cross(v2f_normal, v2f_tangent) * in_tangent.w;
    
    v2f_uv = in_uv;
    gl_Position = in_matrixPVM * in_position;
}
