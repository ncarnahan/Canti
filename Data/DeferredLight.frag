#version 440

uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_depth;

in vec2 v2f_uv;

out vec3 out_color;

void main() {
    out_color = texture(tex_color, v2f_uv).rgb;
    // out_color = abs(texture(tex_normal, v2f_uv).rgb * 2 - vec3(1));
}