
// The MIT License (MIT)

// Copyright (c) 2018 David Kennedy

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#include "DebugUI.h"
#include "PreInit.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "logger/Logger.h"
#ifdef NOVA_WINDOWS_PLATFORM
//#    include "windows/FileSystem.h"
#    include "windows/Window.h"
#elif defined NOVA_LINUX_PLATFORM
//#    include "linux/FileSystem.h"
#    include "linux/Window.h"
#endif
#ifdef NOVA_OPENGL
#    include "graphics/opengl/GraphicsSystem.h"
#endif
#include "Application.h"
namespace Nova
{
void DebugUI::startUp()
{
    LOG_INFO("Initiating DebugUI subsystem");
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO &io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(Window::getInstance().mGLFWindow, false);
    const char *glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple
    // fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the
    // font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in
    // your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture
    // when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below
    // will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to
    // write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("./Dependencies/imgui/misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("./Dependencies/imgui/misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("./Dependencies/imgui/misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("./Dependencies/imgui/misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL,
    // io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);
}
void DebugUI::shutDown()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugUI::drawGUI()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui
    // wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main
    // application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main
    // application. Generally you may always pass all inputs to dear imgui, and hide them from
    // your application based on those two flags.
    // ImGuiIO &io = ImGui::GetIO();
    // io.WantCaptureKeyboard = true;
    // io.WantCaptureMouse = true;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        // static float f = 0.0f;
        // static int counter = 0;

        // ImGui::Begin("Nova"); // Create a window called "Hello, world!" and append into it.

        // ImGui::Text("This is some useful text."); // Display some text (you can use a format
        //                                           // strings too)
        // // ImGui::Checkbox("Demo Window",
        // //                 &show_demo_window); // Edit bools storing our window open/close state
        // // ImGui::Checkbox("Another Window", &show_another_window);

        // ImGui::SliderFloat("float", &f, 0.0f,
        //                    1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        // // ImGui::ColorEdit3("clear color",
        // //                   (float *)&clear_color); // Edit 3 floats representing a color

        // if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return
        //                              // true when edited/activated)
        //     counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        //             1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        // ImGui::End();
    } // Demonstrate creating a simple static window with no decoration + a context-menu to choose
      // which corner of the screen to use.
    // static void ShowExampleAppSimpleOverlay(bool* p_open)
    {
        const float DISTANCE = 10.0f;
        static int corner = 2;
        bool b = true;
        bool *p_open = &b;
        ImVec2 window_pos =
            ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE,
                   (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        if (corner != -1) ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

        // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);`
        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowBorderSize = 0.0f;

        if (ImGui::Begin(
                "Example: Simple Overlay", p_open,
                (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    /*ImGuiWindowFlags_AlwaysAutoResize |*/ ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            // ImGui::Text("Nova");
            // ImGui::Separator();
            auto &rpt = GraphicsSystem::getInstance().getRendererBackend().getProfileTimes();
            ImGui::Text((rpt[0].first + ":%d us.").c_str(), rpt[0].second);
            ImGui::Text((rpt[1].first + ":%d us.").c_str(), rpt[1].second);
            ImGui::Text((rpt[2].first + ":%d us.").c_str(), rpt[2].second);

            auto &apt = Application::getInstance().getProfileTimes();
            ImGui::Text((apt[0].first + ":%d us.").c_str(), apt[0].second);
            ImGui::Text((apt[1].first + ":%d us.").c_str(), apt[1].second);
            ImGui::Text("Frametime:%.3f ms. (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            // if (ImGui::IsMousePosValid())
            //     ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x,
            //                 ImGui::GetIO().MousePos.y);
            // else
            //     ImGui::Text("Mouse Position: <invalid>");
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
                if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
                if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
                if (p_open && ImGui::MenuItem("Close")) *p_open = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    ImGui::Render();
    // int display_w, display_h;
    // glfwMakeContextCurrent(window);
    // glfwGetFramebufferSize(window, &display_w, &display_h);
    // glViewport(0, 0, display_w, display_h);
    // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // glfwMakeContextCurrent(Window::getInstance().mGLFWindow);
}
} // namespace Nova