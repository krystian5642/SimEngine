#include "Renderer.h"
#include "Core/App.h"
#include "Core/Logging/Log.h"

void Renderer::Init()
{
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
}

void Renderer::Shutdown()
{
    if (enableValidationLayers)
    { 
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(mainDevice.logicalDevice, nullptr);
    vkDestroyInstance(instance, nullptr);
}

void Renderer::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        ASSERT(false, "Validation layers requested, but not available!")
    }
    
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "SimEngine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "SimEngine";
    appInfo.apiVersion = VK_API_VERSION_1_4;
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    const auto extensions = getRequiredExtensions();
    ASSERT(checkInstanceExtensionSupport(extensions), "Failed to find all required extensions")
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
    }
    
    const auto result = vkCreateInstance(&createInfo, nullptr, &instance);
    ASSERT(result == VK_SUCCESS, "Failed to create vulkan instance")
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
    , VkDebugUtilsMessageTypeFlagsEXT messageType
    , const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData
    , void* pUserData) 
{
    LOG_ERROR("validation layer: ", pCallbackData->pMessage);

    return VK_FALSE;
}

void Renderer::setupDebugMessenger()
{
    if (!enableValidationLayers) return;
    
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
    createInfo.flags = 0;
    
    const auto result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
    ASSERT(result == VK_SUCCESS, "Failed to create debug messenger")
}

void Renderer::createSurface()
{
    glfwCreateWindowSurface(instance, App::GetCurrentWindow()->GetGLFWWindow(), nullptr, &surface);
    ASSERT(surface != nullptr, "Failed to create window surface")
}

void Renderer::pickPhysicalDevice()
{
    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    ASSERT(deviceCount != 0, "Failed to find GPUs with Vulkan support")
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    for (const auto& device : devices)
    {
        if (checkPhysicalDeviceSuitable(device))
        {
            mainDevice.physicalDevice = device;
            break;
        }
    }
}

void Renderer::createLogicalDevice()
{
    const auto indices = getQueueFamilies(mainDevice.physicalDevice);
    ASSERT(indices.isComplete(), "Failed to find a queue family")
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const std::set uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    
    for (const auto queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    VkPhysicalDeviceFeatures deviceFeatures{};
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    const auto result = vkCreateDevice(mainDevice.physicalDevice, &createInfo, nullptr, &mainDevice.logicalDevice);
    ASSERT(result == VK_SUCCESS, "Failed to create logical device")
    
    vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(mainDevice.logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

void Renderer::createSwapChain()
{
    const auto swapChainDetails = getSwapChainDetails(mainDevice.physicalDevice);
    
    const auto surfaceFormat = chooseSwapSurfaceFormat(swapChainDetails.formats);
    const auto presentMode = chooseSwapPresentMode(swapChainDetails.presentModes);
}

bool Renderer::checkInstanceExtensionSupport(const std::vector<const char*>& extensions) const
{
    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
    
    for (const auto extension : extensions)
    {   
        bool extensionFound = false;
        for (const auto& availableExtension : availableExtensions)
        {
            if (strcmp(availableExtension.extensionName, extension) == 0)
            {
                extensionFound = true;
                break;
            }
        }
        
        if (!extensionFound)
        {
            return false;
        }
    }
    
    return true;
}

bool Renderer::checkValidationLayerSupport() const
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    for (const auto layerName : validationLayers)
    {
        bool layerFound = false;
        for (const auto layer : availableLayers)
        {
            if (strcmp(layerName, layer.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
        if (!layerFound)
        {
            return false;
        }
    }
    return true;
}

bool Renderer::checkPhysicalDeviceSuitable(VkPhysicalDevice device) const
{
    /*VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    return deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;*/
    
    const auto indices = getQueueFamilies(device);
    if (!indices.isComplete())
    {
        return false;
    }
    
    const auto swapChainDetails = getSwapChainDetails(device);
    if (!swapChainDetails.isComplete())
    {
        return false;
    }
    
    const auto extensionsSupported = checkDeviceExtensionSupport(device);
    return extensionsSupported;
}

bool Renderer::checkDeviceExtensionSupport(VkPhysicalDevice device) const
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
    
    for (const auto extension : deviceExtensions)
    {
        bool extensionFound = false;
        for (const auto& availableExtension : availableExtensions)
        {
            if (strcmp(extension, availableExtension.extensionName) == 0)
            {
                extensionFound = true;
                break;
            }
        }
        
        if (!extensionFound)
        {
            return false;
        }
    }
    return true;
}

std::vector<const char*> Renderer::getRequiredExtensions() const
{
    uint32_t glfwExtensionCount;
    const auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    
    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    
    return extensions;
}

VkResult Renderer::CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func) 
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } 
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void Renderer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func) 
    {
        func(instance, debugMessenger, pAllocator);
    }
}

QueueFamilyIndices Renderer::getQueueFamilies(VkPhysicalDevice device) const
{
    QueueFamilyIndices indices;
    
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    for (size_t i = 0; i < queueFamilies.size(); i++)
    {
        const auto& queueFamily = queueFamilies[i];
        
        if (queueFamily.queueCount > 0)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = static_cast<uint32_t>(i);
            }
            
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), surface, &presentSupport);
            
            if (presentSupport)
            {
                indices.presentFamily = static_cast<uint32_t>(i);
            }
            
            if (indices.isComplete())
            {
               break;
            }
        }
    }
    
    return indices;
}

SwapChainDetails Renderer::getSwapChainDetails(VkPhysicalDevice device) const
{
    SwapChainDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }
    else
    {
        ASSERT(false, "Failed to get surface formats")
    }
    
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    
    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    else
    {
        ASSERT(false, "Failed to get surface present modes")
    }
    
    return details;
}

VkSurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }
    
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR Renderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}
