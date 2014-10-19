#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;
uniform sampler2D tex_diffuse;

in vec2 v2f_uv;

out vec4 out_color;

void main() {
    out_color = texture(tex_diffuse, v2f_uv.st);
    
    //Gamma correction
    vec3 gamma = vec3(1.0 / 2.2);
    out_color.rgb = pow(out_color.rgb, gamma);
}
