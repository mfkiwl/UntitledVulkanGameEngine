#pragma once
#include <Core/ECS.hpp>
#include <Renderer/Window/Window.hpp>
#include <Renderer/UI/UI.hpp>
#include <Assets/AssetManager.hpp>
#include "UUID.hpp"

namespace UVK
{
    struct UVK_PUBLIC_API RendererSettings
    {
        RendererSettings() = default;
        std::string themeLoc;
        bool bVsync = false;
        bool bVsyncImmediate = true;

        void saveSettings() const noexcept;
    };

    class Level;
    class GameInstance;

    class UVK_PUBLIC_API UVKGlobal
    {
    public:
        UVKGlobal() noexcept;
        ~UVKGlobal() noexcept;
        UVKGlobal(const UVKGlobal&) = delete;
        void operator=(UVKGlobal const&) = delete;

        GameInstance* instance = nullptr;
        WindowInternal window;

        bool& getEditor() noexcept;
        Level* currentLevel = nullptr;
        AssetManager assetManager;

        static void openLevelInternal(UVK::String name, bool bfirst = false) noexcept;

        std::string levelLocation;
        bool bUsesVulkan{};

        std::function<void(void)> modbegin = [](){};
        std::function<void(float)> modtick = [](float){};
        std::function<void(void)> modend = [](){};
    private:
        IDManager idManager;

        RendererSettings rendererSettings;

        std::string levelName;

        FVector4 colour{};
        FVector4 ambientLight{};

        bool bEditor{};
        ECSManager ecs;

        std::vector<InputAction> inputActionList;
        UIInternal ui;

        /**
         * @note Open the documentation for Levels and opening of levels for more info about this function
         */
        void finalizeOpening() noexcept;
        std::vector<InputAction>& getActions() noexcept;

        std::function<void(void)> openFunction = [=](){};

        friend class Renderer;
        friend class Camera;
        friend class Editor;
        friend class GLPipeline;
        friend class GLEntityManager;
        friend class ECS;
        friend class WindowInternal;
        friend class Input;
        friend class Actor;
        friend class Level;
        friend class UI;
        friend class UIInternal;
        friend class InputActions;
        friend class Math;
        friend class SettingsManager;
        friend class GameInstance;
        friend class Assets;
        friend class StateTracker;
        friend class Utility;
        friend class IDManager;
        friend class VulkanRenderer;
        friend class Events;
        friend class EditorUtilSettings;
        friend class Swapchain;
        friend class Commands;
        friend struct MeshComponentRaw;
        friend struct RendererSettings;
    };

    inline UVKGlobal global;
}