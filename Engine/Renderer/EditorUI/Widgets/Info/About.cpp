#include "About.hpp"
#ifndef PRODUCTION
#include <imgui.h>

void About::display(UVK::FString& engineVersion, UVK::FString& projectName, UVK::FString& projectVersion, UVK::Texture& logo, bool& bShow) noexcept
{
    if (!ImGui::IsPopupOpen("About us"))
        ImGui::OpenPopup("About us");
    if (ImGui::BeginPopupModal("About us", &bShow))
    {
        ImGui::TextWrapped("Untitled Vulkan Game Engine, version %s", engineVersion.c_str());
        ImGui::TextWrapped("Currently editing %s, version %s", projectName.c_str(), projectVersion.c_str());
        ImGui::Separator();
        ImGui::TextWrapped("The Untitled Vulkan Game Engine is brought to you by MadLad Squad");
        ImGui::TextWrapped("Project created and maintained by Stanislav Vasilev (Madman10K)");
        ImGui::Image((void*)(intptr_t)logo.getImage(), ImVec2((float)logo.getWidth() - 100, (float)logo.getHeight() - 100));

        if (ImGui::Button("Close##AboutUS"))
            bShow = false;
        ImGui::EndPopup();
    }
}
#endif