#pragma once

#include "Core/App.h"

class ExampleApp : public App
{
public:
    ExampleApp();
};

App* CreateApplication()
{
    return new ExampleApp();
}
