#ifdef _DEBUG

#include "debugger.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)  {

        DEBUG("VULKAN DEBUGGER: %s\n", pCallbackData->pMessage)

        return VK_FALSE;
}

bool check_dbg_extensions()
{
    uint32_t count = 0;
    vkEnumerateInstanceLayerProperties(&count, 0);
    VkLayerProperties* props = (VkLayerProperties*) calloc(count, sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&count, props);

    for(uint32_t i = 0; i < count; i++) {
        if(strcmp(DBG_LAYERNAME, props[i].layerName) == 0) {
            free(props);
            return true;
        }
    }

    free(props);
    return false;
}

VkDebugUtilsMessengerCreateInfoEXT get_debugger_info()
{
    VkDebugUtilsMessengerCreateInfoEXT info = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pfnUserCallback = debugCallback,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
    };

    return info;
}

const char** get_debugger_exts()
{
    uint32_t count = 0;
    const char** glfwExts = glfwGetRequiredInstanceExtensions(&count);
    const char** exts = (const char**) calloc(count + 1, sizeof(char*));
    for(uint32_t i = 0; i < count; i++) {
        exts[i] = glfwExts[i];
    }
    exts[count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    return exts;
}

uint32_t get_debugger_exts_count()
{
    uint32_t count = 0;
    glfwGetRequiredInstanceExtensions(&count);
    return count + 1;
}

void create_debugger(Debugger *debugger, VkInstance instance)
{
    VkDebugUtilsMessengerCreateInfoEXT info = get_debugger_info();
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(func) {
        VK_CHECK(func(instance, &info, 0, &debugger->debugger))
    }
}

void destroy_debugger(Debugger *debugger, VkInstance instance)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func) {
        func(instance, debugger->debugger, 0);
    }
}

#endif