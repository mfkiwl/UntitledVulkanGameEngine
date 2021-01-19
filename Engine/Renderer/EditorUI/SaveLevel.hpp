#pragma once
#include <Core.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <cpp/imgui_stdlib.h>

namespace UVK
{
    class Level;
}

namespace SaveLevel
{
    class JunkLevel : public UVK::Level
    {
    public:
        virtual void beginPlay() override {}
        virtual void tick(float deltaTime) override {}
        virtual void endPlay() override {}
    };


    static void Display(bool& bOpen, std::string& location, std::string& name)
    {
        ImGui::Begin("Save level");

        ImGui::InputText("location##inputlocationsave", &location);
        ImGui::InputText("name##inputnamesave", &name);

        if (ImGui::Button("Submit##submitbuttononsave"))
        {
            JunkLevel junkLevel;

            junkLevel.save(location.c_str(), name.c_str());
            bOpen = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel##cancelbuttononsave"))
        {
            bOpen = false;
        }

        ImGui::End();
    }
}