#version 440

layout(points) in;
layout(line_strip, max_vertices = 8) out;

uniform mat4 in_matrixPVM;
uniform mat4 in_matrixModel;
uniform mat4 in_matrixView;
uniform mat4 in_matrixProj;

in vec3 v2g_position[];
in vec3 v2g_normal[];
in vec3 v2g_tangent[];
out vec3 g2f_color;

void main() {
    mat4 matrixPV = in_matrixProj * in_matrixView;
    float vectorLength = 0.1f;

    for (int i = 0; i < gl_in.length(); ++i) {
        vec3 position = v2g_position[i];
        vec3 normal = v2g_normal[i];
        vec3 tangent = v2g_tangent[i];
        vec3 bitangent = cross(normal, tangent);

        normal *= vectorLength;
        tangent *= vectorLength;
        bitangent *= vectorLength;

        vec3 normalOffset = normal * 0.1f;
        vec4 basePosition = matrixPV * vec4(position + normalOffset, 1);

        g2f_color = vec3(1, 0, 0);
        gl_Position = basePosition;
        EmitVertex();
        gl_Position = matrixPV * vec4(position + normalOffset + tangent, 1);
        EmitVertex();
        gl_Position = basePosition;
        EmitVertex();

        g2f_color = vec3(0, 1, 0);
        gl_Position = basePosition;
        EmitVertex();
        gl_Position = matrixPV * vec4(position + normalOffset + bitangent, 1);
        EmitVertex();
        gl_Position = basePosition;
        EmitVertex();

        g2f_color = vec3(0, 0, 1);
        gl_Position = basePosition;
        EmitVertex();
        gl_Position = matrixPV * vec4(position + normalOffset + normal, 1);
        EmitVertex();
    }

    EndPrimitive();
}
