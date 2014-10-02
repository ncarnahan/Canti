#version 440

uniform vec3 in_eyePosition;
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
    int type;
};
uniform Light light;

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec4 out_color;

void main() {
    //Diffuse from texture
    vec3 texColor = texture(tex_diffuse, uv.st).rgb;


    //Lighting
    vec3 ambient = pow(light.ambient, vec3(2.2));
    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);

    if (light.type == 0) {
        vec3 lightDir = normalize(light.direction);
        vec3 normalDir = normalize(normal);
        float NdotL = max(dot(normalDir, lightDir), 0);
        diffuse = light.intensity * light.color * NdotL;

        vec3 eyeVec = normalize(in_eyePosition - position);
        vec3 halfVec = normalize(eyeVec + lightDir);
        float NdotH = max(dot(normalDir, halfVec), 0);
        specular = diffuse * pow(NdotH, 32);
    }
    else {
        vec3 lightDir = light.position - position;
        float lightDistance = length(lightDir);
        lightDir = lightDir / lightDistance;    //normalize

        //Attenuate to 0 at the radius
        float dr = lightDistance / light.radius;
        float denom = max(1 + dr, 0.001);
        float attenuation = max(1 / (denom * denom), 0);
        attenuation *= 1 - smoothstep(0, 1, dr);

        //Diffuse component
        vec3 normalDir = normalize(normal);
        float NdotL = max(dot(normalDir, lightDir), 0);
        diffuse = light.intensity * light.color * NdotL * attenuation;
        
        //Specular component
        vec3 eyeVec = normalize(in_eyePosition - position);
        vec3 halfVec = normalize(eyeVec + lightDir);
        float NdotH = max(dot(normalDir, halfVec), 0);
        specular = diffuse * pow(NdotH, 32);
    }

    vec3 lighting = diffuse + specular + ambient;


    //Gamma correction
    vec3 gamma = vec3(1.0 / 2.2);
    vec3 final = pow(texColor * lighting, gamma);

    out_color = vec4(final, 1);
}
