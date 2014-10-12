#include "Program.h"
#include "Shader.h"
#include <iostream>

namespace Graphics
{
    std::string GetProgramLinkError(GLuint program)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        GLchar* buffer = new GLchar[length + 1];
        glGetProgramInfoLog(program, length, NULL, buffer);

        std::string output(buffer, length + 1);

        delete buffer;

        return output;
    }

    Program::Program()
    {
        program = 0;
    }

    Program::~Program()
    {
        if (program != 0)
        {
            glDeleteProgram(program);
        }
    }

    bool Program::Load(const Shader& vertShader, const Shader& fragShader)
    {
        return Load(&vertShader, nullptr, &fragShader);
    }

    bool Program::Load(const Shader& vertShader, const Shader& geomShader, const Shader& fragShader)
    {
        return Load(&vertShader, &geomShader, &fragShader);
    }
    
    bool Program::LoadFromFiles(const char* vertFile, const char* fragFile)
    {
        Shader vertShader, fragShader;

        vertShader.LoadFromFile(ShaderType::Vertex, vertFile);
        fragShader.LoadFromFile(ShaderType::Fragment, fragFile);

        return Load(vertShader, fragShader);
    }

    bool Program::LoadFromFiles(const char* vertFile, const char* geomFile, const char* fragFile)
    {
        Shader vertShader, geomShader, fragShader;

        vertShader.LoadFromFile(ShaderType::Vertex, vertFile);
        geomShader.LoadFromFile(ShaderType::Geometry, geomFile);
        fragShader.LoadFromFile(ShaderType::Fragment, fragFile);

        return Load(vertShader, geomShader, fragShader);
    }

    bool Program::Load(const Shader* vertShader, const Shader* geomShader, const Shader* fragShader)
    {
        //Create
        program = glCreateProgram();

        //Attach shaders to program
        if (vertShader != nullptr) { glAttachShader(program, vertShader->GetShader()); }
        if (geomShader != nullptr) { glAttachShader(program, geomShader->GetShader()); }
        if (fragShader != nullptr) { glAttachShader(program, fragShader->GetShader()); }

        //Bind the attributes to a standard location
        glBindAttribLocation(program, (GLuint)ProgramAttribute::Position, "in_position");
        glBindAttribLocation(program, (GLuint)ProgramAttribute::Normal, "in_normal");
        glBindAttribLocation(program, (GLuint)ProgramAttribute::Tangent, "in_tangent");
        glBindAttribLocation(program, (GLuint)ProgramAttribute::UV, "in_uv");

        //Link the program
        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            std::cout << "Program failed to link.\n";
            std::cout << GetProgramLinkError(program) << std::endl;
            return false;
        }

        return true;
    }

    void Program::Start()
    {
        glUseProgram(program);
    }

    void Program::Stop()
    {
        glUseProgram(0);
    }
}