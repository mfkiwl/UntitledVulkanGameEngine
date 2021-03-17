// GLRenderer.hpp
// Last update 3/17/2021 by Madman10K
#include "Components/GLMesh.hpp"
#include "GLRenderer.hpp"
#include "../../Core/Events/Events.hpp"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "../../GameFramework/GameplayClasses/Level/Level.hpp"
#include "../EditorUI/DetailsPanel.hpp"
#include "../EditorUI/SaveLevel.hpp"
#include "../EditorUI/Filesystem.hpp"
#include "../EditorUI/Statistics.hpp"
#include "../EditorUI/WorldSettings.hpp"

void UVK::GLRenderer::setDarkTheme()
{
    auto& colours = ImGui::GetStyle().Colors;

    colours[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 0.85f };

    colours[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.85f };
    colours[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 0.85f };
    colours[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 0.85f };

    colours[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.85f };
    colours[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 0.85f };
    colours[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 0.85f };

    colours[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.85f };
    colours[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 0.85f };
    colours[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 0.85f };

    colours[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 0.85f };
    colours[ImGuiCol_TabHovered] = ImVec4{ 0.4f, 0.4f, 0.4f, 0.85f };
    colours[ImGuiCol_TabActive] = ImVec4{ 0.3f, 0.3f, 0.3f, 0.85f };
    colours[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 0.85f };
    colours[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.85f };

    colours[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 0.85f };
    colours[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 0.85f };
    colours[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f, 0.15f, 0.95f, 0.85f };

    colours[ImGuiCol_MenuBarBg] = ImVec4{ 0.01, 0.01, 0.01, 0.85f };
}

void UVK::GLRenderer::createWindow(UVK::Level* level) noexcept
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    std::string location;
    std::string name;
    std::string fileOutLocation;

    bSetBuff = true;
    bShowSaveLevelWidget = false;
    bool bShowCreateFile1 = false;
    short selectedFile = 0;

    currentWindow.createWindow();

    logger.consoleLog("Creating geometry", NOTE);
    //GLMesh* mh = new GLMesh();
    //mh->createMesh(vertices, indices, 12, 12);
    //GLMesh* mh2 = new GLMesh();
    //mh2->createMesh(vertices, indices, 12, 12);
    //GLShader* sh = new GLShader();

    MeshComponentRaw ms;

    ms.createMesh(vertices, indices, 12, 12, "../Content/Engine/default.vshader.gl", "../Content/Engine/default.fshader.gl", SHADER_IMPORT_TYPE_FILE);

    GLuint uniformProjection = 0, uniformModel = 0;
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (GLfloat)currentWindow.getBufferWidth() / currentWindow.getBufferHeight(), 1.0f, 100.0f);
    logger.consoleLog("Compiled Shaders", SUCCESS);

#ifndef __MINGW32__
    std_filesystem::path res("../Content/Engine/");

    Texture folder(static_cast<std::string>(res.string() + "folder.png"));
    folder.load();

    Texture audio(static_cast<std::string>(res.string() + "audio.png"));
    audio.load();

    Texture model(static_cast<std::string>(res.string() + "model.png"));
    model.load();

    //sh->createFromFile(static_cast<std::string>(res.string() + "default.vshader.gl").c_str(), static_cast<std::string>(res.string() + "default.fshader.gl").c_str());
#else
    Texture folder(static_cast<std::string>("../Content/Engine/folder.png"));
    folder.load();

    Texture audio(static_cast<std::string>("../Content/Engine/audio.png"));
    audio.load();

    Texture model(static_cast<std::string>("../Content/Engine/model.png"));
    model.load();

    sh->createFromFile("../Content/Engine/default.vshader.gl", "../Content/Engine/default.fshader.gl");
#endif
    if (bEditor)
    {


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigViewportsNoTaskBarIcon = true;


        ImGui::StyleColorsDark();
        ImGui::StyleColorsClassic();


        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        setDarkTheme();
        ImGui_ImplGlfw_InitForOpenGL(currentWindow.getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }



    events.callBegin();
    GLfloat deltaTime = 0;
    GLfloat lastTime = 0;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    while (!glfwWindowShouldClose(currentWindow.getWindow()))
    {
        // Why do I do this here I do not know. I really shouldn't have to do this since it is enabled on window creation
        // but for some reason I have to enable it on tick
        //glEnable(GL_DEPTH_TEST);
        
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (bEditor)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        

        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        events.callTick(deltaTime);

        //uniformProjection = sh->getProjectionLocation();
        //glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        
        Model mat = Model();

        mat.translate(FVector(0.0f, 2.0f, -2.5f));
        mat.rotate(90.0f, FVector(0.0f, 1.0f, 0.0f));
        mat.scale(FVector(1.0f, 1.0f, 1.0f));
        //mat = glm::translate(mat, UVK::FVector(0.0f, 2.0f, -2.5f));
        //mat = glm::rotate(mat, glm::radians(90.0f), UVK::FVector(0.0f, 1.0f, 0.0f));
        //mat = glm::scale(mat, UVK::FVector(1.0f, 1.0f, 1.0f));

        ms.render(projection, mat);

        

        //sh->useShader();
        //uniformModel = sh->getModelLocation();

        
        //glm::mat4 mat(1.0f);

        //mat = glm::translate(mat, glm::vec3(0.0f, 2.0f, -2.5f));
        //mat = glm::rotate(mat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        //mat = glm::scale(mat, glm::vec3(1.0f, 1.0f, 1.0f));
        //glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat));

        //mh->render();

        //glm::mat4 mat2(1.0f);
        //mat2 = glm::translate(mat2, glm::vec3(1.0f, 0.0f, -2.5f));
        
        //mat2 = glm::scale(mat2, glm::vec3(1.0f, 1.0f, 1.0f));
        //glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat2));
        //mh2->render();

        glUseProgram(0);

        level->tick(5);

        if (bEditor)
        {
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            window_flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowBgAlpha(1.0f);

                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;

            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }


            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            bool bIsOpen = true;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &bIsOpen, window_flags);

            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::Button("Save level"))
                    {
                        bShowSaveLevelWidget = true;
                    }

                    if (ImGui::Button("New level"))
                    {
                        pool.clear();
                    }

                    if (ImGui::Button("Open level"))
                    {
                        bShowOpenLevelWidget = true;
                    }

                    if (ImGui::Button("New File"))
                    {
                        bShowCreateFile1 = true;
                    }

                    if (ImGui::Button("Regenerate files"))
                    {
                        system("cd ../UVKBuildTool/build/ && ./UVKBuildTool --generate && cd ../../");
                    }

                    if (ImGui::Button("Exit"))
                    {
                        glfwSetWindowShouldClose(currentWindow.getWindow(), GL_TRUE);
                        std::terminate();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::Button("Undo"))
                    {

                    }

                    if (ImGui::Button("Redo"))
                    {

                    }

                    if (ImGui::Button("Undo history"))
                    {

                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::End();

            if (bShowSaveLevelWidget)
            {
                SaveLevel::display(bShowSaveLevelWidget, location, name);
            }

            if (bShowOpenLevelWidget)
            {
                ImGui::Begin("Open Level");
                char buffer[256];

                ImGui::InputText("File location", buffer, 256);
                if (ImGui::Button("Cancel"))
                {
                    bShowOpenLevelWidget = false;
                }

                ImGui::SameLine();
                if (ImGui::Button("Submit"))
                {
                    UVK::Level::open(buffer);
                    bShowOpenLevelWidget = false;
                }


                ImGui::End();
            }

            if (bShowCreateFile1)
            {
                ImGui::Begin("Create a file");

                if (ImGui::Selectable("Game mode")) selectedFile = 1;
                if (ImGui::Selectable("Game state")) selectedFile = 2;
                if (ImGui::Selectable("Player state")) selectedFile = 3;
                if (ImGui::Selectable("Player controller")) selectedFile = 4;
                if (ImGui::Selectable("Pawn")) selectedFile = 5;
                if (ImGui::Selectable("Game instance")) selectedFile = 6;
                if (ImGui::Selectable("Level")) selectedFile = 7;

                ImGui::InputTextWithHint("Location##genfile", "Location starts from your Source folder!", &fileOutLocation);

                if (ImGui::Button("Create"))
                {
                    switch(selectedFile)
                    {
                        case 1:
                            system(static_cast<std::string>("cd ../UVKBuildTool/build && ./UVKBuildTool --game-mode " + fileOutLocation + "cd ../../").c_str());
                            break;
                        case 2:
                            system(static_cast<std::string>("cd ../UVKBuildTool/build && ./UVKBuildTool --game-state " + fileOutLocation + "cd ../../").c_str());
                            break;
                        case 3:
                            system(static_cast<std::string>("cd ../UVKBuildTool/build && ./UVKBuildTool --player-state " + fileOutLocation + "cd ../../").c_str());
                            break;
                        case 4:
                            system(static_cast<std::string>("cd ../UVKBuildTool/build && ./UVKBuildTool --player-controller " + fileOutLocation + "cd ../../").c_str());
                            break;
                        case 5:
                            system(static_cast<std::string>("cd ../UVKBuildTool/build && ./UVKBuildTool --pawn " + fileOutLocation + "cd ../../").c_str());
                            break;
                        case 6:
                            system(static_cast<std::string>("cd ../UVKBuildTool/build && ./UVKBuildTool --game-instance " + fileOutLocation + "cd ../../").c_str());
                            break;
                        case 7:
                            system(static_cast<std::string>("cd ../UVKBuildTool/build && ./UVKBuildTool --level " + fileOutLocation + "cd ../../").c_str());
                            break;
                        default:
                            break;
                    }

                    selectedFile = 0;
                    bShowCreateFile1 = false;
                    fileOutLocation = "";
                }
                if (ImGui::Button("Cancel"))
                {
                    selectedFile = 0;
                    bShowCreateFile1 = false;
                    fileOutLocation = "";
                }

                ImGui::End();
            }

            {
                ImGui::Begin("Scene Hierarchy");

                pool.each([&](entt::entity ent)
                {
                    auto& a = registry.getComponent<CoreComponent>(ent);

                    if (ImGui::Selectable(a.name.c_str()))
                    {
                        selectedEntity = ent;
                    }
                });

                ImGui::End();
            }

            {
                DetailsPanel::display(selectedEntity);
            }

            {
                Filesystem::display(folder, audio, model);
            }

            {
                ImGui::Begin("Toolbar");
                ImGui::Text("Coming soon!");
                ImGui::End();
            }

            {
                ImGui::Begin("Tools");

                ImGui::Text("Coming soon!");

                ImGui::End();
            }

            {
                Statistics::display();
            }

            {
                WorldSettings::display(FVector4(1.0f, 1.0f, 1.0f, 1.0f), FVector4(1.0f, 1.0f, 1.0f, 1.0f), "a");
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }

        glfwSwapBuffers(currentWindow.getWindow());
    }
    events.callEnd();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    currentWindow.destroyWindow();

    std::terminate();
}

/*void UVK::GLRenderer::createTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

/*
void UVK::GLRenderer::addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = static_cast<std::string>(shaderCode).length();

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar ErrorLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, 1024, NULL, ErrorLog);
        logger.consoleLogComplex<GLchar*>("Error compiling the a shader:", ERROR, {ErrorLog});
        return;
    }

    glAttachShader(theProgram, theShader);
}

void UVK::GLRenderer::compileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        logger.consoleLog("Failed to create shader", ERROR);
        return;
    }

    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(ErrorLog), NULL, ErrorLog);
        logger.consoleLogComplex<GLchar*>("Error linking program:", ERROR, {ErrorLog});
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(ErrorLog), NULL, ErrorLog);
        logger.consoleLogComplex<GLchar*>("Error validating program:", ERROR, {ErrorLog});
        return;
    }
}
*/