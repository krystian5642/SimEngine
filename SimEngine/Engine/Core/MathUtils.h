#pragma once

struct Transform
{
    void SetPosition(const glm::vec3& newPosition)
    {
        position = newPosition;
        shouldUpdateModelMatrix = true;
    }
    const glm::vec3& GetPosition() const { return position; }
    
    void SetOrientation(const glm::quat& newOrientation)
    {
        orientation = newOrientation;
        shouldUpdateModelMatrix = true;
    }
    const glm::quat& GetOrientation() const { return orientation; }
    
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

        cachedModelMatrix = glm::translate(cachedModelMatrix, position);
        cachedModelMatrix *= glm::mat4_cast(orientation);
        cachedModelMatrix = glm::scale(cachedModelMatrix, scale);
        
        shouldUpdateModelMatrix = false;
    }
    
    glm::vec3 position{};
    glm::quat orientation = glm::identity<glm::quat>();
    glm::vec3 scale{1.0f};
    
    mutable glm::mat4 cachedModelMatrix{glm::mat4(1.0f)};
    mutable bool shouldUpdateModelMatrix{false};
};

class MathUtils
{
public:
    static constexpr float EPS = 1e-6f;
    
    template <class T>
    static T RandomNum(T min, T max);
    
    static bool IsNearlyZeroVector(const glm::vec3& vec, float epsilon = EPS)
    {
        return glm::all(glm::lessThan(glm::abs(vec), glm::vec3(epsilon)));
    }
    
    static glm::vec3 RandomScalarVec3(float min, float max)
    {
        const auto value = RandomNum(min, max);
        return glm::vec3{value, value, value};
    }
    
    static glm::vec3 GetRotationFromDirection(glm::vec3 direction)
    {
        direction = glm::normalize(direction);
        
        const float yaw = glm::degrees(std::asin(-direction.z));
        const float pitch = glm::degrees(std::atan2(direction.y, direction.x));

        return glm::vec3(0.0f, yaw, pitch);
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