#include "GraphicsPipeline.hpp"
#include "Swapchain.hpp"
#include "Depth.hpp"
#include <Core/Interfaces/RendererInterface.hpp>

UVK::GraphicsPipeline::GraphicsPipeline(UVK::VKDevice& dev, Swapchain& swap, VKDescriptors& desc, VKDepthBuffer& depth) noexcept
{
    device = &dev;
    swapchain = &swap;
    descriptors = &desc;
    depthBuffer = &depth;
}

void UVK::GraphicsPipeline::createGraphicsPipeline() noexcept
{
    descriptors->createPushConstantRange();
    std::vector<VKShader> shaders;
    std::vector<VkPipelineShaderStageCreateInfo> stages;

    VKShader::each([&](VKShader& shader){
        // TODO: Remove this and make the shaders per object
        Utility::sanitiseFilepath(shader.name, false);
        if (shader.name.find("../Content/Engine/vkshader.") != FString::npos)
            shaders.push_back(shader);
    });

    for (auto& a : shaders)
    {
        auto code = a.getShaderBytecode();

        // Create the shader module
        const VkShaderModuleCreateInfo shaderModuleCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = code.size(),
            .pCode = reinterpret_cast<uint32_t*>(code.data()),
        };
        VkShaderModule module;
        // Create the shader module
        VkResult result = vkCreateShaderModule(device->getDevice(), &shaderModuleCreateInfo, nullptr, &module);
        if (result != VK_SUCCESS)
        {
            Logger::log("Failed to create a Vulkan shader module! Error code: ", UVK_LOG_TYPE_ERROR, result);
            std::terminate();
        }

        // Create the pipeline stage
        a.info = VkPipelineShaderStageCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = static_cast<VkShaderStageFlagBits>(a.type),
            .module = module,
            .pName = "main"
        };
        a.module = module;
        stages.push_back(a.info);
    }

    constexpr VkVertexInputBindingDescription bindingDescription =
    {
        .binding = 0,                               // The binding number of the structure
        .stride = sizeof(VKVertex),                 // The offset between consecutive elements withing the buffer
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX    // Make it per vertex
    };

    // Set the vertex input attribute descriptions
    // Indices:
    //     0 - Coordinate
    //     1 - Colour
    //     2 - UV
    //     3 - Normal
    constexpr VkVertexInputAttributeDescription attributeDescriptions[] =
    {
        {
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(VKVertex, pos)
        },
        {
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(VKVertex, colour)
        },
        {
            .location = 2,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(VKVertex, uv)
        },
        {
            .location = 3,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(VKVertex, normal)
        }
    };

    // Create the pipeline vertex state informatino struct
    const VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &bindingDescription,
        .vertexAttributeDescriptionCount = 4,
        .pVertexAttributeDescriptions = attributeDescriptions
    };

    // Set the topology to render triangles
    constexpr VkPipelineInputAssemblyStateCreateInfo inputAssembly =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    // Make the viewports dynamic
    const VkPipelineViewportStateCreateInfo viewportStateCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = nullptr,
        .scissorCount = 1,
        .pScissors = nullptr
    };

    // Starting rasterizer here
    constexpr VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,                           // Disable depth clamp
        .rasterizerDiscardEnable = VK_FALSE,                    // Disable discarding from the rasterizer
        .polygonMode = VK_POLYGON_MODE_FILL,                    // TODO: Check this out for wireframe
        .cullMode = VK_CULL_MODE_BACK_BIT,                      // Which face is the backface
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,           // Backface culling technique
        .depthBiasEnable = VK_FALSE,                            // TODO: Might be interesting later
        .lineWidth = 1.0f,                                      // Set the line width
    };

    // Get if sample rate shading is enabled
    VkBool32 sampleRateShading = Renderer::sampleRateShading() ? VK_TRUE : VK_FALSE;
    Renderer::sampleRateShadingMult() = Renderer::sampleRateShadingMult() > 1.0f ? 1.0f : Renderer::sampleRateShadingMult() < 0.0f ? 0.0f : Renderer::sampleRateShadingMult();


    // Enable multisampling and set the sample rate shading
    const VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = static_cast<VkSampleCountFlagBits>(UVK::Renderer::msaaSampleCount()),   // Number of samples
        .sampleShadingEnable = sampleRateShading,                                                       // Is the sample rate shading feature enabled or disabled
        .minSampleShading = Renderer::sampleRateShadingMult()                                           // The sample rate shading multiplier
    };

    constexpr VkPipelineColorBlendAttachmentState colourState =
    {
        .blendEnable = VK_TRUE,                                     // Enable blending
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };

    const VkPipelineColorBlendStateCreateInfo colourBlendingCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,      // Disable use of logical operators
        .attachmentCount = 1,
        .pAttachments = &colourState,   // Pass the attachment
    };

    // A list of descriptor layouts, one for the regular descriptor and one for the sampler
    const VkDescriptorSetLayout descriptorSetLayouts[] =
    {
        descriptors->layout(),
        descriptors->samplerLayout()
    };

    const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 2,
        .pSetLayouts = descriptorSetLayouts,                            // Pass the layouts
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &descriptors->getPushConstantRange(),    // Pass the push constant range struct
    };

    // Create the pipeline layout
    auto result = vkCreatePipelineLayout(device->getDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
    if (result != VK_SUCCESS)
    {
        Logger::log("Couldn't create a Vulkan graphics pipeline layout! Error code: ", UVK_LOG_TYPE_ERROR, result);
        std::terminate();
    }

    const VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_TRUE,             // Enable depth
        .depthWriteEnable = VK_TRUE,            // Enable writing to the depth buffer
        .depthCompareOp = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,          // TODO: Add stencil testing
    };

    // Set up dynamic state
    constexpr VkDynamicState dynamicStateEnables[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

    const VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = dynamicStateEnables,
    };

    const VkGraphicsPipelineCreateInfo pipelineCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<uint32_t>(stages.size()),
        .pStages = stages.data(),
        .pVertexInputState = &vertexInputStateCreateInfo,
        .pInputAssemblyState = &inputAssembly,
        .pTessellationState = nullptr,                          // TODO: Implement tessellation
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multisampleStateCreateInfo,
        .pDepthStencilState = &depthStencilStateCreateInfo,
        .pColorBlendState = &colourBlendingCreateInfo,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = pipelineLayout,
        .renderPass = renderPass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1
    };

    // Create the graphics pipeline
    result = vkCreateGraphicsPipelines(device->getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
    if (result != VK_SUCCESS)
    {
        Logger::log("Failed to create a Vulkan graphics pipeline! Error code: ", UVK_LOG_TYPE_ERROR, result);
        std::terminate();
    }

    // Destroy the shader modules
    for (const auto& a : shaders)
        vkDestroyShaderModule(device->getDevice(), a.module, nullptr);
}

void UVK::GraphicsPipeline::destroyGraphicsPipeline() noexcept
{
    vkDestroyPipeline(device->getDevice(), graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device->getDevice(), pipelineLayout, nullptr);
    vkDestroyRenderPass(device->getDevice(), renderPass, nullptr);
}

void UVK::GraphicsPipeline::createRenderPass() noexcept
{
    VkImageLayout finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    uint32_t attachmentCount = 3;
    if (Renderer::msaaSampleCount() >= 0 && Renderer::msaaSampleCount() < 2)
    {
        finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachmentCount = 2;
    }

    // Create the render pass attachments
    // Indices:
    //     0: Colour
    //     1: Depth
    //     2: Colour resolve(for anti-aliasing)
    const VkAttachmentDescription descriptions[] =
    {
        // Colour attachment
        {
            .format = swapchain->surfaceFormat.format,
            .samples = static_cast<VkSampleCountFlagBits>(UVK::Renderer::msaaSampleCount()),
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = finalLayout,
        },
        // Depth buffer attachment
        {
            .format = depthBuffer->getFormat(),
            .samples = static_cast<VkSampleCountFlagBits>(UVK::Renderer::msaaSampleCount()),
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        },
        // Colour resolve attachment
        {
            .format = swapchain->surfaceFormat.format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        }
    };

    // Same layout as the above array
    constexpr VkAttachmentReference attachmentReferences[] =
    {
        {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        },
        {
            .attachment = 1,
            .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        },
        {
            .attachment = 2,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        }
    };

    // Set the resolve attachment depending on weather MSAA is enabled
    const VkAttachmentReference* resolveAttachment = &attachmentReferences[2];
    if (Renderer::msaaSampleCount() >= 0 && Renderer::msaaSampleCount() < 2)
        resolveAttachment = nullptr;

    // Create the subpass
    const VkSubpassDescription subpass =
    {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachmentReferences[0],
        .pResolveAttachments = resolveAttachment,
        .pDepthStencilAttachment = &attachmentReferences[1],
    };

    // Create the subpass dependencies
    constexpr VkSubpassDependency subpassDependency[2] =
    {
        {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dependencyFlags = 0,
        },
        {
            .srcSubpass = 0,
            .dstSubpass = VK_SUBPASS_EXTERNAL,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
            .dependencyFlags = 0,
        }
    };

    // Create the render pass
    const VkRenderPassCreateInfo renderPassCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = attachmentCount,
        .pAttachments = descriptions,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 2,
        .pDependencies = subpassDependency
    };

    auto result = vkCreateRenderPass(device->getDevice(), &renderPassCreateInfo, nullptr, &renderPass);
    if (result != VK_SUCCESS)
    {
        Logger::log("Failed to create a Vulkan render pass! Error code: ", UVK_LOG_TYPE_ERROR, result);
        std::terminate();
    }
}

VkPipelineLayout& UVK::GraphicsPipeline::getPipelineLayout() noexcept
{
    return pipelineLayout;
}

VkRenderPass& UVK::GraphicsPipeline::getRenderPass() noexcept
{
    return renderPass;
}

VkPipeline& UVK::GraphicsPipeline::getPipeline() noexcept
{
    return graphicsPipeline;
}
