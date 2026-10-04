#include "RigidBody.h"

#include "Components/VectorVisualizerComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Physics/RigidBody/Components/ShapeComponents.h"
#include "Physics/RigidBody/Components/RigidBodyComponent.h"

RigidBody::RigidBody(const SceneObjectParams& params)
    : Entity(params)
{
    openUIByDefault = true;
    
    shape = AddComponent<CylinderComponent>("Shape");
    shape->propertyChangedEvent.BindRaw(this, &RigidBody::UpdateProperties);
    shape->openUIByDefault = true;
    
    rigidBodyComponent = AddComponent<RigidBodyComponent>("Rigid Body Component");
    
    CreateVisualizationComponents();
    
    SetRotationMode(RotationMode::Quaternion);
    
    showVisualizationComponents.SetOnChangedEvent(this, &RigidBody::OnShowVisualizationComponents);
}

void RigidBody::Init()
{
    Entity::Init();
    
    UpdateProperties();
}

void RigidBody::PhysicsTick(float physicsDeltaTime)
{
    Entity::PhysicsTick(physicsDeltaTime);
    
    if (showVisualizationComponents)
    {
        UpdateVisualizationComponents();
    }
}

void RigidBody::ApplyForce(const glm::vec3& force, bool velocityChange)
{
    rigidBodyComponent->ApplyForce(force, velocityChange);
}

void RigidBody::ApplyForceAtLocation(const glm::vec3& force, const glm::vec3& location
    , bool velocityChange)
{
    rigidBodyComponent->ApplyForceAtLocation(force, location, velocityChange);
}

void RigidBody::UpdateVisualizationComponents()
{
    const glm::vec3& centerOfMass = rigidBodyComponent->GetCenterOfMass();
    const glm::vec3& linearVelocity = rigidBodyComponent->GetLinearVelocity();
    const glm::vec3& angularVelocity = rigidBodyComponent->GetAngularVelocity();
    const glm::vec3 angularMomentum = rigidBodyComponent->CalculateAngularMomentum();
    
    if (linearVelocityVisComp->visible)
    {
        linearVelocityVisComp->SetStart(centerOfMass);
        linearVelocityVisComp->SetDirection(linearVelocity);
    }
    
    if (angularVelocityVisComp->visible)
    {
        angularVelocityVisComp->SetStart(centerOfMass);
        angularVelocityVisComp->SetDirection(angularVelocity);
    }
    
    if (angularMomentumVisComp->visible)
    {
        angularMomentumVisComp->SetStart(centerOfMass);
        angularMomentumVisComp->SetDirection(angularMomentum);
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
    rigidBodyComponent->UpdateShapeProperties(shape);
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
