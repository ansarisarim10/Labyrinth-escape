#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    // state (same as demos)
    glm::vec3 pos{ 0.0f, 0.0f, 3.0f };
    glm::vec3 front{ 0.0f, 0.0f,-1.0f };
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };

    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 60.0f;   // degrees

    // constructors
    Camera() = default;
    explicit Camera(const glm::vec3& startPos) : pos(startPos) {}

    // primary API (new)
    glm::mat4 view() const;
    glm::mat4 proj(float aspect) const;
    void lookDelta(float dx, float dy);
    void moveWSAD(bool w, bool s, bool a, bool d, bool upKey, bool downKey, float dt);

    // --- compatibility shims (so old main.cpp compiles unchanged) ---
    glm::mat4 getView() const { return view(); }                 // old name used in your main.cpp
    void moveWSAD(int dir, float dt);                            // old 2-arg version
};
