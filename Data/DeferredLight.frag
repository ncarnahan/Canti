#version 440

uniform vec3 in_eyePosition;
uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;
uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_depth;
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

in vec2 v2f_uv;
in mat4 v2f_matrixInvPV;

out vec3 out_color;

vec3 reconstructWorldPosition() {
    vec4 positionCS;
    positionCS.xy = v2f_uv * 2.0 - 1.0;
    positionCS.z = texture(tex_depth, v2f_uv).r * 2.0 - 1.0;
    positionCS.w = 1.0;
    vec4 positionWS = v2f_matrixInvPV * positionCS;
    return positionWS.xyz / positionWS.w;
}

void main() {
    vec3 texColor = texture(tex_color, v2f_uv).rgb;
    vec3 normalDir = texture(tex_normal, v2f_uv).rgb * 2 - vec3(1);

    vec3 lightDir;
    float lightDistance;
    float attenuation = 1;

    if (light.type == 0) {
        lightDir = normalize(-light.direction);
    }
    else {
        lightDir = light.position - reconstructWorldPosition();
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

    //Combine light components
    vec3 lighting = diffuse + ambient;

    out_color = texColor * lighting;
    //out_color = /* abs */(reconstructWorldPosition());
    // out_color = abs(texture(tex_normal, v2f_uv).rgb * 2 - vec3(1));
}