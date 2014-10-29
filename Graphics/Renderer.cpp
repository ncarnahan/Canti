#include "Renderer.h"
#include "Material.h"
#include "Light.h"

namespace Graphics
{
    void UseLight(Program* program, Light* light)
    {
        //Light uniforms
        glUniform1i(program->GetUniformLocation("light.type"), (int)light->type);

        glUniform3fv(program->GetUniformLocation("light.position"), 1, &light->position[0]);
        glUniform3fv(program->GetUniformLocation("light.direction"), 1, &light->direction[0]);
        glUniform3fv(program->GetUniformLocation("light.color"), 1, &light->color[0]);
            
        glUniform1f(program->GetUniformLocation("light.intensity"), light->intensity);
        glUniform1f(program->GetUniformLocation("light.radius"), light->radius);
        glUniform1f(program->GetUniformLocation("light.cosAngle"), Math::CosDeg(light->angle));
        glUniform1f(program->GetUniformLocation("light.innerPercent"), light->innerPercent);
    }



    Renderer::Renderer()
    {
        
    }

    void Renderer::Submit(DrawCall& drawCall)
    {
        _drawCalls.push_back(drawCall);
    }

    void Renderer::Draw()
    {
        Material* lastMaterial = nullptr;

        Matrix4x4 pvMatrix = _projectionMatrix * _viewMatrix;
        Matrix4x4 pvmMatrix;

        for (auto& drawCall : _drawCalls)
        {
            //Use the material
            if (drawCall.material != lastMaterial)
            {
                drawCall.material->Start();
                lastMaterial = drawCall.material;
            }


            auto program = drawCall.material->GetProgram();

            //Setup matrices
            pvmMatrix = pvMatrix * drawCall.modelMatrix;
            //TEMP: Use UBO
            glUniformMatrix4fv(program->GetUniformLocation("in_matrixProj"), 1, false, &_projectionMatrix[0]);
            glUniformMatrix4fv(program->GetUniformLocation("in_matrixView"), 1, false, &_viewMatrix[0]);
            glUniformMatrix4fv(program->GetUniformLocation("in_matrixModel"), 1, false, &drawCall.modelMatrix[0]);
            glUniformMatrix4fv(program->GetUniformLocation("in_matrixPVM"), 1, false, &pvmMatrix[0]);

            glUniform3fv(program->GetUniformLocation("in_eyePosition"), 1, &_eyePosition[0]);
            glUniform3fv(program->GetUniformLocation("light.ambient"), 1, &drawCall.ambientLight[0]);


            //Apply the light
            if (drawCall.material->IsLit())
            {
                UseLight(drawCall.material->GetProgram(), drawCall.light);
            }


            //Setup blending per pass
            drawCall.material->Pass(drawCall.pass);


            glBindVertexArray(drawCall.vao);
            glDrawElements(GL_TRIANGLES, drawCall.size, GL_UNSIGNED_INT, 0);
        }

        _drawCalls.clear();
    }
}
