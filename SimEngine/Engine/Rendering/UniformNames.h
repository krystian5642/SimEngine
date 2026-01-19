#pragma once

#define UNIFORM_NAME inline constexpr std::string_view

namespace UniformNames
{
    UNIFORM_NAME model = "model";
    UNIFORM_NAME view = "view";
    UNIFORM_NAME projection = "projection";
    UNIFORM_NAME cameraPosition = "cameraPosition";
}
