#version 440

uniform sampler2D tex_diffuse;

in vec4 normal;
in vec4 uv;

out vec4 out_color;

void main() {
    out_color = texture(tex_diffuse, uv.st);
}