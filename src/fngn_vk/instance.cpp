#include "instance.h"
#include <string>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <iostream>
#include <exception>

fngn_vk::instance::instance(const window& window)
	: m_window(window)
{
    create_instance();
}

fngn_vk::instance::~instance()
{
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

    VkResult result = vkCreateInstance(&create_info, nullptr, &m_instance);
    fnvk_verify(result, "create instance");
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

std::vector<VkExtensionProperties> fngn_vk::instance::get_available_extensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    return extensions;
}