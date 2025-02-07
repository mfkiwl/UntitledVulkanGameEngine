#pragma once
#include <Core.hpp>
#ifndef PRODUCTION
#include <iostream>
#include <Renderer/Textures/Texture.hpp>

namespace OpenLevelWidget
{
    // Opens a level
    bool display(UVK::FString& openLevel, bool& bShowOpenLevelWidget, double& dr, UVK::FVector4& colour) noexcept;
}
#endif