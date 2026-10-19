/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/ui/viewer_shell_render.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <iostream>
int main()
{
    if(!glfwInit())return 77;
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    auto* window=glfwCreateWindow(1280,800,"OGL ViewerShell GPU",nullptr,nullptr);
    if(!window){glfwTerminate();return 77;}
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {glfwDestroyWindow(window);glfwTerminate();return 1;}
    using namespace nexverse::ui;
    int error=0;
    {
        UiRenderer renderer;
        if(!renderer.initialize(1280,800))error=2;
        if(!error)
        {
            ViewerShellView view;
            view.avatar_name="NexVerse Test";
            view.region_name="Testregion";
            view.region_connected=true;
            view.terrain_ready=true;
            view.parcel_ready=true;
            view.avatar_present=true;
            view.appearance_ready=true;
            view.objects=200U;
            view.terrain_patches=128U;
            view.remote_avatars=2U;
            view.agent_position={128.0f,128.0f,25.0f};
            view.animations=3U;
            view.attachments=5U;
            view.avatar_positions={{64.0f,120.0f},{160.0f,140.0f}};
            view.chat_lines={"Tester: Hallo NexVerse","Andere: Hallo OGL"};
            view.nearby_objects={{17U,2.8f,"11111111-2222-3333-4444-555555555555"},
                                 {29U,6.2f,"aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"}};

            view.animation_entries={
                {"6ed24bd8-91aa-4b12-ccc7-c97c857ab4e0",4},
                {"aec4610c-757f-bc4e-c092-c6e9caf18daf",6}};
            ViewerShellState state;
            for(const auto panel:{
                ViewerWindow::none,ViewerWindow::chat,ViewerWindow::teleport,
                ViewerWindow::region,ViewerWindow::map,ViewerWindow::avatar,
                ViewerWindow::animations,ViewerWindow::objects,
                ViewerWindow::settings,ViewerWindow::help,ViewerWindow::move})
            {
                glViewport(0,0,1280,800);
                glClearColor(0.0f,0.0f,0.0f,1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                state.window=panel;
                state.open_menu=-1;
                state.chat_draft="Testnachricht";
                render_viewer_shell(renderer,1280,800,state,view);
                if(glGetError()!=GL_NO_ERROR){error=3;break;}
            }
            // SSA with many attachments must also render its F3
            // provenance row, with no unverified geometry claims.
            if(!error)
            {
                state.window=ViewerWindow::avatar;
                state.avatar_bake_page=0U;
                view.avatar_wearables_status="Wearables: Legacy 13/17 Assets (COF offen)";
                view.avatar_self_attachment_packet=true;
                view.attachments=85U;
                render_viewer_shell(renderer,1280,800,state,view);
                if(glGetError()!=GL_NO_ERROR)error=6;
                view.avatar_self_attachment_packet=false;
                view.attachments=0U;
                // Authenticated legacy Low158 has a skin but no
                // AttachmentBlock byte. Keep this real F3 state visible
                // and distinct from a missing AvatarAppearance packet.
                view.appearance_ready=true;
                view.avatar_wearables_status="Wearables: Legacy-Antwort ausstehend";
                render_viewer_shell(renderer,1280,800,state,view);
                if(glGetError()!=GL_NO_ERROR)error=7;
                view.appearance_ready=false;
                render_viewer_shell(renderer,1280,800,state,view);
                if(glGetError()!=GL_NO_ERROR)error=8;
            }
            if(!error)
            {
                state.window=ViewerWindow::none;
                state.open_menu=2;
                render_viewer_shell(renderer,1280,800,state,view);
                if(glGetError()!=GL_NO_ERROR)error=4;
            }
            // Pixel probe inside the in-world menu bar. A blank
            // background would remain black, so this catches missing UI.
            if(!error)
            {
                std::array<unsigned char,4> pixel{};
                glReadPixels(12,800-78,1,1,GL_RGBA,GL_UNSIGNED_BYTE,
                             pixel.data());
                if(glGetError()!=GL_NO_ERROR ||
                   (pixel[0]==0U&&pixel[1]==0U&&pixel[2]==0U))
                    error=5;
            }
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    if(error)std::cerr<<"ViewerShell GL renderer failure "<<error<<"\n";
    return error;
}
