#include "App.h"

#include "imgui_impl_glfw.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "ExampleScenes/TestScenes.h"

#include <GLFW/glfw3.h>

#define REGISTER_SCENE(name) SceneManager::RegisterScene(SceneNames::name, []() -> std::unique_ptr<Scene> { return std::make_unique<name##Scene>(); })

App::App()
{
    SceneManager::SetDefaultScene(SceneNames::SpringTest);
    
    REGISTER_SCENE(FallingBalls);
    REGISTER_SCENE(Gravity);
    REGISTER_SCENE(BallCollisions2D);
    REGISTER_SCENE(BallCollisions3D);
    REGISTER_SCENE(BallLauncher);
    REGISTER_SCENE(CylindricalCoordinate);
    REGISTER_SCENE(SphericalCoordinate);
    REGISTER_SCENE(ArrowTest);
    REGISTER_SCENE(CoriolisEffect);
    REGISTER_SCENE(SpringTest);
}

void App::Run()
{
    renderer.Init();
    SceneManager::Init();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    auto win = &window;
    
    ImGui_ImplGlfw_InitForOpenGL(win->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    lastFrameTime = glfwGetTime();
    while (!window.ShouldClose())
    {
        double currentFrameTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;
        
        window.Update();
        if (glfwGetWindowAttrib(window.GetGLFWWindow(), GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        const double tickTime1 = glfwGetTime();
        SceneManager::Tick(deltaTime);
        const double tickTime2 = glfwGetTime();
        
        const double renderTime1 = glfwGetTime();
        SceneManager::Render();
        const double renderTime2 = glfwGetTime();
        
        ImGui::Begin("Menu");
        
        ImGui::Text("Stats");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Tick Time: %.3f ms", (tickTime2 - tickTime1) * 1000.0f);
        ImGui::Text("Render Time: %.3f ms", (renderTime2 - renderTime1) * 1000.0f);
        ImGui::Text("Total time: %.3f ms", (renderTime2 - tickTime1) * 1000.0f);
        ImGui::Separator();
        
        
        const auto currentScene = SceneManager::GetCurrentScene();
        
        ImGui::Text("Object count: %zu", currentScene->GetObjectCount());
        
        const auto& cameraPosition = currentScene->GetCameraPosition();
        
        float pitch, yaw;
        currentScene->GetCameraRotation(pitch, yaw);
        
        ImGui::Text("Camera position: (%.3f, %.3f, %.3f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
        ImGui::Text("Camera rotation: (pitch %.3f, yaw %.3f)", pitch, yaw);
        
        ImGui::Separator();
        if (ImGui::Button(isPaused ? "Resume" : "Pause"))
        {
            isPaused = !isPaused;
        }
        
        ImGui::Separator();
        
        ImGui::Text("Current scene: %s", currentScene->GetName().c_str());
        
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Test Scenes"))
        {
            const auto scenes = SceneManager::GetSceneNames();
            for (const auto& scene : scenes)
            {
                if (ImGui::Button(scene.c_str()))
                {
                    SceneManager::LoadScene(scene);
                }
            }
        }
        
        ImGui::Separator();
        ImGui::Spacing();
        SceneManager::GetCurrentScene()->DrawImGui();
        
        ImGui::End(); 
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        window.SwapBuffers();
    }
    
    SceneManager::OnDestroy();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

App& App::Get()
{
    static App app;
    return app;
}