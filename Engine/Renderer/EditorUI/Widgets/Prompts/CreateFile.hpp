#pragma once
#include <Generated/BuildDef.hpp>
#ifndef PRODUCTION
#include <iostream>
#include <Renderer/Textures/Texture.hpp>
#include <imgui.h>


namespace CreateFile
{
    // Creates a file
    bool display(std::string& fileOutLocation, bool& bShowCreateFile1, const std::string& prjname) noexcept;
}
#endif