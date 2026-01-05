#pragma once

#include "Core/App.h"

class ExampleApp : public SimEngine::App
{
public:
    ExampleApp();
    ~ExampleApp() override;
};

SimEngine::App* CreateApplication()
{
    return new ExampleApp();
}