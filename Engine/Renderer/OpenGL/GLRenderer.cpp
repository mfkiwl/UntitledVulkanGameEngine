// GLRenderer.cpp
// Last update 4/15/2021 by Madman10K
#include <GL/glew.h>
#include "GLRenderer.hpp"
#include "../OpenGL/Components/GLMesh.hpp"
#include "../OpenGL/Components/GLCamera.hpp"
#include "../../Core/Events/Events.hpp"
#include "../../GameFramework/GameplayClasses/Level/Level.hpp"
#include "Engine/Core/Core/Registry.hpp"

void UVK::GLRenderer::renderEditor()
{
    ed.runEditor(colour, fb);
}

void UVK::GLRenderer::initEditor() {
    if (bEditor)
    {
        ed.setTheme(colTheme);
        ed.initEditor();
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
}

void UVK::GLRenderer::createWindow(UVK::Level* level) noexcept
{
    GLCamera cm = GLCamera(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 750.0f);
    
    /*unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f
    };
*/
    currentWindow.createWindow();

    logger.consoleLog("Creating geometry", UVK_LOG_TYPE_NOTE);
    

/*
    registry.createActor("Maikati");
    pool.each([&](entt::entity ent){
        if (registry.getComponent<CoreComponent>(ent).name == "Maikati")
        {
            auto& a = registry.addComponent<MeshComponentRaw>(ent);
            a.createMesh(vertices, indices, 20, 12, "../Content/Engine/defaultvshader.gl", "../Content/Engine/defaultfshader.gl", SHADER_IMPORT_TYPE_FILE);

            auto& b = registry.addComponent<AudioComponent3D>(ent);
            b.play("and.wav", true, 1.0f, 1.0f, FVector(15.0f, 0.0f, 0.0f));
        }
    });
    */
    projection = glm::perspective(glm::radians(90.0f), (GLfloat)currentWindow.getBufferWidth() / (GLfloat)currentWindow.getBufferHeight(), 0.1f, 100.0f);
    logger.consoleLog("Compiled Shaders", UVK_LOG_TYPE_SUCCESS);

    initEditor();

    logger.consoleLog("Init editor", UVK_LOG_TYPE_SUCCESS);

    if (!bEditor)
    {
        events.callBegin();
    }
    
    GLfloat deltaTime;
    GLfloat lastTime = 0;
    
    colour = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
    while (!glfwWindowShouldClose(currentWindow.getWindow()))
    {        
        glfwPollEvents();

        auto now = (float)glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        cm.move(deltaTime);
        cm.moveMouse(deltaTime, UVK::Input::getMousePositionChange());

        if (bEditor)
        {
            fb.useFramebuffer();
            glEnable(GL_DEPTH_TEST);
        }
        glClearColor(colour.x, colour.y, colour.z, colour.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (bEditor)
        {
            ed.beginFrame();
        }

        pool.each([&](entt::entity ent){

            if (registry.getComponent<CoreComponent>(ent).name == "Maikati")
            {

                auto& a = registry.getComponent<MeshComponentRaw>(ent);

                //ed.getResources().brick.useTexture();
                a.render(projection, cm);
            }
        });
        glUseProgram(0);

        if (bEditor)
        {
            UVK::GLFrameBuffer::unbindFramebuffer();
            glDisable(GL_DEPTH_TEST);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            level->tick(deltaTime);
            renderEditor();
        }

        glfwSwapBuffers(currentWindow.getWindow());
    }
    events.callEnd();

    ed.destroyContext();

    pool.each([&](entt::entity ent)
    {
        //if (registry.hasComponent<AudioComponent2D>(ent))
        //{
        //    auto& a = registry.getComponent<AudioComponent2D>(ent);
        //    a.stopAudio();
        //}

        //if (registry.hasComponent<AudioComponent3D>(ent))
        //{
        //    auto& a = registry.getComponent<AudioComponent3D>(ent);
        //    a.stopAudio();
        //}

        if (registry.hasComponent<MeshComponentRaw>(ent))
        {
            auto& a = registry.getComponent<MeshComponentRaw>(ent);

            a.clearMesh();
        }
    });
}