#include "Renderer.h"
#include "Material.h"
#include "Light.h"
#include "ShadowMap.h"
#include <algorithm>

namespace Graphics
{
    void UseLight(Program* program, Light* light)
    {
        //Light uniforms
        glUniform1i(program->GetUniformLocation("light.type"), (int)light->type);

        glUniform3fv(program->GetUniformLocation("light.position"), 1, &light->position[0]);
        Vector3 direction = light->rotation * Vector3::forward;
        glUniform3fv(program->GetUniformLocation("light.direction"), 1, &direction[0]);
        glUniform3fv(program->GetUniformLocation("light.color"), 1, &light->color[0]);
            
        glUniform1f(program->GetUniformLocation("light.intensity"), light->intensity);
        glUniform1f(program->GetUniformLocation("light.radius"), light->radius);
        glUniform1f(program->GetUniformLocation("light.cosAngle"), Math::CosDeg(light->angle / 2));
        glUniform1f(program->GetUniformLocation("light.innerPercent"), light->innerPercent);
    }

    void UseShadowMap(Program* program, ShadowMap* shadowMap)
    {
        if (shadowMap == nullptr)
        {
            //No shadows; set strength to 0
            glUniform1f(program->GetUniformLocation("shadow.strength"), 0);
        }
        else
        {
            //Shadow map uniforms
            glUniform1f(program->GetUniformLocation("shadow.bias"), shadowMap->bias);
            glUniform1f(program->GetUniformLocation("shadow.strength"), shadowMap->strength);

            const Matrix4x4 biasMatrix = Matrix4x4(
                0.5f, 0, 0, 0.5f,
                0, 0.5f, 0, 0.5f,
                0, 0, 0.5f, 0.5f,
                0, 0, 0, 1);
            Matrix4x4 matrixPV = biasMatrix * shadowMap->projMatrix * shadowMap->viewMatrix;
            glUniformMatrix4fv(program->GetUniformLocation("shadow.matrixPV"), 1, GL_FALSE, &matrixPV[0]);

            //Bind shadow map
            glUniform1i(program->GetUniformLocation("shadow.texture"), 7);
            glActiveTexture(GL_TEXTURE7);
            shadowMap->framebuffer.GetTexture()->Bind();
        }
    }



    Renderer::Renderer() :
        _lastMaterialId(0),
        sortEnabled(true),
        ignoreCount(0)
    {
    }

    SortKey Renderer::CreateSortKey(float depth, Material* material, uint8_t pass)
    {
        uint32_t materialId;

        auto iter = _materialMap.find(material);
        if (iter == _materialMap.end())
        {
            materialId = _lastMaterialId++;

            //Add to the hashmap for later
            _materialMap.insert(
                std::pair<Material*, uint32_t>(material, materialId));
        }
        else
        {
            //It already exists, use the existing ID
            materialId = iter->second;
        }

        return SortKey(material->blendType, depth, materialId, pass);
    }

    void Renderer::Submit(SortKey key, DrawCall& drawCall)
    {
        DrawCallPair pair;
        pair.key = key;
        pair.drawCall = drawCall;
        _drawCalls.push_back(pair);
    }

    void Renderer::Draw()
    {
        if (sortEnabled)
        {
            std::sort(_drawCalls.begin(), _drawCalls.end());
        }

        Material* lastMaterial = nullptr;

        Matrix4x4 pvMatrix = _projectionMatrix * _viewMatrix;
        Matrix4x4 pvmMatrix;

        for (int i = 0; i < (int)_drawCalls.size() - ignoreCount; i++)
        {
            auto& drawCall = _drawCalls[i].drawCall;

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
            if (drawCall.material->useLighting)
            {
                UseLight(drawCall.material->GetProgram(), drawCall.light);
                UseShadowMap(drawCall.material->GetProgram(), drawCall.light->shadowMap);
            }


            //Setup blending per pass
            drawCall.material->Pass(drawCall.pass);


            glBindVertexArray(drawCall.vao);
            glDrawElements(drawCall.drawMode, drawCall.size, GL_UNSIGNED_INT, 0);
        }

        _drawCalls.clear();
    }
}
