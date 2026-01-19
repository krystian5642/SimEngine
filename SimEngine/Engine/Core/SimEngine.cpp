#include "App.h"

int main()
{
    App::currentApp = CreateApplication();
    App::currentApp->Run();
    delete App::currentApp;
    
    return 0;
}
