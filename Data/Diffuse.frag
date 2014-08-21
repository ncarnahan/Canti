#version 440

uniform sampler2D tex_diffuse;
uniform sampler2D tex_lightmap;

in vec4 normal;
in vec4 uv;

void main() {
    float lightmap = texture(tex_lightmap, uv.st).r;
    gl_FragColor = texture(tex_diffuse, uv.st) * vec4(vec3(lightmap), 1);
    //gl_FragColor = vec4(lightmap, lightmap, lightmap, 1);
}