#pragma once
struct Mesh {
    unsigned int vao = 0, vbo = 0, ebo = 0;
    int  count = 0;
    bool indexed = false;
    void draw() const;
    static Mesh Cube();
    static Mesh Plane();
};
