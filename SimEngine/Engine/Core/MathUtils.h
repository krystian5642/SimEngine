#pragma once

#define DIRECTION static constexpr glm::vec3

struct Directions
{
    DIRECTION UP      = glm::vec3(0.0f, 1.0f, 0.0f);
    DIRECTION DOWN    = glm::vec3(0.0f, -1.0f, 0.0f);
    DIRECTION RIGHT   = glm::vec3(1.0f, 0.0f, 0.0f);
    DIRECTION LEFT    = glm::vec3(-1.0f, 0.0f, 0.0f);
    DIRECTION FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
    DIRECTION BACK    = glm::vec3(0.0f, 0.0f, 1.0f);
};

class MathUtils
{
public:
    static constexpr float EPS = 1e-4f;
    
    static inline const std::vector directions = {
        Directions::UP,
        Directions::DOWN,
        Directions::LEFT,
        Directions::RIGHT,
        Directions::FORWARD,
        Directions::BACK
    };
    
    template <class T>
    static T RandomNum(T min, T max);
    
    static bool IsNearlyZeroVector(const glm::vec3& vec, float epsilon = EPS)
    {
        return glm::all(glm::lessThan(glm::abs(vec), glm::vec3(epsilon)));
    }
};

template <class T>
T MathUtils::RandomNum(T min, T max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution dist(min, max);
    return dist(gen);
}
