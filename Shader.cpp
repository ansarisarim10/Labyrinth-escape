#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>

static std::string loadFile(const std::string& p) {
    std::ifstream f(p, std::ios::binary);
    if (!f) {
        std::string m = "Cannot open shader file: " + p;
        MessageBoxA(nullptr, m.c_str(), "Shader Load Error", MB_ICONERROR);
        return {};
    }
    std::ostringstream ss; ss << f.rdbuf();
    return ss.str();
}

static GLuint compile(GLenum type, const std::string& src, const char* name) {
    GLuint s = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(s, 1, &c, nullptr);
    glCompileShader(s);
    GLint ok = 0; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0; glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0'); glGetShaderInfoLog(s, len, nullptr, log.data());
        std::string m = std::string("Compile error in ") + name + ":\n" + log;
        MessageBoxA(nullptr, m.c_str(), "Shader Compile Error", MB_ICONERROR);
    }
    return s;
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath) {
    std::string vsrc = loadFile(vsPath);
    std::string fsrc = loadFile(fsPath);
    GLuint v = compile(GL_VERTEX_SHADER, vsrc, vsPath.c_str());
    GLuint f = compile(GL_FRAGMENT_SHADER, fsrc, fsPath.c_str());

    id = glCreateProgram();
    glAttachShader(id, v);
    glAttachShader(id, f);
    glLinkProgram(id);

    GLint ok = 0; glGetProgramiv(id, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len = 0; glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0'); glGetProgramInfoLog(id, len, nullptr, log.data());
        MessageBoxA(nullptr, log.c_str(), "Program Link Error", MB_ICONERROR);
    }

    glDeleteShader(v);
    glDeleteShader(f);
}

Shader::~Shader() {
    if (id) glDeleteProgram(id);
}

void Shader::use() const {
    glUseProgram(id);
}

int Shader::uniformLoc(const char* name) const {
    return glGetUniformLocation(id, name);
}

void Shader::setMat4(const char* name, const glm::mat4& m) const {
    glUniformMatrix4fv(uniformLoc(name), 1, GL_FALSE, &const_cast<glm::mat4&>(m)[0][0]);
}

void Shader::setVec3(const char* name, const glm::vec3& v) const {
    glUniform3fv(uniformLoc(name), 1, &v[0]);
}

void Shader::setFloat(const char* name, float v) const {
    glUniform1f(uniformLoc(name), v);
}

void Shader::setInt(const char* name, int v) const {
    glUniform1i(uniformLoc(name), v);
}
