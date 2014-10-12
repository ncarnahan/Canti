#version 440

in vec3 g2f_color;
out vec4 out_color;

void main() {
    out_color = vec4(g2f_color, 1);
}
