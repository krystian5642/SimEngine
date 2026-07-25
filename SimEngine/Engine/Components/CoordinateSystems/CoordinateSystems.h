#pragma once

enum class CoordinateSystemType
{
    Cartesian,
    Cylindrical,
};

class CoordinateSystem
{
public:
    virtual ~CoordinateSystem() = 0 {}
    
    glm::vec3 Move(const glm::vec3& moveDelta);
    const glm::vec3& GetPosition(bool getCartesianPosition = false) const { return getCartesianPosition ? cartesianPosition : position; }
    
protected:
    virtual void CheckCoordinates() {}
    virtual void UpdateCartesianPosition() {}
    
    glm::vec3 position{};
    glm::vec3 cartesianPosition{};
};

class CartesianCoordinateSystem : public CoordinateSystem
{
protected:
    void UpdateCartesianPosition() override;
};

class CylindricalCoordinateSystem : public CoordinateSystem
{
protected:
    void CheckCoordinates() override;
    void UpdateCartesianPosition() override;
};