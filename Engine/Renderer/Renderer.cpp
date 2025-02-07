#include "Renderer.hpp"
#include <Engine/Core/Core/Global.hpp>
#include <GameFramework/GameplayClasses/Level/Level.hpp>
#include "OpenGL/GLRenderer.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <yaml.h>

namespace YAML
{
    template<>
    struct convert<UVK::FVector4>
    {
        static Node encode(const UVK::FVector4& rhs) noexcept
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
    
        static bool decode(const Node& node, UVK::FVector4& rhs) noexcept
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;
    
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}


void UVK::RendererSettings::saveSettings() const noexcept
{
    YAML::Emitter out;
    out << YAML::BeginMap;

    out << YAML::Key << "vulkan" << YAML::Value << global.bUsesVulkan;
    out << YAML::Key << "theme" << YAML::Value << themeLoc.c_str();
    out << YAML::Key << "v-sync" << YAML::Value << bVsync;
    out << YAML::Key << "v-sync-immediate" << YAML::Value << bVsyncImmediate;
    out << YAML::Key << "msaa-samples" << YAML::Value << samples;
    out << YAML::Key << "sample-rate-shading" << YAML::Value << sampleRateShading;
    out << YAML::Key << "sample-rate-shading-mult" << YAML::Value << sampleRateShadingMult;

    out << YAML::EndMap;

    std::ofstream fileout("../Config/Settings/Renderer.yaml");
    fileout << out.c_str();
}

UVK::RendererInternal::RendererInternal(UVK::Level* lvl, bool bUsesEditor) noexcept
{
    startRenderer(lvl, bUsesEditor);
}

void UVK::RendererInternal::startRenderer(UVK::Level* lvl, bool bUsesEditor) noexcept
{
    loadSettings();

    if (global.bUsesVulkan)
    {
        VulkanRenderer renderer{};
        renderer.run();
    }
    else
    {
        GLRenderer renderer(lvl, bUsesEditor, rs->themeLoc.c_str());
    }
}

void UVK::RendererInternal::loadSettings() noexcept
{
    YAML::Node a;
    bool bUsesConf = true;

    rs = &global.rendererSettings;

    try
    {
        a = YAML::LoadFile("../Config/Settings/Renderer.yaml");
    }
    catch (YAML::BadFile&)
    {
        bUsesConf = false;

        Logger::log("Invalid renderer file defaulting to OpenGL with default theme if the editor is in use!", UVK_LOG_TYPE_ERROR);
    }

    if (bUsesConf)
    {
        if (a["vulkan"])
            global.bUsesVulkan = a["vulkan"].as<bool>();

        if (a["theme"])
            rs->themeLoc = a["theme"].as<std::string>();

        if (a["v-sync"])
            rs->bVsync = a["v-sync"].as<bool>();

        if (a["v-sync-immediate"])
            rs->bVsyncImmediate = a["v-sync-immediate"].as<bool>();
        if (a["msaa-samples"])
            rs->samples = a["msaa-samples"].as<uint32_t>();
        if (a["sample-rate-shading"])
            rs->sampleRateShading = a["sample-rate-shading"].as<bool>();
        if (a["sample-rate-shading-mult"])
            rs->sampleRateShadingMult = a["sample-rate-shading-mult"].as<float>();
    }
    else
    {
        global.bUsesVulkan = false;
    }
}