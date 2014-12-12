#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;
uniform sampler2D tex_diffuse;

struct Material
{
    vec3 specularColor;
    float specularExponent;
};
uniform Material material;

in vec3 v2f_normal;
in vec2 v2f_uv;

out vec3 out_color;
out vec4 out_specular;
out vec3 out_normal;

void main() {
    //Diffuse from texture
    vec3 texColor = texture(tex_diffuse, v2f_uv.st).rgb;
    out_color = texColor;
    out_specular = vec4(material.specularColor, material.specularExponent / 255);
    out_normal = normalize(v2f_normal);
}
