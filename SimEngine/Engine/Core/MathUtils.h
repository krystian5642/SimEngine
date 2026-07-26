#pragma once

struct Transform
{
    void SetCartesianPosition(const glm::vec3& newCartesianPosition)
    {
        cartesianPosition = newCartesianPosition;
        shouldUpdateModelMatrix = true;
    }
    const glm::vec3& GetCartesianPosition() const { return cartesianPosition; }
    
    void SetRotation(const glm::vec3& newRotation)
    {
        rotation = newRotation;
        shouldUpdateModelMatrix = true;
    }
    const glm::vec3& GetRotation() const { return rotation; }
    
    void SetScale(const glm::vec3& newScale)
    {
        scale = newScale;
        shouldUpdateModelMatrix = true;
    }
    const glm::vec3& GetScale() const{ return scale; }
    
    const glm::mat4& GetModelMatrix() const
    {
        if (shouldUpdateModelMatrix)
        {
            RefreshModelMatrix();
        }
        
        return cachedModelMatrix;
    }
    
private:
    void RefreshModelMatrix() const
    {
        cachedModelMatrix = glm::mat4(1.0f);

        cachedModelMatrix = glm::translate(cachedModelMatrix, cartesianPosition);
        
        cachedModelMatrix = glm::rotate(cachedModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        cachedModelMatrix = glm::rotate(cachedModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        cachedModelMatrix = glm::rotate(cachedModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

        cachedModelMatrix = glm::scale(cachedModelMatrix, scale);
        
        shouldUpdateModelMatrix = false;
    }
    
    glm::vec3 cartesianPosition{};
    glm::vec3 rotation{};
    glm::vec3 scale{1.0f};
    
    mutable glm::mat4 cachedModelMatrix{glm::mat4(1.0f)};
    mutable bool shouldUpdateModelMatrix{false};
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