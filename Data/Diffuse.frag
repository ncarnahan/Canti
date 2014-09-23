#version 440

uniform sampler2D tex_diffuse;

in vec3 normal;
in vec2 uv;

out vec4 out_color;

void main() {
    //Diffuse from texture
    out_color = texture(tex_diffuse, uv.st);

    //Lighting
    vec4 l = normalize(vec4(1, 1, 1, 0));
    vec4 n = normalize(vec4(normal, 0));
    out_color *= max(dot(n, l), 0);

    //Gamma correction
    vec4 gamma = vec4(1.0 / 2.2);
    gamma.w = 1.0;
    out_color = pow(out_color, gamma);

    //Ambient light
    out_color += vec4(0.05, 0.05, 0.05, 0);
}
