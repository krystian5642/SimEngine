#include "App.h"

int main()
{
    SimEngine::App* app = CreateApplication();
    app->Run();
    delete app;
    
    return 0;
}
