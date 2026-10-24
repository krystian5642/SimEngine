
#include "CoupledSprings.h"

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Physics/Gravity/SimplePhysicsBall.h"
#include "Physics/Gravity/Components/SimpleGravityComponent.h"
#include "Physics/RigidBody/Components/ShapeComponents.h"
#include "Physics/Spring/Components/PhysicsSpringComponent.h"

CoupledSprings::CoupledSprings(const SceneObjectParams& params)
    : Entity(params)
{
    springsNum.SetOnChangedEvent(this, &CoupledSprings::OnSpringsNumChanged);
    
    RecreateSprings();
}

void CoupledSprings::RecreateSprings()
{
    Clear();
 
    openUIByDefault = true;
    
    SimpleGravityComponent* prevBody = nullptr;
    for (int i = 0; i < springsNum; i++)
    {
        auto spring = AddComponent<PhysicsSpringComponent>("Physics Spring Component");
        spring->radius = 0.4f;
        spring->coilsNum = 13.0f;
        spring->GetSpringLine()->color = glm::vec4{1.0f, 0.9f, 0.3f, 1.0f};
        spring->GetSpringLine()->thickness = 3.0f;
        spring->start = glm::vec3{0.0f, -static_cast<float>(i) * spring->baseLenght, 0.0f};
        spring->end = glm::vec3{0.0f, -static_cast<float>(i + 1) * spring->baseLenght, 0.0f};
        
        auto ball = AddChild<SimplePhysicsBall>("Physics Ball");
    
        ball->SetPosition(spring->end);
        spring->AttachComponentToEnd(ball->physicsBody);
        
        if (prevBody)
        {
            spring->AttachComponentToStart(prevBody);
        }
        prevBody = ball->physicsBody;
    }
    
    auto meshCube = AddComponent<MeshComponent>();
    meshCube->mesh = MeshManager::Get().GetAssetByName("box");
    meshCube->material = MaterialManager::Get().GetAssetByName("bronze");
    meshCube->SetScale(glm::vec3{3.5f, 0.6f, 3.5f});
}

void CoupledSprings::OnSpringsNumChanged(int newSpringsNum)
{
    RecreateSprings();
}
