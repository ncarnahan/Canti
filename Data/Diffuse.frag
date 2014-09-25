#version 440

uniform sampler2D tex_diffuse;
struct DirLight
{
    float intensity;
    vec3 direction;
    vec3 color;
};
uniform DirLight light;

in vec3 normal;
in vec2 uv;

out vec4 out_color;

void main() {
    //Diffuse from texture
    vec3 diffuse = texture(tex_diffuse, uv.st).rgb;

    //Lighting
    vec3 ambient = vec3(0.05);
    ambient = pow(ambient, vec3(2.2));

    vec3 lightDir = normalize(light.direction);
    vec3 normalDir = normalize(normal);
    float dotProd = dot(normalDir, lightDir);
    vec3 lighting = max(light.intensity * light.color * dotProd, 0) + ambient;

    //Gamma correction
    vec3 gamma = vec3(1.0 / 2.2);
    vec3 final = pow(diffuse * lighting, gamma);

    out_color = vec4(final, 0);
}
