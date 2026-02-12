#include "Asteroid.h"

#include "Components/RotatingComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"

Asteroid::Asteroid(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshEntity(parent, scene, name)
{
    SetMesh(MeshManager::Get().GetAssetByName("planet"));
    SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
    
    rotatingComponent = AddComponent<RotatingComponent>();
    rotatingComponent->rotatingData.localAngularVelocity = {
        MathUtils::RandomNum(10.0f, 50.0f)
        , MathUtils::RandomNum(10.0f, 50.0f)
        , MathUtils::RandomNum(10.0f, 50.0f)};
    
    rotatingComponent->rotatingData.orbitAngularVelocity = {
        //MathUtils::RandomNum(1.0f, 2.0f)
        0.0f
        , MathUtils::RandomNum(10.0f, 50.0f)
        , 0.0f //MathUtils::RandomNum(1.0f, 2.0f)
    };
    
    SetUseQuaternionsForRotation(true);
    
    SetScale({MathUtils::RandomNum(0.5f, 3.0f), MathUtils::RandomNum(0.5f, 3.0f), MathUtils::RandomNum(0.5f, 3.0f)});
}
