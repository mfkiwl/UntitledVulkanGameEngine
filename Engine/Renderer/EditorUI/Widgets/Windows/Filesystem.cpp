#include <GL/glew.h>
#include "Filesystem.hpp"
#ifndef PRODUCTION
#include "Assets/Asset.hpp"
#include <imgui.h>
#include <cpp/imgui_stdlib.h>
#include <State/StateTracker.hpp>
#include <Renderer/EditorUI/EditorUtils/Structs.hpp>

#ifndef __MINGW32__
// ---------------------------------------------------------------------------------------------------------------------
// ------------------------------------------- Begin Macro definition section ------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
#define HANDLE_MOUSE_BUTTONS                                                                        \
            if (UVK::Input::getKey(Keys::MouseButtonLeft) == Keys::KeyPressed)                      \
            {                                                                                       \
                if (UVK::Input::getAction("editor-bind-modifier") == Keys::KeyPressed)              \
                {                                                                                   \
                    if (bFileSelected)                                                              \
                    {                                                                               \
                        if (it != selectedFiles.end())                                              \
                            selectedFiles.erase(it);                                                \
                        else                                                                        \
                            currentSelectedFile.clear();                                            \
                    }                                                                               \
                    else                                                                            \
                        selectedFiles.push_back(path);                                              \
                }                                                                                   \
                else                                                                                \
                {                                                                                   \
                    currentSelectedFile = path;                                                     \
                    selectedFiles.clear();                                                          \
                }                                                                                   \
            }                                                                                       \
            else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))                                 \
            {                                                                                       \
                ImGui::OpenPopup("##FSEditPopup");                                                  \
                selectedEditFile = path;                                                            \
            }                                                                                       \

#define DOUBLE_POP                                                                                  \
            if (bFileSelected)                                                                      \
            {                                                                                       \
                ImGui::PopStyleColor();                                                             \
                ImGui::PopStyleColor();                                                             \
            }                                                                                       \

#define CREATE_FILE(x, y)                                                                           \
        if (ImGui::MenuItem(x))                                                                     \
        {                                                                                           \
            y(pt);                                                                                  \
            bNewFolder = true;                                                                      \
            return bReturn;                                                                         \
        }                                                                                           \

void clearSelectedFilesAndSubmitTransaction(UVK::Transaction& transaction, std_filesystem::path& pt, const std_filesystem::path& pt2, volatile bool& bNewFolder, std::vector<std_filesystem::path>& selectedFiles, std_filesystem::path& currentSelectedFile) noexcept;
auto applyColourToSelectedFiles(std::vector<std_filesystem::path>& selectedFiles, const std_filesystem::path& currentSelectedFile, const std_filesystem::path& path, bool& bFileSelected) noexcept
{
    // TODO: We might want to fix this
    auto it = std::find(selectedFiles.begin(), selectedFiles.end(), path);
    bFileSelected = (it != selectedFiles.end());
    if (bFileSelected || currentSelectedFile == path)
    {
        bFileSelected = true; // Needed so that we can pop the colours
        ImGui::PushStyleColor(ImGuiCol_Button, { 0.87f, 0.64f, 0.03, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 0.94f, 0.30f, 1.0f });
    }
    return it;
}
// ---------------------------------------------------------------------------------------------------------------------
// ------------------------------------------- End Macro Definition section --------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

bool Filesystem::display(std_filesystem::path& pt, UVK::Texture* textures, UVK::EditorSettings::FilesystemWidgetData& data, bool& bShow) noexcept
{
// ---------------------------------------------------------------------------------------------------------------------
// ------------------------------------------- Begin variable section --------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

    bool bReturn = false;

    // Variables for the UI
    float cellSize = data.padding + data.imageSize;
    int columns;

    // Variables for previews and folder refresh
    volatile static bool bCurrentlyUsingPreviews = false;
    volatile static bool bNewFolder = true;
    static unsigned int fileNum = 0;
    static std::vector<UVK::Texture> previews;

    // CTRL/SHIFT select
    static std::vector<std_filesystem::path> selectedFiles;
    static std_filesystem::path currentSelectedFile;

    // Right click popup
    static std_filesystem::path selectedEditFile;
    static bool bCalledFromEditPopup = false;

    // Variables for renaming
    static bool bRenaming = false;
    static UVK::FString renameText;

    // Warning for when deleting a folder specifically
    static bool bDeleteWarning = false;
// ---------------------------------------------------------------------------------------------------------------------
// ------------------------------------------- End variable section ----------------------------------------------------
// ---------------------------------------- Begin initialization section -----------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

    if (bNewFolder)
    {
        bCurrentlyUsingPreviews = false;
        fileNum = 0;
        for (auto& a : std_filesystem::directory_iterator(pt))
            fileNum++;

        for (auto& a : previews)
            a.destroy();
        previews.clear();
    }

    if (data.bUsePreviews)
        bCurrentlyUsingPreviews = fileNum >= data.maxFileNum ? false : true;

    if (bCurrentlyUsingPreviews && (previews.size() != data.maxFileNum || bNewFolder))
        previews.resize(data.maxFileNum);
// ---------------------------------------------------------------------------------------------------------------------
// ----------------------------------------- End initialization section ------------------------------------------------
// -------------------------------------------- Begin Menubar section --------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

    ImGui::Begin("Filesystem##Widget", &bShow, ImGuiWindowFlags_MenuBar);

    ImGui::BeginMenuBar();

    CREATE_FILE("+ Add File", createFile)
    if (ImGui::MenuItem("- Remove File") || (UVK::Input::getKey(Keys::Delete) == Keys::KeyPressed && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)))
    {
        bDeleteWarning = true;
        bCalledFromEditPopup = false;
    }
    CREATE_FILE("+ Add Directory", createFolder)

    if (ImGui::MenuItem("* Rename File") || (UVK::Input::getAction("editor-filesystem-rename") == Keys::KeyPressed && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !currentSelectedFile.empty()))
    {
        bRenaming = true;
        bCalledFromEditPopup = false;
        renameText = currentSelectedFile.filename().string();
    }

    if (ImGui::MenuItem("+ 2x Duplicate") || (UVK::Input::getAction("editor-bind-modifier") == Keys::KeyPressed && (UVK::Input::getKey(Keys::C) == Keys::KeyPressed || UVK::Input::getKey(Keys::D) == Keys::KeyPressed) && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !currentSelectedFile.empty()))
    {
        try
        {
            for (auto& a : selectedFiles)
                std_filesystem::copy(a, std_filesystem::path(a.string() + "Copy"));
            std_filesystem::copy(currentSelectedFile, std_filesystem::path(currentSelectedFile.string() + "Copy"));
        }
        catch (std_filesystem::filesystem_error&){}
    }
    ImGui::Separator();
    ImGui::Text("%s", pt.string().c_str());
    ImGui::EndMenuBar();
// ---------------------------------------------------------------------------------------------------------------------
// --------------------------------------------- End Menubar section ---------------------------------------------------
// -------------------------------------------- Begin Warning section --------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
    if (bDeleteWarning)
    {
        if (!ImGui::IsPopupOpen("Warning##FileDeletion"))
            ImGui::OpenPopup("Warning##FileDeletion");
        if (ImGui::BeginPopupModal("Warning##FileDeletion", &bDeleteWarning))
        {
            ImGui::TextWrapped("You are trying to delete a file. Deleting a file can be undone, however folders will not be! Files are also not stored in a recycle bin, and are stored in memory, until the end of the application!");
            static bool bAccepted = false;

            ImGui::TextWrapped("I am aware of this!");
            ImGui::SameLine();
            ImGui::Checkbox("##Accepted", &bAccepted);

            if (ImGui::Button("Close##FiledDeletion"))
                bDeleteWarning = false;
            ImGui::SameLine();
            if (ImGui::Button("Delete##FileDeletion") && bAccepted)
            {
                if (bCalledFromEditPopup)
                    deleteFile(pt, selectedEditFile);
                else
                {
                    if (!currentSelectedFile.empty())
                        deleteFile(pt, currentSelectedFile);
                    for (auto& a : selectedFiles)
                        if (!a.empty())
                            deleteFile(pt, a);
                }
                currentSelectedFile.clear();
                selectedFiles.clear();
                bNewFolder = true;
                bDeleteWarning = false;
                bAccepted = false;

                return bReturn;
            }

            ImGui::EndPopup();
        }
    }
// ---------------------------------------------------------------------------------------------------------------------
// --------------------------------------------- End Warning section ---------------------------------------------------
// ------------------------------------------- Begin Explorer section --------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
    ImGui::BeginChild("Explorer");

    // Decide on the number of columns
    columns = (int)(ImGui::GetContentRegionAvail().x / cellSize);
    // If columns are less than one set them to 1 otherwise continue
    columns = columns < 1 ? 1 : columns;

    // Create the columns for the elements
    ImGui::Columns(columns, nullptr, false);

    auto p = UVK::FString(pt.string());
    UVK::Utility::sanitiseFilepath(p, true);
    pt = std_filesystem::path(p.c_str());

    // ---- Begin ../ button ----
    ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
    if (!(p == "../Content/" || p == "../Content"))
    {
        ImGui::ImageButton((void*)(intptr_t)textures[UVK::EditorTextures::FS_ICON_FOLDER].getImage(), ImVec2(data.imageSize, data.imageSize));
        if (ImGui::BeginDragDropTarget())
        {
            //const ImGuiPayload* payload;
            if (ImGui::AcceptDragDropPayload("ENGINE_FS_WIDGET_LVL") || ImGui::AcceptDragDropPayload("ENGINE_FS_WIDGET_ALL"))
            {
                try
                {
                    // Copy the selected files one directory up, then
                    for (auto& f : selectedFiles)
                    {
                        std_filesystem::copy(f, f.parent_path() / ".." / f.filename());
                        std_filesystem::remove_all(f);
                    }
                    std_filesystem::copy(currentSelectedFile, currentSelectedFile.parent_path() / ".." / currentSelectedFile.filename());
                    std_filesystem::remove_all(currentSelectedFile);
                }
                catch (std_filesystem::filesystem_error&){}
            }
            // TODO: Go back to commit 44f40ca if there are any problems here!
            ImGui::EndDragDropTarget();
        }
        // Double-click on the ".." button makes us go one directory up
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            // Submit transaction for undo/redo
            UVK::Transaction transaction =
            {
                .undofunc = [](UVK::TransactionPayload& payload)
                {
                    *payload.path = std_filesystem::path(payload.coreComponent.name.c_str());
                    *payload.vbChanged = true;
                },
                .redofunc = [](UVK::TransactionPayload& payload)
                {
                    *payload.path = std_filesystem::path(payload.coreComponent.devName.c_str());
                    *payload.vbChanged = true;
                },
                .transactionPayload =
                {
                    .coreComponent =
                    {
                        .name = pt.string(),
                        .devName = pt.parent_path().string()
                    },
                    .vbChanged = &bNewFolder,
                    .path = &pt
                }
            };
            clearSelectedFilesAndSubmitTransaction(transaction, pt, pt.parent_path(), bNewFolder, selectedFiles, currentSelectedFile);
            return bReturn;
        }

        ImGui::TextWrapped("../");
        ImGui::NextColumn();
    }
    // ---- End ../ button ----
    // ---- Begin iterative section ----
    int i = 0;
    for (auto& a : std_filesystem::directory_iterator(pt))
    {
        i++;
        auto& path = a.path();
        ImGui::PushID(path.string().c_str());
        if (a.is_directory())
        {
            bool bFileSelected;
            auto it = applyColourToSelectedFiles(selectedFiles, currentSelectedFile, path, bFileSelected);
            ImGui::ImageButton((void*)(intptr_t)textures[UVK::EditorTextures::FS_ICON_FOLDER].getImage(), ImVec2(data.imageSize, data.imageSize));
            if (ImGui::BeginDragDropTarget())
            {
                if (ImGui::AcceptDragDropPayload("ENGINE_FS_WIDGET_LVL") || ImGui::AcceptDragDropPayload("ENGINE_FS_WIDGET_ALL"))
                {
                    try
                    {
                        for (auto& f : selectedFiles)
                        {
                            std_filesystem::copy(f, path);
                            std_filesystem::remove_all(f);
                        }
                        std_filesystem::copy(currentSelectedFile, path);
                        std_filesystem::remove_all(currentSelectedFile);
                    }
                    catch (std_filesystem::filesystem_error&){}
                }
                // TODO: Go back to commit 44f40ca if there are any problems here!
                ImGui::EndDragDropTarget();
            }
            if (ImGui::IsItemHovered())
            {
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    UVK::Transaction transaction =
                    {
                        .undofunc = [](UVK::TransactionPayload& payload)
                        {
                            *payload.path = std_filesystem::path(payload.coreComponent.name.c_str());
                            *payload.vbChanged = true;
                        },
                        .redofunc = [](UVK::TransactionPayload& payload)
                        {
                            *payload.path = std_filesystem::path(payload.coreComponent.devName.c_str());
                            *payload.vbChanged = true;
                        },
                        .transactionPayload =
                        {
                            .coreComponent =
                            {
                                .name = pt.string(),
                                .id = 330,
                                .devName = (pt/path.filename()).string()
                            },
                            .vbChanged = &bNewFolder,
                            .path = &pt
                        }
                    };
                    clearSelectedFilesAndSubmitTransaction(transaction, pt, (pt / path.filename()),bNewFolder, selectedFiles, currentSelectedFile);
                    DOUBLE_POP
                    return bReturn;
                }
                else HANDLE_MOUSE_BUTTONS
            }
            DOUBLE_POP
        }
        else
        {
            bool bFileSelected;
            auto it = applyColourToSelectedFiles(selectedFiles, currentSelectedFile, path, bFileSelected);
            ImGui::ImageButton((void*)(intptr_t)(selectTextures(textures, a, previews, bCurrentlyUsingPreviews, i, bNewFolder)->getImage()), ImVec2(data.imageSize, data.imageSize));
            if (ImGui::IsItemHovered())
            {
                HANDLE_MOUSE_BUTTONS
            }
            DOUBLE_POP
        }
// ---------------------------------------------------------------------------------------------------------------------
// -------------------------------------------- End Action subsection --------------------------------------------------
// -------------------------------------------- Begin Popup subsection -------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
        if (ImGui::BeginDragDropSource())
        {
            if (!a.is_directory())
                ImGui::Image((void*)(intptr_t)(selectTextures(textures, a, previews, bCurrentlyUsingPreviews, i, bNewFolder)->getImage()), ImVec2(data.imageSize, data.imageSize));
            else
                ImGui::Image((void*)(intptr_t)textures[UVK::EditorTextures::FS_ICON_FOLDER].getImage(), ImVec2(data.imageSize, data.imageSize));
            ImGui::Text("%s", path.string().c_str());
            if (path.extension() == ".uvklevel")
                ImGui::SetDragDropPayload("ENGINE_FS_WIDGET_LVL", path.string().c_str(), path.string().size() - strlen(".uvklevel"), ImGuiCond_Once);
            else
                ImGui::SetDragDropPayload("ENGINE_FS_WIDGET_ALL", path.string().c_str(), path.string().size(), ImGuiCond_Once);

            ImGui::EndDragDropSource();
        }
        if (selectedEditFile == path && ImGui::BeginPopup("##FSEditPopup"))
        {
            CREATE_FILE("+ New Folder##FSEditorPopup", createFolder);
            CREATE_FILE("+ New File##FSEditorPopup", createFile);
            ImGui::Separator();

            if (ImGui::MenuItem("- Delete##FSEditorPopup"))
            {
                bDeleteWarning = true;
                bCalledFromEditPopup = true;
            }

            if (ImGui::MenuItem("* Rename##FSEditorPopup"))
            {
                bRenaming = true;
                bCalledFromEditPopup = true;
                renameText = selectedEditFile.filename().string();
            }

            if (ImGui::MenuItem("+ 2x Duplicate##FSEditorPopup"))
            {
                try
                {
                    std_filesystem::copy(selectedEditFile, std_filesystem::path(selectedEditFile.string() + "Copy"));
                }
                catch (std_filesystem::filesystem_error&){}
                return bReturn;
            }
            ImGui::EndPopup();
        }
// ---------------------------------------------------------------------------------------------------------------------
// --------------------------------------------- End Popup subsection --------------------------------------------------
// ------------------------------------------- Begin Rename subsection -------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

        if (bRenaming)
        {
            if ((bCalledFromEditPopup && a.path() == selectedEditFile) || (!bCalledFromEditPopup && a.path() == currentSelectedFile))
            {
                if (ImGui::InputText(("##Rename" + a.path().string()).c_str(), &renameText) || ImGui::IsItemActive())
                    bReturn = true;
                // I don't think it's useful to set it as any other key really
                if (UVK::Input::getKey(Keys::Enter) == Keys::KeyPressed)
                {
                    std_filesystem::rename(path.parent_path()/path.filename(), path.parent_path()/renameText.c_str());

                    bRenaming = false;
                    bNewFolder = true;

                    return bReturn;
                }
            }
            else
                // path.filename().c_str() shows only the first letter of a filename on Windows for some reason
                ImGui::TextWrapped("%s", path.filename().string().c_str());
        }
        else
            // path.filename().c_str() shows only the first letter of a filename on Windows for some reason
            ImGui::TextWrapped("%s", path.filename().string().c_str());

        ImGui::NextColumn();
        ImGui::PopID();
    }
// ---------------------------------------------------------------------------------------------------------------------
// -------------------------------------------- End Explorer section ---------------------------------------------------
// ------------------------------------------- Begin Settings section --------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
    // For some reason the neat popups that we tried to add when right-clicking on empty space after everything else is rendered
    // conflict with the other popups, also true fo SceneHierarchy
    // TODO: Fix someday
    //if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !ImGui::IsPopupOpen("##FSEditPopup") && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
    //    ImGui::OpenPopup("##FSEditPopup2");
    //if (ImGui::BeginPopup("##FSEditPopup2"))
    //{
    //    if (ImGui::MenuItem("New Folder##FSEditorPopupA"))
    //    {
    //        createFolder(pt);
    //        bNewFolder = true;
    //        return bReturn;
    //    }
    //
    //    if (ImGui::MenuItem("New File##FSEditorPopupA"))
    //    {
    //        createFile(pt);
    //        bNewFolder = true;
    //        return bReturn;
    //    }
    //    ImGui::EndPopup();
    //}
    bNewFolder = false;
    ImGui::PopStyleColor();
    ImGui::Columns(1);

    ImGui::EndChild();
    ImGui::BeginGroup();
    ImGui::Separator();
    ImGui::Columns(4, nullptr, false);

    ImGui::TextWrapped("Image size");
    ImGui::SameLine();
    ImGui::SliderFloat("##Image size", &data.imageSize, 1.0f, 256.0f);
    ImGui::NextColumn();

    ImGui::TextWrapped("Padding");
    ImGui::SameLine();
    ImGui::SliderFloat("##Padding", &data.padding, 20.0f, 256.0f);
    ImGui::NextColumn();

    ImGui::TextWrapped("Max Previews");
    if (ImGui::IsItemHovered())
    {
        ImGui::SetNextWindowSize(ImVec2(300.0f, 55.0f));
        ImGui::BeginTooltip();
        ImGui::TextWrapped("If there are more files in the directory than the integer, then previews are not loaded!");
        ImGui::EndTooltip();
    }
    ImGui::SameLine();
    ImGui::SliderInt("##MaxImages", reinterpret_cast<int*>(&data.maxFileNum), 0, 1024);
    ImGui::NextColumn();

    ImGui::SetColumnWidth(3, 150.0f);
    ImGui::TextWrapped("Previews?");
    ImGui::SameLine();
    ImGui::Checkbox("##Display preview images", &data.bUsePreviews);

    ImGui::Columns(1);
    ImGui::EndGroup();
    ImGui::End();
    return bReturn;
// ---------------------------------------------------------------------------------------------------------------------
// --------------------------------------------- End Settings section---------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
}

void Filesystem::createFile(const std_filesystem::path &pt) noexcept
{
    if (std_filesystem::exists(pt/"NewFile.txt"))
    {
        int i = 0;
        while (std_filesystem::exists(pt/("NewFile" + std::to_string(i) + ".txt")))
            i++;

        std::ofstream out(pt/("NewFile" + std::to_string(i) + ".txt"));
        out << std::endl;
        out.close();

        UVK::Transaction transaction =
        {
            .undofunc = [](UVK::TransactionPayload& payload)
            {
                if (std_filesystem::exists(payload.coreComponent.name.c_str()))
                    remove(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .redofunc = [](UVK::TransactionPayload& payload)
            {
                std::ofstream out(payload.coreComponent.name.c_str());
                out << std::endl;
                out.close();
            },
            .transactionPayload =
            {
                .coreComponent =
                {
                    .name = (pt/("NewFile" + std::to_string(i) + ".txt")).string()
                }
            }
        };
        UVK::StateTracker::push(transaction);
    }
    else
    {
        std::ofstream out(pt/"NewFile.txt");
        out << std::endl;
        out.close();

        UVK::Transaction transaction =
        {
            .undofunc = [](UVK::TransactionPayload& payload)
            {
                if (std_filesystem::exists(payload.coreComponent.name.c_str()))
                    remove(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .redofunc = [](UVK::TransactionPayload& payload)
            {
                std::ofstream out(payload.coreComponent.name.c_str());
                out << std::endl;
                out.close();
            },
            .transactionPayload
            {
                .coreComponent =
                {
                    .name = (pt/"NewFile.txt").string()
                }
            }

        };
        UVK::StateTracker::push(transaction);
    }
}

void Filesystem::createFolder(const std_filesystem::path& pt) noexcept
{
    if (std_filesystem::exists(pt/"NewFolder"))
    {
        int i = 0;
        while (std_filesystem::exists(pt/("NewFolder" + std::to_string(i))))
            i++;

        create_directory(std_filesystem::path(pt/("NewFolder" + std::to_string(i))));

        UVK::Transaction transaction =
        {
            .undofunc = [](UVK::TransactionPayload& payload)
            {
                remove_all(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .redofunc = [](UVK::TransactionPayload& payload)
            {
                if (!std_filesystem::exists(payload.coreComponent.name.c_str()))
                    std_filesystem::create_directory(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .transactionPayload
            {
                .coreComponent =
                {
                    .name = (pt/("NewFolder" + std::to_string(i))).string()
                }
            }
        };
        UVK::StateTracker::push(transaction);
    }
    else
    {
        create_directory(pt/"NewFolder");

        UVK::Transaction transaction =
        {
            .undofunc = [](UVK::TransactionPayload& payload)
            {
                remove_all(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .redofunc = [](UVK::TransactionPayload& payload)
            {
                if (!std_filesystem::exists(payload.coreComponent.name.c_str()))
                    std_filesystem::create_directory(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .transactionPayload =
            {
                .coreComponent =
                {
                    .name = (pt/"NewFolder").string()
                }
            }
        };
        UVK::StateTracker::push(transaction);
    }
}

void Filesystem::deleteFile(std_filesystem::path& pt, std_filesystem::path& selectedFile) noexcept
{
    if (is_directory(selectedFile))
    {
        // TODO: Add a warning here
        //create_directory(std_filesystem::path("../Generated/EditorFS")/selectedFile.filename());
        //copy(selectedFile, std_filesystem::path("../Generated/EditorFS")/selectedFile.filename(), std_filesystem::copy_options::recursive);
        remove_all(selectedFile);
        UVK::Transaction transaction =
        {
            .undofunc = [](UVK::TransactionPayload& payload)
            {
                //if (!act.valid())
                // TODO: Recreate the whole directory structure
                std_filesystem::create_directory(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .redofunc = [](UVK::TransactionPayload& payload)
            {
                std_filesystem::remove_all(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .transactionPayload =
            {
                .coreComponent =
                {
                    .name = selectedFile.string(),
                    .id = 330,
                    .devName = "EngineFilesystemFolderDelete"
                },
                .deltaCoreComponent
                {

                }
            }
        };
        UVK::StateTracker::push(transaction);
    }
    else
    {
        std::ifstream in(selectedFile);
        std::string buf;
        std::string temp;

        while (std::getline(in, temp))
            buf.append(temp + '\n');

        remove(selectedFile);

        UVK::Transaction transaction =
        {
            .undofunc = [](UVK::TransactionPayload& payload)
            {
                std::ofstream out(std_filesystem::path(payload.coreComponent.name.c_str()));
                out << payload.coreComponent.devName << std::flush;
                out.close();
            },
            .redofunc = [](UVK::TransactionPayload& payload)
            {
                std_filesystem::remove(std_filesystem::path(payload.coreComponent.name.c_str()));
            },
            .transactionPayload =
            {
                .coreComponent =
                {
                    .name = selectedFile.string(),
                    .devName = buf
                }
            }
        };
        UVK::StateTracker::push(transaction);
    }

    selectedFile.clear();
}

UVK::Texture* Filesystem::selectTextures(UVK::Texture* textures, const std_filesystem::path& path, std::vector<UVK::Texture>& previews, volatile bool& bPreviews, const int& currentIndex, volatile bool& bLoad) noexcept
{
    // Constants for the file extensions
    static constexpr const char* audioExtensions[] = { ".wav", ".flac", ".m4a", ".ogg", ".mp3" };
    static constexpr const char* imageExtensions[] = { ".jpeg", ".jpg", ".tiff", ".gif", ".bmp", ".png", ".tga", ".psd", ".pic" };
    static constexpr const char* videoExtensions[] = { ".mov", ".m4v", ".mp4", ".mpeg", ".mkv", ".mpg", ".wmv", ".webm" };
    static constexpr const char* objExtensions[] = { ".obj", ".fbx", ".glb", ".gltf" };
    static constexpr const char* codeExtensions[] = { ".yaml", ".uvklevel", ".yml", ".glsl", ".vshader.glsl", ".fshader.glsl", ".vshader", ".fshader" };

    for (const auto& b : audioExtensions)
        if (path.filename().extension().string() == b)
            return &textures[UVK::EditorTextures::FS_ICON_AUDIO];

    for (const auto& b : imageExtensions)
    {
        if (path.filename().extension().string() == b)
        {
            if (bPreviews)
            {
                if (bLoad)
                {
                    previews[currentIndex] = UVK::Texture(path.string());
                    previews[currentIndex].loadImgui();
                    return &previews[currentIndex];
                }
                return &previews[currentIndex];
            }
            else return &textures[UVK::EditorTextures::FS_ICON_IMAGE];
        }
    }

    for (const auto& b : videoExtensions)
        if (path.filename().extension().string() == b)
            return &textures[UVK::EditorTextures::FS_ICON_VIDEO];

    for (const auto& b : objExtensions)
        if (path.filename().extension().string() == b)
            return &textures[UVK::EditorTextures::FS_ICON_MODEL];

    for (const auto& b : codeExtensions)
        if (path.filename().extension().string() == b)
            return &textures[UVK::EditorTextures::FS_ICON_CODE];

    if (path.filename().extension().string() == ".ttf")
        return &textures[UVK::EditorTextures::FS_ICON_CODE];

    return &textures[UVK::EditorTextures::FS_ICON_UNKNOWN];
}

void clearSelectedFilesAndSubmitTransaction(UVK::Transaction& transaction, std_filesystem::path& pt, const std_filesystem::path& pt2, volatile bool& bNewFolder, std::vector<std_filesystem::path>& selectedFiles, std_filesystem::path& currentSelectedFile) noexcept
{
    UVK::StateTracker::push(transaction);
    pt = pt2;
    bNewFolder = true;
    selectedFiles.clear();
    currentSelectedFile.clear();
}
#endif
#endif