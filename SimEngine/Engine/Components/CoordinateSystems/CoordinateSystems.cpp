#include "CoordinateSystems.h"

glm::vec3 CoordinateSystem::Move(const glm::vec3& moveDelta)
{
    position += moveDelta;
    CheckCoordinates();
    UpdateCartesianPosition();
    return cartesianPosition;
}

void CartesianCoordinateSystem::UpdateCartesianPosition()
{
    cartesianPosition = position;
}

void CylindricalCoordinateSystem::CheckCoordinates()
{
    position.x = glm::max(0.0f, position.x);
    position.y = std::fmod(glm::max(0.0f, position.y), glm::two_pi<float>());
}

void CylindricalCoordinateSystem::UpdateCartesianPosition()
{
    cartesianPosition.x = position.x * glm::cos(position.y);
    cartesianPosition.y = position.z;
    cartesianPosition.z = position.x * glm::sin(position.y);
}

void SphericalCoordinateSystem::CheckCoordinates()
{
    position.x = glm::max(0.0f, position.x);
    position.y = std::fmod(glm::max(0.0f, position.y), glm::pi<float>());
    position.z = std::fmod(glm::max(0.0f, position.z), glm::two_pi<float>());
}

void SphericalCoordinateSystem::UpdateCartesianPosition()
{
    cartesianPosition.x = position.x * glm::sin(position.y) * glm::cos(position.z);
    cartesianPosition.y = position.x * glm::cos(position.y);
    cartesianPosition.z = position.x * glm::sin(position.y) * glm::sin(position.z);
}
