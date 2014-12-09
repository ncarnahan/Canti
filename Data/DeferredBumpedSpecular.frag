#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;
uniform sampler2D tex_diffuse;
uniform sampler2D tex_normal;

in vec3 v2f_normal;
in vec3 v2f_tangent;
in vec3 v2f_bitangent;
in vec2 v2f_uv;

out vec4 out_color;
out vec3 out_normal;

void main() {
    //Diffuse from texture
    vec3 texColor = texture(tex_diffuse, v2f_uv.st).rgb;
    vec3 normalTS = texture(tex_normal, v2f_uv.st).rgb;
    normalTS.rg = 2.0 * normalTS.rg - 1.0;    //Convert from [0, 1] to [-1, 1]
    normalTS = normalize(normalTS);

    mat3 matrixInvTS = mat3(
        normalize(v2f_tangent),
        normalize(v2f_bitangent),
        normalize(v2f_normal));

    out_color = vec4(texColor, material.specularExponent / 255);
    out_normal = vec3(normalize(matrixInvTS * normalTS)) / 2 + vec3(0.5);
}
