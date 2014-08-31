#version 440

uniform sampler2D tex_diffuse;

in vec4 normal;
in vec4 uv;

void main() {
    gl_FragColor = texture(tex_diffuse, uv.st);
}