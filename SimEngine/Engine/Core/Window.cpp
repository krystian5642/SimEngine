
#include "Window.h"

#include <stdexcept>
#include <GLFW/glfw3.h>

namespace SimEngine
{
    Window::Window(int width, int height, const std::string& title)
        : width(width)
        , height(height)
    {
        if (!glfwInit())
        {
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window.");
        }
        
        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            glfwDestroyWindow(window);
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLEW");
        }

        glfwSetWindowUserPointer(window, this);
        
        glfwSetFramebufferSizeCallback(window, OnWindowSizeChanged);
        glfwSetCursorPosCallback(window, OnMouseEvent);
        glfwSetKeyCallback(window, OnKeyEvent);
        
        glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
        glViewport(0, 0, bufferWidth, bufferHeight);
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::Update()
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(window);
    }

    double Window::ConsumeXMouseDelta()
    {
        double mouseDelta = mouseXDelta;
        mouseXDelta = 0.0f;
        return mouseDelta;
    }

    double Window::ConsumeYMouseDelta()
    {
        double mouseDelta = mouseYDelta;
        mouseYDelta = 0.0f;
        return mouseDelta;
    }

    float Window::GetAspectRatio() const
    {
        return static_cast<float>(width) / static_cast<float>(height);
    }

    bool Window::IsKeyDown(int key) const
    {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(window);
    }

    void Window::SetWindowShouldClose(int value) const
    {
        glfwSetWindowShouldClose(window, value);
    }

    void Window::SetWindowTitle(const std::string& title)
    {
        glfwSetWindowTitle(window, title.c_str());
    }

    void Window::OnWindowSizeChanged(GLFWwindow* window, int width, int height)
    {
        Window* mainWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
        mainWindow->width = width;
        mainWindow->height = height;
        
        glViewport(0, 0, width, height);
    }

    void Window::OnMouseEvent(GLFWwindow* window, double xPos, double yPos)
    {
        Window* mainWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (mainWindow->firstMouseMove)
        {
            mainWindow->lastXMouse = xPos;
            mainWindow->lastYMouse = yPos;
            mainWindow->firstMouseMove = false;
            return;
        }

        mainWindow->mouseXDelta = xPos - mainWindow->lastXMouse;
        mainWindow->mouseYDelta = yPos - mainWindow->lastYMouse;

        mainWindow->lastXMouse = xPos;
        mainWindow->lastYMouse = yPos;
    }

    void Window::OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* mainWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
        
        if (key == GLFW_KEY_ESCAPE)
        {
            mainWindow->SetWindowShouldClose(GL_TRUE);
        }
    }
}
