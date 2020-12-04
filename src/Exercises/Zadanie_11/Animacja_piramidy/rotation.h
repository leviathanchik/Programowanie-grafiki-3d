//
// Created by ttrata on 18.11.2020.
//

#include "glm/glm.hpp"
#include <cmath>

inline glm::mat3 rotation(float angle, const glm::vec3 &axis) {
    auto const A = glm::normalize(axis);
    auto const s = std::sin(angle);
    auto const c = std::cos(angle);

    return glm::mat3(
            c + A.x * A.x * (1.0f - c),
            A.y * A.x * (1.0f - c) + A.z * s,
            A.z * A.x * (1.0f - c) - A.y * s,

            A.x * A.y * (1.0f - c) - A.z * s,
            c + A.y * A.y * (1.0f - c),
            A.z * A.y * (1.0f - c) + A.x * s,

            A.x * A.z * (1.0f - c) + A.y * s,
            A.y * A.z * (1.0f - c) - A.x * s,
            c + A.z * A.z * (1.0f - c)
    );
}     