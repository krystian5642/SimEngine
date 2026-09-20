#pragma once

enum class RotationMode
{
    EulerXYZ,
    Quaternion
};

struct Transform
{
    void SetRotationMode(RotationMode newRotationMode)
    {
        rotationMode = newRotationMode;
        shouldUpdateModelMatrix = true;
    }
    RotationMode GetRotationMode() const { return rotationMode; }
    
    void SetPosition(const glm::vec3& newPosition)
    {
        position = newPosition;
        shouldUpdateModelMatrix = true;
    }
    const glm::vec3& GetPosition() const { return position; }
    
    void SetQuatRotation(const glm::quat& newRotation)
    {
        assert(rotationMode == RotationMode::Quaternion);
        
        quatRotation = newRotation;
        shouldUpdateModelMatrix = true;
    }
    const glm::quat& GetQuatRotation() const { return quatRotation; }
    
    void SetEulerRotation(const glm::vec3& newEulerRotation)
    {
        assert(rotationMode == RotationMode::EulerXYZ);
        
        eulerRotation = newEulerRotation;
        shouldUpdateModelMatrix = true;
    }
    const glm::vec3& GetEulerRotation() const { return eulerRotation; }
    
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
        
        switch (rotationMode)
        {
            case RotationMode::EulerXYZ:
                {
                    cachedModelMatrix = glm::rotate(cachedModelMatrix
                        , glm::radians(eulerRotation.z)
                        , glm::vec3(0.0f, 0.0f, 1.0f));
                    
                    cachedModelMatrix = glm::rotate(cachedModelMatrix
                        , glm::radians(eulerRotation.y)
                        , glm::vec3(0.0f, 1.0f, 0.0f));
                    
                    cachedModelMatrix = glm::rotate(cachedModelMatrix
                        , glm::radians(eulerRotation.x)
                        , glm::vec3(1.0f, 0.0f, 0.0f));      
                    
                    break;
                }
            
            case RotationMode::Quaternion:
                {
                    cachedModelMatrix *= glm::mat4_cast(quatRotation);
                    break;
                }
            default:
                throw std::runtime_error("Invalid rotation mode");
        }
        
        cachedModelMatrix = glm::scale(cachedModelMatrix, scale);
        
        shouldUpdateModelMatrix = false;
    }
    
    RotationMode rotationMode{RotationMode::EulerXYZ};
    
    glm::vec3 position{};
    glm::vec3 eulerRotation{};
    glm::quat quatRotation = glm::identity<glm::quat>();
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