#pragma once

#define DIRECTION static constexpr glm::vec3

struct Transform
{
    glm::vec3 position{};
    glm::vec3 rotation{};
    glm::vec3 scale{1.0f};
    
    glm::mat4 CalculateModelMatrix() const
    {
        auto modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, position);
        
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

        modelMatrix = glm::scale(modelMatrix, scale);
        
        return modelMatrix;
    }
};

class MathUtils
{
public:
    static constexpr float EPS = 1e-4f;
    
    template <class T>
    static T RandomNum(T min, T max);
    
    static bool IsNearlyZeroVector(const glm::vec3& vec, float epsilon = EPS)
    {
        return glm::all(glm::lessThan(glm::abs(vec), glm::vec3(epsilon)));
    }
    
    static glm::vec3 RandomScalarVec3(float min, float max)
    {
        const auto value = MathUtils::RandomNum(min, max);
        return glm::vec3{value, value, value};
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