
#include "SpringPendulum.h"

#include "Components/MeshComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Physics/Gravity/SimplePhysicsBall.h"
#include "Physics/Gravity/Components/SimpleGravityComponent.h"
#include "Physics/RigidBody/Components/ShapeComponents.h"
#include "Physics/Spring/Components/PhysicsSpringComponent.h"

SpringPendulum::SpringPendulum(const SceneObjectParams& params)
    : Entity(params)
{
    openUIByDefault = true;
    
    auto springComp = AddComponent<PhysicsSpringComponent>("Physics Spring Component");
    springComp->radius = 0.4f;
    springComp->coilsNum = 13.0f;
    springComp->openUIByDefault = true;
    springComp->GetSpringLine()->thickness = 2.0f;
    springComp->GetSpringLine()->color = glm::vec4{1.0f, 0.9f, 0.3f, 1.0f};
    springComp->GetSpringLine()->thickness = 3.0f;
    
    auto meshCube = AddComponent<MeshComponent>();
    meshCube->mesh = MeshManager::Get().GetAssetByName("box");
    meshCube->material = MaterialManager::Get().GetAssetByName("bronze");
    meshCube->Move(glm::vec3(0.0f, 0.0f, 0.0f));
    meshCube->SetScale(glm::vec3{3.5f, 0.6f, 3.5f});
    
    auto ball = AddChild<SimplePhysicsBall>("Physics Ball");
    
    ball->SetPosition(springComp->end);
    springComp->AttachComponentToEnd(ball->physicsBody);
    
    ball->sphere->radius = 0.8f;
}
