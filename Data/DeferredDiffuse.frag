#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;
uniform sampler2D tex_diffuse;

in vec3 v2f_position;
in vec3 v2f_normal;
in vec2 v2f_uv;

out vec3 out_color;
out vec4 out_specular;
out vec3 out_normal;

void main() {
    //Diffuse from texture
    vec3 texColor = texture(tex_diffuse, v2f_uv.st).rgb;
    out_color = texColor;
    out_specular = vec4(0);
    out_normal = vec3(normalize(v2f_normal)) / 2 + vec3(0.5);
}
