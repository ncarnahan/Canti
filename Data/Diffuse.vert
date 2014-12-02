#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;
struct Shadow
{
    mat4 matrixPV;
    float bias;
    float strength;
    sampler2D texture;
};
uniform Shadow shadow;

in vec4 in_position;
in vec3 in_normal;
in vec2 in_uv;

out vec3 v2f_position;
out vec4 v2f_shadowPosition;
out vec3 v2f_normal;
out vec2 v2f_uv;

void main() {
    vec4 worldPosition = (in_matrixModel * in_position);
    v2f_position = worldPosition.xyz;
    v2f_shadowPosition = shadow.matrixPV * worldPosition;

    mat3x3 normalMatrix = mat3x3(in_matrixModel);
    v2f_normal = normalize(normalMatrix * in_normal);
    v2f_uv = in_uv;
    gl_Position = in_matrixPVM * in_position;
}
