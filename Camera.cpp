#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

static inline glm::vec3 rightFrom(const glm::vec3& front) {
    return glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::mat4 Camera::view() const {
    return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::proj(float aspect) const {
    return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
}

void Camera::lookDelta(float dx, float dy) {
    const float sens = 0.1f;
    yaw += dx * sens;
    pitch += dy * sens;
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);

    // keep 'up' stable using world-up
    up = glm::normalize(glm::cross(rightFrom(front), front));
}

void Camera::moveWSAD(bool w, bool s, bool a, bool d, bool upKey, bool downKey, float dt) {
    const float speed = 5.0f * dt;
    const glm::vec3 right = rightFrom(front);
    const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    if (w)       pos += speed * front;
    if (s)       pos -= speed * front;
    if (a)       pos -= speed * right;
    if (d)       pos += speed * right;
    if (upKey)   pos += speed * worldUp;
    if (downKey) pos -= speed * worldUp;
}

// --- compatibility: old 2-arg moveWSAD(dir, dt) ---
void Camera::moveWSAD(int dir, float dt) {
    bool w = false, s = false, a = false, d = false, upKey = false, downKey = false;
    switch (dir) {
    case 0: w = true; break;        // FORWARD
    case 1: s = true; break;        // BACKWARD
    case 2: a = true; break;        // LEFT
    case 3: d = true; break;        // RIGHT
    default: break;
    }
    moveWSAD(w, s, a, d, upKey, downKey, dt);
}
