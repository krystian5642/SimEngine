#pragma once

#include <string>
#include <GL/glew.h>

struct GLFWwindow;

namespace SimEngine
{
    class Window
    {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();
    
        void Update();
        void SwapBuffers();

        double ConsumeXMouseDelta();
        double ConsumeYMouseDelta();

        float GetAspectRatio() const;
        bool IsKeyDown(int key) const;
        bool ShouldClose() const;
        void SetWindowShouldClose(int value) const;
    
        void SetWindowTitle(const std::string& title);
    
    private:
        static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);
        static void OnMouseEvent(GLFWwindow* window, double xPos, double yPos);
        static void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    
        int width, height;

        int bufferWidth, bufferHeight;
    
        GLFWwindow* window;

        bool firstMouseMove = true;
        double lastXMouse, lastYMouse;
        double mouseXDelta{}, mouseYDelta{};
    };
}
