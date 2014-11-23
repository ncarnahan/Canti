#version 440

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;
uniform sampler2D tex_diffuse;
uniform sampler2D tex_normal;
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
in vec3 v2f_tangent;
in vec3 v2f_bitangent;
in vec2 v2f_uv;

out vec4 out_color;

void main() {
    //Diffuse from texture
    vec3 texColor = texture(tex_diffuse, v2f_uv.st).rgb;
    vec3 texNormal = texture(tex_normal, v2f_uv.st).rgb;
    texNormal.r = 2.0 * texNormal.r - 1.0;    //Convert from [0, 1] to [-1, 1]
    texNormal.g = 2.0 * texNormal.g - 1.0;
    //texNormal.b = 2.0 * texNormal.b - 1.0;  //Blender blue channel is [0, 1] already
    texNormal = normalize(texNormal);

    vec3 normal = normalize(v2f_normal);
    vec3 tangent = normalize(v2f_tangent);
    vec3 bitangent = normalize(v2f_bitangent);

    //Lighting
    vec3 lightDirWS;
    float lightDistance;
    float attenuation = 1;

    if (light.type == 0) {
        lightDirWS = normalize(-light.direction);
    }
    else {
        lightDirWS = light.position - v2f_position;
        lightDistance = length(lightDirWS);
        lightDirWS = lightDirWS / lightDistance;    //normalize
    }

    //Convert light direction to tangent space
    vec3 lightDirTS = vec3(
        dot(lightDirWS, tangent),
        dot(lightDirWS, bitangent),
        dot(lightDirWS, normal)
    );

    //Light components
    vec3 ambient = pow(light.ambient, vec3(2.2));
    vec3 diffuse = vec3(0);
    float NdotL = dot(texNormal, lightDirTS);

    if (NdotL > 0) {
        //Calculate attenuation
        if (light.type > 0) {
            //Attenuate to 0 at the radius
            float dr = lightDistance / light.radius;
            float denom = max(1 + dr, 0.001);
            attenuation = max(1 / (denom * denom), 0);
            attenuation *= 1 - smoothstep(0, 1, dr);

            if (light.type == 2) {
                float multiplier = 0;
                float spotEffect = dot(lightDirWS, -light.direction);

                if (spotEffect > light.cosAngle) {
                    float zeroToOne = max((spotEffect - light.cosAngle) / (1 - light.cosAngle), 0);
                    multiplier = smoothstep(0, 1, zeroToOne / max(1 - light.innerPercent, 0.001));
                }

                attenuation *= multiplier;
            }
        }

        //Compute diffuse
        diffuse = light.intensity * light.color * NdotL * attenuation;
    }

    //Combine light components
    vec3 lighting = diffuse + ambient;

    out_color = vec4(texColor * lighting, 1);
}
