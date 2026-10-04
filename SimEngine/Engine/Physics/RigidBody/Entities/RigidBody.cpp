#include "RigidBody.h"

#include "Components/VectorVisualizerComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Physics/RigidBody/Components/RigidBody/ShapeComponents.h"

RigidBody::RigidBody(const SceneObjectParams& params)
    : Entity(params)
{
    openUIByDefault = true;
    
    shape = AddComponent<CylinderComponent>("Shape");
    shape->propertyChangedEvent.BindRaw(this, &RigidBody::UpdateProperties);
    shape->openUIByDefault = true;
    
    CreateVisualizationComponents();
    
    SetRotationMode(RotationMode::Quaternion);
    
    showVisualizationComponents.SetOnChangedEvent(this, &RigidBody::OnShowVisualizationComponents);
}

void RigidBody::Init()
{
    Entity::Init();
    
    localInertiaTensor = shape->CalculateLocalInertiaTensor();
    mass = shape->mass;
    invMass = 1.0f / mass;

    inertiaTensor = localInertiaTensor;
    invInertiaTensor = glm::inverse(localInertiaTensor);
}

void RigidBody::PhysicsTick(float physicsDeltaTime)
{
    Entity::Tick(physicsDeltaTime);
    
    const glm::mat3 rotationMatrix = glm::mat3(GetRotationMatrix());
    const glm::mat3 transRotationMatrix = glm::transpose(rotationMatrix);
    
    inertiaTensor = rotationMatrix * localInertiaTensor * transRotationMatrix;
    invInertiaTensor = glm::inverse(inertiaTensor);
    
    const glm::vec3 linearAcceleration = accumulatedForce * invMass;
    
    velocity.linearVelocity += linearAcceleration * physicsDeltaTime;
    
    const glm::vec3 moveDelta = velocity.linearVelocity * physicsDeltaTime;
    Move(moveDelta);
    
    centerOfMass += moveDelta;
    
    const glm::vec3 angularAcceleration = invInertiaTensor * (accumulatedTorque
            - glm::cross(velocity.angularVelocity, inertiaTensor * velocity.angularVelocity));
        
    velocity.angularVelocity += angularAcceleration * physicsDeltaTime;
    
    const float rotationDelta = glm::length(velocity.angularVelocity) * physicsDeltaTime;
    Rotate(rotationDelta, velocity.angularVelocity);
    
    accumulatedForce = {};
    accumulatedTorque = {};
    
    if (showVisualizationComponents)
    {
        UpdateVisualizationComponents();
    }
}

void RigidBody::ApplyForce(const glm::vec3& force, bool velocityChange)
{
    if (velocityChange)
    {
        velocity.linearVelocity += force / mass;
    }
    else
    {
        accumulatedForce += force;
    }
}

void RigidBody::ApplyTorque(const glm::vec3& force, const glm::vec3& location, bool velocityChange)
{
    const glm::vec3 torque = glm::cross(location - centerOfMass, force);
    if (velocityChange)
    {
        velocity.angularVelocity += invInertiaTensor * torque;
    }
    else
    {
        accumulatedTorque += torque;
    }
    
    for (auto* property : shape->GetProperties())
    {
        property->readOnly = true;
    }
    
    ApplyForce(force, velocityChange);
}

void RigidBody::UpdateVisualizationComponents()
{
    if (linearVelocityVisComp->visible)
    {
        linearVelocityVisComp->SetStart(centerOfMass);
        linearVelocityVisComp->SetDirection(velocity.linearVelocity);
    }
    
    if (angularVelocityVisComp->visible)
    {
        angularVelocityVisComp->SetStart(centerOfMass);
        angularVelocityVisComp->SetDirection(velocity.angularVelocity);
    }
    
    if (angularMomentumVisComp->visible)
    {
        angularMomentumVisComp->SetStart(centerOfMass);
        angularMomentumVisComp->SetDirection(inertiaTensor * velocity.angularVelocity);
    }
    
    if (centerOfMassVisLine->visible)
    {
        centerOfMassVisLine->GetLine()->AddPoint(centerOfMass);
    }
    
    if (centerOfMassVisMesh->visible)
    {
        centerOfMassVisMesh->SetPosition(centerOfMass);
    }
}

void RigidBody::UpdateProperties()
{
    localInertiaTensor = shape->CalculateLocalInertiaTensor();
    mass = shape->mass;
    
    invMass = 1.0f / mass;
}

void RigidBody::OnShowVisualizationComponents(bool newShowVisualizers)
{
    for (auto* visualizationComponent : visualizationComponents)
    {
        visualizationComponent->visible = newShowVisualizers;
        visualizationComponent->visible.readOnly = !newShowVisualizers;
    }
}

void RigidBody::CreateVisualizationComponents()
{
    linearVelocityVisComp = AddComponent<VectorVisualizerComponent>("Linear Velocity");
    linearVelocityVisComp->useParentLocationAsStart = false;
    linearVelocityVisComp->scaleLenghtFactor = 2.0f;
    linearVelocityVisComp->scaleFactor = 0.5f;
    linearVelocityVisComp->color = {0.0f, 1.0f, 0.0f, 1.0f};
    visualizationComponents.push_back(linearVelocityVisComp);

    angularVelocityVisComp = AddComponent<VectorVisualizerComponent>("Angular Velocity");
    angularVelocityVisComp->useParentLocationAsStart = false;
    angularVelocityVisComp->scaleLenghtFactor = 1.0f;
    angularVelocityVisComp->scaleFactor = 0.5f;
    angularVelocityVisComp->color = {1.0f, 0.0f, 0.0f, 1.0f};
    visualizationComponents.push_back(angularVelocityVisComp);

    angularMomentumVisComp = AddComponent<VectorVisualizerComponent>("Angular Momentum");
    angularMomentumVisComp->useParentLocationAsStart = false;
    angularMomentumVisComp->scaleLenghtFactor = 7.0f;
    angularMomentumVisComp->scaleFactor = 0.5f;
    angularMomentumVisComp->color = {1.0f, 0.8f, 0.0f, 1.0f};
    visualizationComponents.push_back(angularMomentumVisComp);

    centerOfMassVisLine = AddComponent<LineComponent>("Center of Mass Line");
    centerOfMassVisLine->followParent = false;
    centerOfMassVisLine->GetLine()->color = glm::vec4{0.5f, 0.7f, 1.0f, 1.0f};
    centerOfMassVisLine->GetLine()->thickness = 3.0f;
    centerOfMassVisLine->GetLine()->maxLength = 10.0f;
    visualizationComponents.push_back(centerOfMassVisLine);

    centerOfMassVisMesh = AddComponent<MeshComponent>("Center of Mass");
    centerOfMassVisMesh->mesh = MeshManager::Get().GetAssetByName("sphere");
    centerOfMassVisMesh->material = MaterialManager::Get().GetAssetByName("ruby");
    centerOfMassVisMesh->SetScale(glm::vec3{0.2f});
    visualizationComponents.push_back(centerOfMassVisMesh);
}
