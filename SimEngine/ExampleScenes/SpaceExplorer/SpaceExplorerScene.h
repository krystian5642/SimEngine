#pragma once
#include "Scene/Scene.h"

#define SCENE_NAME inline const std::string

namespace SceneNames 
{
    SCENE_NAME SpaceExplorer = "Space Explorer";
}

class SpaceExplorerScene : public Scene
{
public:
    SpaceExplorerScene(const std::string& name = SceneNames::SpaceExplorer);
};
