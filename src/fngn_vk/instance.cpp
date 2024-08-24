#include <fngn_vk/instance.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <iostream>
#include <exception>

fngn_vk::instance::instance(
    const window& window)
	: m_window(window)
{
    create_instance();
}

fngn_vk::instance::~instance()
{
    if (fngn_vk::validator::is_enabled() && m_debug_messenger)
    {
        fngn_vk::validator::destroy_debug_utils_messenger_ext(m_instance, m_debug_messenger, nullptr);
        m_debug_messenger = nullptr;
    }

    if (m_instance != nullptr)
    {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = nullptr;
    }
}

void fngn_vk::instance::create_instance()
{
    auto app_info = construct_app_info();
    auto create_info = construct_instance_creation_info(&app_info);
    check_extensions(create_info);

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    if (fngn_vk::validator::is_enabled())
    {
        if (!fngn_vk::validator::check_validation_layer_support())
        {
            throw std::runtime_error("Validation layers requested, but not available!");
        }

        auto layers = fngn_vk::validator::get_validation_layers();
        create_info.enabledLayerCount = static_cast<uint32_t>(fngn_vk::validator::get_num_validation_layers());
        create_info.enabledExtensionCount = static_cast<uint32_t>(fngn_vk::validator::get_num_validation_layers());
        create_info.ppEnabledLayerNames = fngn_vk::validator::get_validation_layers();
        create_info.ppEnabledExtensionNames = fngn_vk::validator::get_validation_extensions();
        fngn_vk::validator::populate_debug_messenger_create_info(debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
    }
    else
    {
        create_info.enabledLayerCount = 0;
        create_info.pNext = nullptr;
    }

    VkResult result = vkCreateInstance(&create_info, nullptr, &m_instance);
    fnvk_verify(result, "create instance");
    setup_debug_messages();
}

VkApplicationInfo fngn_vk::instance::construct_app_info()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Fungeon RTX";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_HEADER_VERSION_COMPLETE;

    return appInfo;
}

VkInstanceCreateInfo fngn_vk::instance::construct_instance_creation_info(VkApplicationInfo* app_info)
{
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = app_info;

    auto required = m_window.get_glfw_required_extensions();

    std::vector<char*> required_cstring{};

    for (auto& string : required)
        required_cstring.push_back(&string.front());

    createInfo.enabledExtensionCount = required.size();
    createInfo.ppEnabledExtensionNames = required_cstring.data();
    createInfo.enabledLayerCount = 0;

    return createInfo;
}

void fngn_vk::instance::check_extensions(const VkInstanceCreateInfo& info)
{
    auto available = get_available_extensions();
    auto required = m_window.get_glfw_required_extensions();

    std::vector<std::string> pure_available_names;
    std::transform(STD_RANGE(available), std::back_inserter(pure_available_names),
        [](const VkExtensionProperties& vk_extension)
        {
            return std::string(vk_extension.extensionName);
        });

    std::sort(STD_RANGE(pure_available_names));
    std::sort(STD_RANGE(required));

    std::cout << "Available Vulkan extensions:" << std::endl;
    std::copy(STD_RANGE(pure_available_names), std::ostream_iterator<std::string>(std::cout, "\t"));
    std::cout << std::endl;

    std::cout << "Required Vulkan extensions:" << std::endl;
    std::copy(STD_RANGE(required), std::ostream_iterator<std::string>(std::cout, "\t"));
    std::cout << std::endl;

    std::vector<std::string> intersection;
    std::set_intersection(STD_RANGE(required), STD_RANGE(pure_available_names),
        std::back_inserter(intersection));

    if (intersection.size() > 0)
    {
        std::cout << "\nMissing required extensions: ...\n";
        std::copy(STD_RANGE(intersection), std::ostream_iterator<std::string>(std::cout, "\t"));
        std::cout << '\n';

        throw std::runtime_error("Could not find all required extensions!");
    }

}

void fngn_vk::instance::setup_debug_messages()
{
    if (!fngn_vk::validator::is_enabled())
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    fngn_vk::validator::populate_debug_messenger_create_info(createInfo);

    if (fngn_vk::validator::create_debug_utils_messenger_ext(m_instance, &createInfo, nullptr, &m_debug_messenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

std::vector<VkExtensionProperties> fngn_vk::instance::get_available_extensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    return extensions;
}