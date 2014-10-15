#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;
uniform sampler2D tex_diffuse;
struct Light
{
    vec3 ambient;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float radius;
    float cosAngle;
    float innerPercent;
    int type;
};
uniform Light light;

in vec3 v2f_position;
in vec3 v2f_normal;
in vec2 v2f_uv;

out vec4 out_color;

void main() {
    //Diffuse from texture
    vec3 texColor = texture(tex_diffuse, v2f_uv.st).rgb;


    //Lighting
    vec3 ambient = pow(light.ambient, vec3(2.2));
    vec3 diffuse = vec3(0);

    if (light.type == 0) {
        vec3 lightDir = normalize(light.direction);
        vec3 normalDir = normalize(v2f_normal);
        float NdotL = max(dot(normalDir, lightDir), 0);
        diffuse = light.intensity * light.color * NdotL;
    }
    else if (light.type == 1) {
        vec3 lightDir = light.position - v2f_position;
        float lightDistance = length(lightDir);
        lightDir = lightDir / lightDistance;    //normalize

        //Attenuate to 0 at the radius
        float dr = lightDistance / light.radius;
        float denom = max(1 + dr, 0.001);
        float attenuation = max(1 / (denom * denom), 0);
        attenuation *= 1 - smoothstep(0, 1, dr);

        //Diffuse component
        vec3 normalDir = normalize(v2f_normal);
        float NdotL = max(dot(normalDir, lightDir), 0);
        diffuse = light.intensity * light.color * NdotL * attenuation;
    }
    else {
        vec3 lightDir = light.position - v2f_position;
        float lightDistance = length(lightDir);
        lightDir = lightDir / lightDistance;    //normalize

        //Attenuate to 0 at the radius
        float dr = lightDistance / light.radius;
        float denom = max(1 + dr, 0.001);
        float attenuation = max(1 / (denom * denom), 0);
        attenuation *= 1 - smoothstep(0, 1, dr);

        float spotEffect = dot(lightDir, -light.direction);
        if (spotEffect > light.cosAngle) {
            float zeroToOne = max((spotEffect - light.cosAngle) / (1 - light.cosAngle), 0);
            attenuation *= smoothstep(0, 1, zeroToOne / max(1 - light.innerPercent, 0.001));
        }
        else {
            attenuation = 0;
        }

        //Diffuse component
        vec3 normalDir = normalize(v2f_normal);
        float NdotL = max(dot(normalDir, lightDir), 0);
        diffuse = light.intensity * light.color * NdotL * attenuation;
    }

    vec3 lighting = diffuse + ambient;


    //Gamma correction
    vec3 gamma = vec3(1.0 / 2.2);
    vec3 final = pow(texColor * lighting, gamma);

    out_color = vec4(final, 1);
}
