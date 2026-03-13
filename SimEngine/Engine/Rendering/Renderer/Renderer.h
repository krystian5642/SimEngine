#pragma once

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    bool isComplete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
    
    bool isComplete() const
    {
        return !formats.empty() && !presentModes.empty();
    }
};
 
struct SwapChainImage
{
    VkImage image;
    VkImageView imageView;
};

class Renderer
{
public:
    void Init();
    void Shutdown();
    
private:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createRenderPass();
    void createGraphicsPipeline();
    
    bool checkInstanceExtensionSupport(const std::vector<const char*>& extensions) const;
    bool checkValidationLayerSupport() const;
    bool checkPhysicalDeviceSuitable(VkPhysicalDevice device) const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
    
    std::vector<const char*> getRequiredExtensions() const;
    
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance
        , const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo
        , const VkAllocationCallbacks* pAllocator
        , VkDebugUtilsMessengerEXT* pDebugMessenger);
    
    void DestroyDebugUtilsMessengerEXT(VkInstance instance
        , VkDebugUtilsMessengerEXT debugMessenger
        , const VkAllocationCallbacks* pAllocator);
    
    QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device) const;
    SwapChainDetails getSwapChainDetails(VkPhysicalDevice device) const;
    
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
    
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;
    
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    
    const std::string shadersFolder = "Engine/Rendering/Renderer/Shaders_2/";
    
#ifdef _DEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif
    
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    struct{
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
    } mainDevice;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSwapchainKHR swapChain;
    std::vector<SwapChainImage> swapChainImages;
    
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
};
