#version 440

in vec4 normal;

void main() {
    gl_FragColor = normal / 2 + vec4(0.5, 0.5, 0.5, 1);
}