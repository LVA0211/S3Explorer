#pragma once

#include "Object.h"  // so it knows what Object is
#include <functional>

// Typedef to simplify usage
using AnimationFunction = std::function<void(Object&, float)>;

// Reusable animation functions
AnimationFunction rotateInPlane(float speed, glm::vec4 u, glm::vec4 v);
AnimationFunction bounce(float amplitude = 1.0f, float frequency = 1.0f);
AnimationFunction idle(); // empty animation (no-op)
AnimationFunction spinObject();

