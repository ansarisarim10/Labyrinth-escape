#include "Mesh.h"
#include <vector>
#include <glad/glad.h>

static Mesh makeVAO(const std::vector<float>& verts, const std::vector<unsigned>& idx) {
    Mesh m; m.indexed = !idx.empty(); m.count = m.indexed ? (int)idx.size() : (int)(verts.size() / 8);
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glBindVertexArray(m.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    if (m.indexed) {
        glGenBuffers(1, &m.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(unsigned), idx.data(), GL_STATIC_DRAW);
    }
    GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    return m;
}

void Mesh::draw() const {
    glBindVertexArray(vao);
    if (indexed) glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    else        glDrawArrays(GL_TRIANGLES, 0, count);
}

Mesh Mesh::Cube() {
    std::vector<float> v = {
        -0.5f,-0.5f,-0.5f, 0,0,-1, 0,0,  0.5f,-0.5f,-0.5f, 0,0,-1, 1,0,  0.5f, 0.5f,-0.5f, 0,0,-1, 1,1,
         0.5f, 0.5f,-0.5f, 0,0,-1, 1,1, -0.5f, 0.5f,-0.5f, 0,0,-1, 0,1, -0.5f,-0.5f,-0.5f, 0,0,-1, 0,0,
        -0.5f,-0.5f, 0.5f, 0,0, 1, 0,0,  0.5f,-0.5f, 0.5f, 0,0, 1, 1,0,  0.5f, 0.5f, 0.5f, 0,0, 1, 1,1,
         0.5f, 0.5f, 0.5f, 0,0, 1, 1,1, -0.5f, 0.5f, 0.5f, 0,0, 1, 0,1, -0.5f,-0.5f, 0.5f, 0,0, 1, 0,0,
        -0.5f, 0.5f, 0.5f,-1,0, 0, 1,0, -0.5f, 0.5f,-0.5f,-1,0, 0, 1,1, -0.5f,-0.5f,-0.5f,-1,0, 0, 0,1,
        -0.5f,-0.5f,-0.5f,-1,0, 0, 0,1, -0.5f,-0.5f, 0.5f,-1,0, 0, 0,0, -0.5f, 0.5f, 0.5f,-1,0, 0, 1,0,
         0.5f, 0.5f, 0.5f, 1,0, 0, 1,0,  0.5f, 0.5f,-0.5f, 1,0, 0, 1,1,  0.5f,-0.5f,-0.5f, 1,0, 0, 0,1,
         0.5f,-0.5f,-0.5f, 1,0, 0, 0,1,  0.5f,-0.5f, 0.5f, 1,0, 0, 0,0,  0.5f, 0.5f, 0.5f, 1,0, 0, 1,0,
        -0.5f,-0.5f,-0.5f, 0,-1,0, 0,1,  0.5f,-0.5f,-0.5f, 0,-1,0, 1,1,  0.5f,-0.5f, 0.5f, 0,-1,0, 1,0,
         0.5f,-0.5f, 0.5f, 0,-1,0, 1,0, -0.5f,-0.5f, 0.5f, 0,-1,0, 0,0, -0.5f,-0.5f,-0.5f, 0,-1,0, 0,1,
        -0.5f, 0.5f,-0.5f, 0, 1,0, 0,1,  0.5f, 0.5f,-0.5f, 0, 1,0, 1,1,  0.5f, 0.5f, 0.5f, 0, 1,0, 1,0,
         0.5f, 0.5f, 0.5f, 0, 1,0, 1,0, -0.5f, 0.5f, 0.5f, 0, 1,0, 0,0, -0.5f, 0.5f,-0.5f, 0, 1,0, 0,1,
    };
    return makeVAO(v, {});
}

Mesh Mesh::Plane() {
    std::vector<float> v = {
        -0.5f,0.f,-0.5f, 0,1,0, 0,0,
         0.5f,0.f,-0.5f, 0,1,0, 1,0,
         0.5f,0.f, 0.5f, 0,1,0, 1,1,
        -0.5f,0.f, 0.5f, 0,1,0, 0,1,
    };
    std::vector<unsigned> idx = { 0,1,2, 0,2,3 };
    return makeVAO(v, idx);
}
