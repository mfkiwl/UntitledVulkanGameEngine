#pragma once
#include <any>
#include "Generated/BuildDef.hpp"
#ifdef _WIN32
    #ifdef UVK_LIB_COMPILE
        #define UVK_PUBLIC_API __declspec(dllexport)
    #else
        #define UVK_PUBLIC_API __declspec(dllimport)
    #endif
#else
    #define UVK_PUBLIC_API
#endif
#include <logger/UVKLog.h>

// This is for MinGW32 because it is retarded
#ifndef __MINGW32__
    #if __has_include(<filesystem>)
        #include <filesystem>
        #define std_filesystem std::filesystem
    #else
        #include <experimental/filesystem>
        #define std_filesystem std::experimental::filesystem
    #endif
#else
    #error "Compiling against MinGW is not supported!"
#endif

#ifdef PRODUCTION
    #define UVK_START(x) std::ios_base::sync_with_stdio(!(x)); \
    logger.setCrashOnError(true); \
    UVK::Utility::removeConsole()
#else
    #define UVK_START(x) std::ios_base::sync_with_stdio(!(x))
#endif


#ifdef PRODUCTION
    // TODO: Make it so that it refers to the archive!
    #define UVK_CONTENT_PATH "../Content/"
#else
    #define UVK_CONTENT_PATH "../Content/"
#endif
#define UVK_CONFIG_ENGINE_PATH "../Config/Engine/"
#define UVK_CONFIG_SETTINGS_PATH "../Config/Settings/"
#define UVK_CONFIG_PRJ_PATH "../"

#define FS_ICON_AUDIO 0
#define FS_ICON_IMAGE 1
#define FS_ICON_VIDEO 2
#define FS_ICON_FOLDER 3
#define FS_ICON_FONT 4
#define FS_ICON_MODEL 5
#define FS_ICON_UNKNOWN 6
#define FS_ICON_CODE 7
#define FS_ICON_CLOSE 8

#define FC_GAME_MODE 1
#define FC_GAME_STATE 2
#define FC_PLAYER_STATE 3
#define FC_PLAYER_CONTROLLER 4
#define FC_PAWN 5
#define FC_LEVEL 6
#define FC_SCRIPTABLE_OBJECT 7

#define VK_SHADER_STAGE_VERT 0
#define VK_SHADER_STAGE_FRAG 1

#define VK_UNDEFINED_TO_SUBPASS_LAYOUT 0
#define VK_SUBPASS_TO_PRESENTATION_LAYOUT 1

#define VK_MAX_CONCURRENT_IMAGE_DRAW 2

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define COMPONENT_MESH_RAW 0
#define COMPONENT_MESH 1
#define COMPONENT_AUDIO 2
//#define GLEW_STATIC