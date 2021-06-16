// GLRenderer.hpp
// Last update 15/6/2021 by Madman10K
#pragma once
#include <Core.hpp>
#include <utility>
#include "../EditorUI/Editor.hpp"
#include "Components/GLFrameBuffer.hpp"
#include "Engine/Renderer/Window/Window.hpp"
#include "UVKLog.h"

namespace UVK
{
    class Level;

    /**
     * @brief The OpenGL renderer
     */
    class GLRenderer
    {
    public:
        GLRenderer() = delete;

        explicit GLRenderer(Level* level, const bool& bUsesEditor, std::string theme)
        {
            bEditor = bUsesEditor;
            colTheme = std::move(theme);
            start(level);
        }
    private:
        void start(Level* level) const noexcept;

        bool bEditor;
        std::string colTheme;
    };
}