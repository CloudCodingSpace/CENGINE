#include "instance.h"

void create_instance(Instance* inst) {
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "CENGINE ",
        .pEngineName = "CENGINE",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };



    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo
    };

#ifdef _DEBUG
    const char** exts = get_debugger_exts();
    VkDebugUtilsMessengerCreateInfoEXT debgInfo = get_debugger_info();
    uint32_t count = get_debugger_exts_count();
    const char* layer[] = {DBG_LAYERNAME};

    createInfo.enabledExtensionCount = count;
    createInfo.ppEnabledExtensionNames = exts;
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = layer;
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debgInfo;
#else
    uint32_t count = 0;
    const char** exts = glfwGetRequiredInstanceExtensions(&count);

    createInfo.enabledExtensionCount = count;
    createInfo.ppEnabledExtensionNames = exts;
    createInfo.enabledLayerCount = 0;
#endif

    VK_CHECK(vkCreateInstance(&createInfo, 0, &inst->instance))
}

void destroy_instance(Instance* inst) {
    vkDestroyInstance(inst->instance, 0);
}