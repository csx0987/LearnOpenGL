#include "shader.h"
#include "gFunction.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // 文件读取shader
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (const std::ifstream::failure &e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << '\n';
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    // 顶点
    GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
    GLCall(glCompileShader(vertex));
    checkCompileErrors(vertex, "VERTEX");

    // 片段
    GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
    GLCall(glCompileShader(fragment));
    checkCompileErrors(fragment, "FRAGMENT");

    // program
    GLCall(ID = glCreateProgram());
    GLCall(glAttachShader(ID, vertex));
    GLCall(glAttachShader(ID, fragment));
    GLCall(glLinkProgram(ID));
    checkCompileErrors(ID, "PROGRAM");

    GLCall(glDeleteShader(vertex));
    GLCall(glDeleteShader(fragment));
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(ID));
}

void Shader::use() const
{
    GLCall(glUseProgram(ID));
}

void Shader::setBool(const std::string &name, bool value) const
{
    GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value));
}

void Shader::setInt(const std::string &name, int value) const
{
    GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), value));
}

void Shader::setFloat(const std::string &name, float value) const
{
    GLCall(glUniform1f(glGetUniformLocation(ID, name.c_str()), value));
}

void Shader::setVec3(const std::string &name, float v1, float v2, float v3) const
{
    GLCall(glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{ 
    GLCall(glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
}

void Shader::setVec4(const std::string &name, float v1, float v2, float v3, float v4) const
{
    GLCall(glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4));
}

void Shader::setMat4(const std::string &name, glm::mat4 &trans) const
{
    GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans)));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            GLCall(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLCall(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}