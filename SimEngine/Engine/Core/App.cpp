#include "App.h"


#include "Logging/Log.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

App::App()
{
    currentWindow = &window;
}

void App::Run()
{
    Log::Init();
    SceneManager::Init();
    renderer.Init();

    lastFrameTime = glfwGetTime();
    while (!window.ShouldClose())
    {
        double currentFrameTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;
        
        window.Update();
        
        const double tickTime1 = glfwGetTime();
        SceneManager::Tick(deltaTime);
        const double tickTime2 = glfwGetTime();
        
        const double renderTime1 = glfwGetTime();
        SceneManager::Render();
        const double renderTime2 = glfwGetTime();
        
        /*
        ImGui::Begin("Menu");
        
        ImGui::Text("Stats");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Tick Time: %.3f ms", (tickTime2 - tickTime1) * 1000.0f);
        ImGui::Text("Render Time: %.3f ms", (renderTime2 - renderTime1) * 1000.0f);
        ImGui::Text("Total time: %.3f ms", (renderTime2 - tickTime1) * 1000.0f);
        ImGui::Separator();
        */
        
        
        //const auto currentScene = SceneManager::GetCurrentScene();
        
        /*ImGui::Text("Object count: %zu", currentScene->GetObjectCount());*/
        
       // const auto& cameraPosition = currentScene->GetCameraPosition();
        
        //float pitch, yaw;
        //currentScene->GetCameraRotation(pitch, yaw);
        
        /*ImGui::Text("Camera position: (%.3f, %.3f, %.3f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
        ImGui::Text("Camera rotation: (pitch %.3f, yaw %.3f)", pitch, yaw);
        
        ImGui::Separator();
        if (ImGui::Button(isPaused ? "Resume" : "Pause"))
        {
            isPaused = !isPaused;
        }*/
        
        /*ImGui::Separator();
        
        ImGui::Text("Current scene: %s", currentScene->GetName().c_str());
        // Main Scenes
        if (ImGui::CollapsingHeader("Main Scenes", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::Button("Space Explorer"))
            {
                SceneManager::LoadScene("Space Explorer");
            }
        }*/

        // Test Scenes
        /*
        if (ImGui::CollapsingHeader("Test Scenes"))
        {
            const auto scenes = SceneManager::GetSceneNames();
            for (const auto& scene : scenes)
            {
                if (scene == "Space Explorer")
                {
                    continue;
                }
                
                /*if (ImGui::Button(scene.c_str()))
                {
                    SceneManager::LoadScene(scene);
                }#1#
            }
        }
        */
        
        /*ImGui::Separator();
        ImGui::Spacing();*/
        //SceneManager::GetCurrentScene()->DrawImGui();
        
        /*ImGui::End(); 
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/
        
        window.SwapBuffers();
    }
    
    SceneManager::OnDestroy();
    renderer.Shutdown();
    
    /*ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();*/
    
    glfwTerminate();
}
