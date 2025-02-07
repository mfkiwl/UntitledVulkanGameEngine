#include "Utility.hpp"
#include <UVKShaderCompiler/Src/Functions.hpp>
#include "Device.hpp"

bool UVK::QueueFamilyIndices::valid() const noexcept
{
    return graphicsFamily >= 0 && presentationFamily >= 0;
}

std::vector<char> UVK::VKShader::getShaderBytecode() const noexcept
{
    auto hash = USC::getShaderHash(name.c_str());
    bool bFound = false;
    for (const auto& a : std_filesystem::directory_iterator("../Generated"))
    {
        if (!a.is_directory())
        {
            if (a.path() == std_filesystem::path("../Generated/" + hash + ".spv"))
            {
                bFound = true;
                break;
            }
        }
    }

    if (!bFound)
        USC::recompileShaders();

    std::ifstream in("../Generated/" + hash + ".spv", std::ios::ate | std::ios::binary);
    if (!in.is_open())
    {
        Logger::log("Failed to load file!", UVK_LOG_TYPE_ERROR);
        std::terminate();
    }

    std::vector<char> returnVal(in.tellg());
    in.seekg(0);
    in.read(returnVal.data(), returnVal.size());
    in.close();

    return returnVal;
}

UVK::VKShader::VKShader(const char* fname) noexcept
{
    init(fname);
}

void UVK::VKShader::init(const char* fname) noexcept
{
    name = fname;
    if (name.rfind(".vert") != UVK::FString::npos)
        type = VKShaderType::VK_SHADER_TYPE_VERTEX;
    else if (name.rfind(".frag") != UVK::FString::npos)
        type = VKShaderType::VK_SHADER_TYPE_FRAGMENT;
    else if (name.rfind(".geom") != UVK::FString::npos)
        type = VKShaderType::VK_SHADER_TYPE_GEOMETRY;
    else if (name.rfind(".comp") != UVK::FString::npos)
        type = VKShaderType::VK_SHADER_TYPE_COMPUTE;
    else if (name.rfind(".tesc") != UVK::FString::npos)
        type = VKShaderType::VK_SHADER_TYPE_TESSELLATION_CONTROL;
    else if (name.rfind(".tese") != UVK::FString::npos)
        type = VKShaderType::VK_SHADER_TYPE_TESSELLATION_EVALUATION;
    else
        type = VKShaderType::VK_SHADER_TYPE_NONE;
}

void UVK::VKShader::each(const std::function<void(VKShader&)>& func) noexcept
{
    for (auto& a : std_filesystem::recursive_directory_iterator("../Content/"))
    {
        if (!a.is_directory())
        {
            VKShader shader(a.path().string().c_str());
            if (shader.type == VKShaderType::VK_SHADER_TYPE_NONE)
                continue;
            func(shader);
        }
    }
}

VkFormat UVK::SwapchainImage::findBestImageFormat(const std::vector<VkFormat>& formats, const VkImageTiling& tiling, VkFormatFeatureFlags featureFlags, VKDevice& device)
{
    for (auto& a : formats)
    {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(device.getPhysicalDevice(), a, &properties);
        if ((tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & featureFlags) == featureFlags) || (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & featureFlags) == featureFlags))
            return a;
    }
    Logger::log("Couldn't find optimal image format!", UVK_LOG_TYPE_ERROR);
    std::terminate();
}

void UVK::SwapchainImage::createImage(const UVK::FVector2& size, const VkFormat& format, const VkImageTiling& tiling, const VkImageUsageFlags& usageFlags, const VkMemoryPropertyFlags& propertyFlags, UVK::VKDevice& device, uint32_t mipLevels, VkSampleCountFlagBits sampleCountFlagBits)
{
    const VkImageCreateInfo imageCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent =
        {
            .width = static_cast<uint32_t>(size.x),
            .height = static_cast<uint32_t>(size.y),
            .depth = 1
        },
        .mipLevels = mipLevels,
        .arrayLayers = 1,
        .samples = sampleCountFlagBits,
        .tiling = tiling,
        .usage = usageFlags,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    auto result = vkCreateImage(device.getDevice(), &imageCreateInfo, nullptr, &image);
    if (result != VK_SUCCESS)
    {
        Logger::log("Couldn't create a vulkan image! Error code: ", UVK_LOG_TYPE_ERROR, result);
        std::terminate();
    }

    VkMemoryRequirements memoryRequirements = {};
    vkGetImageMemoryRequirements(device.getDevice(), image, &memoryRequirements);

    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device.getPhysicalDevice(), &memoryProperties);

    uint32_t i = 0;
    for (; i < memoryProperties.memoryTypeCount; i++)
        if ((memoryRequirements.memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
            goto success;
    Logger::log("Couldn't get required memory property flags from the currently selected device!", UVK_LOG_TYPE_ERROR);
    std::terminate();
    success:
    const VkMemoryAllocateInfo memoryAllocateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = i
    };

    result = vkAllocateMemory(device.getDevice(), &memoryAllocateInfo, nullptr, &memory);
    if (result != VK_SUCCESS)
    {
        Logger::log("Couldn't allocate memory for hte given image! Error code: ", UVK_LOG_TYPE_ERROR, result);
        std::terminate();
    }
    vkBindImageMemory(device.getDevice(), image, memory, 0);
}

void UVK::SwapchainImage::createImageView(const VkFormat& format, const VkImageAspectFlags& aspectFlags, UVK::VKDevice& dev, uint32_t mipLevels)
{
    const VkImageViewCreateInfo viewCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .components =
        {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .subresourceRange =
        {
            .aspectMask = aspectFlags,
            .baseMipLevel = 0,
            .levelCount = mipLevels,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
    };
    auto result = vkCreateImageView(dev.getDevice(), &viewCreateInfo, nullptr, &imageView);
    if (result != VK_SUCCESS)
    {
        Logger::log("Failed to create an image view! Error code: ", UVK_LOG_TYPE_ERROR, result);
        std::terminate();
    }
}

void UVK::SwapchainImage::destroy(VKDevice& device) const
{
    vkDestroyImageView(device.getDevice(), imageView, nullptr);
    vkDestroyImage(device.getDevice(), image, nullptr);
    vkFreeMemory(device.getDevice(), memory, nullptr);
}
