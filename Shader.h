#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int id = 0;

    Shader() = default;
    Shader(const std::string& vsPath, const std::string& fsPath);
    ~Shader();

    void use() const;
    int  uniformLoc(const char* name) const;

    void setMat4(const char* name, const glm::mat4& m) const;
    void setVec3(const char* name, const glm::vec3& v) const;
    void setFloat(const char* name, float v) const;
    void setInt(const char* name, int   v) const;
};
