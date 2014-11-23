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
struct Shadow
{
    mat4 matrixPV;
    float bias;
    float strength;
    sampler2D texture;
    //samplerCubeShadow cubeMap;
};
uniform Shadow shadow;

in vec3 v2f_position;
in vec3 v2f_shadowPosition;
in vec3 v2f_normal;
in vec2 v2f_uv;

out vec4 out_color;

void main() {
    //Diffuse from texture
    vec3 texColor = texture(tex_diffuse, v2f_uv.st).rgb;
    float texAlpha = texture(tex_diffuse, v2f_uv.st).a;


    //Lighting
    vec3 normalDir = normalize(v2f_normal);
    vec3 lightDir;
    float lightDistance;
    float attenuation = 1;

    if (light.type == 0) {
        lightDir = normalize(light.direction);
    }
    else {
        lightDir = light.position - v2f_position;
        lightDistance = length(lightDir);
        lightDir = lightDir / lightDistance;    //normalize
    }

    //Light components
    vec3 ambient = pow(light.ambient, vec3(2.2));
    vec3 diffuse = vec3(0);
    float NdotL = dot(normalDir, lightDir);

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
                float spotEffect = dot(lightDir, -light.direction);

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

    float occluderDepth = texture(shadow.texture, v2f_shadowPosition.xy).r;
    float shadowStrength = 0;
    if (occluderDepth < v2f_shadowPosition.z - 0.002) {
        float l = smoothstep(0.9, 1.0, length(2 * v2f_shadowPosition.xy - vec2(1)));
        shadowStrength = (1 - l) * 0.5;
    }

    //Combine light components
    vec3 lighting = diffuse * (1 - shadowStrength) + ambient;

    out_color = vec4(texColor * lighting, texAlpha);
}
