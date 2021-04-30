// Editor.hpp
// Last update 4/28/2021 by Madman10K
#pragma once
#include <iostream>
#include <utility>
#include <Core.hpp>
#include "Widgets/EditorViewport.hpp"
#include "../Textures/Texture.hpp"

namespace UVK
{
    class Editor
    {
    public:
        Editor() = default;

        void initEditor();
        void runEditor(FVector4& colour, GLFrameBuffer& fb);
        static void beginFrame();
        void destroyContext();

        void setTheme(std::string theme)
        {
            colTheme = std::move(theme);
        }
    private:
        Texture folder;
        Texture audioImg;
        Texture model;
        Texture play;
        Texture brick;
        Texture logoTxt;

        int entNum = 0;
        int viewportWidth = 0, viewportHeight = 0;
        short selectedFile = 0;

        Actor selectedEntity;

        bool bShowOpenLevelWidget= false;
        bool bShowSaveLevelWidget = false;
        bool bFinalisedCommand = false;
        bool bShowCreateFile1 = false;
        bool bShowAboutUs = false;
        bool bShowSaveWarning = false;

        void displayEditor(FVector4& colour, GLFrameBuffer& fb);

        std::string levelName;
        std::string openLevel;
        std::string location;
        std::string name;
        std::string fileOutLocation;
        std::string entAppend;
        std::string terminalCommand;
        std::string engineVersion;
        std::string projectVersion;
        std::string projectName;
        std::string colTheme;
    };
}