#include "Animations.h"

AnimationFunction rotateInPlane(float speed, glm::vec4 u, glm::vec4 v) {

    const glm::vec4 uNorm = glm::normalize(u);
    const glm::vec4 vNorm = glm::normalize(v - glm::dot(v, uNorm) * uNorm);

    const glm::mat4 A = glm::outerProduct(u, u) + glm::outerProduct(v, v);
    const glm::mat4 B = glm::outerProduct(v, u) - glm::outerProduct(u, v);

    return [speed, A, B](Object& obj, float time) {
        float angle = speed * time;
        obj.transform = glm::mat4(1.0f)
            + (glm::cos(angle) - 1.0f) * A
            + glm::sin(angle) * B;
    };
}