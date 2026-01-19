#pragma once

struct GLFWwindow;

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void Update();
    void SwapBuffers();

    double ConsumeXMouseDelta();
    double ConsumeYMouseDelta();
    
    double ConsumeYMouseScrollDelta();

    float GetAspectRatio() const;
    bool IsKeyDown(int key) const;
    bool IsMouseButtonDown(int button) const;
    bool ShouldClose() const;
    void SetWindowShouldClose(int value) const;

    void SetWindowTitle(const std::string& title);

    GLFWwindow* GetGLFWWindow() const { return window; }
    
private:
    static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);
    static void OnMouseEvent(GLFWwindow* window, double xPos, double yPos);
    static void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
    static void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

    int width, height;

    int bufferWidth, bufferHeight;

    GLFWwindow* window;

    bool firstMouseMove = true;
    double lastXMouse, lastYMouse;
    double mouseXDelta{}, mouseYDelta{};
    
    double lastYMouseScroll{};
};
