
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
    // io.Fonts->AddFontFromFileTTF("./Dependencies/imgui/misc/fonts/Roboto-Medium.ttf", 13.0f);
    // io.Fonts->AddFontFromFileTTF("./Resources/fonts/Nexa Bold.otf", 130.0f);
    // io.Fonts->AddFontFromFileTTF("./Resources/fonts/Kontanter-Bold.otf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("./Dependencies/imgui/misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("./Dependencies/imgui/misc/fonts/DroidSans.ttf", 13.0f);
    // io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf",
    //                              13.0f);
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
static float profileTimes[6][20];
static int profileTimesOffset[6] = {0};

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

    // Demonstrate creating a simple static window with no decoration + a context-menu to choose
    // which corner of the screen to use.
    // static void ShowExampleAppSimpleOverlay(bool* p_open)
    {
        const float DISTANCE = 10.0f;
        static int corner = 2;
        bool yes = true;
        ImVec2 window_pos =
            ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE,
                   (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        if (corner != -1) ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

        // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);`
        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowBorderSize = 0.0f;
        style.WindowRounding = 0.0f;
        style.AntiAliasedLines = false;

        if (ImGui::Begin(
                "", &yes,
                (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    /*ImGuiWindowFlags_AlwaysAutoResize |*/ ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            // ImGui::ShowDemoWindow(&yes);
            // ImGui::Text("Nova");
            // ImGui::Separator();
            auto &rpt = GraphicsSystem::getInstance().getRendererBackend().getProfileTimes();
            ImGui::Text("geometry pass:%ld us.", rpt["Geometry pass"]); // geometry pass
            profileTimes[0][profileTimesOffset[0]] = rpt["Geometry pass"];
            profileTimesOffset[0] = (profileTimesOffset[0] + 1) % IM_ARRAYSIZE(profileTimes[0]);
            ImGui::PlotLines("", profileTimes[0], IM_ARRAYSIZE(profileTimes[0]),
                             profileTimesOffset[0], "", 0.0f, 5000.0f, ImVec2(0, 40));

            ImGui::Text("light pass:%ld us.", rpt["Light pass"]); // light pass
            profileTimes[1][profileTimesOffset[1]] = rpt["Light pass"];
            profileTimesOffset[1] = (profileTimesOffset[1] + 1) % IM_ARRAYSIZE(profileTimes[1]);
            ImGui::PlotLines("", profileTimes[1], IM_ARRAYSIZE(profileTimes[1]),
                             profileTimesOffset[1], "", 0.0f, 1000.0f, ImVec2(0, 40));

            ImGui::Text("post process:%ld us.", rpt["Post-process"]); // post process

            auto &apt = Application::getInstance().getProfileTimes();
            ImGui::Text("render time:%.1f ms.",
                        apt["Render time"] / 1000.0f); // render time

            ImGui::Text("entities update:%ld us.", apt["Entities update"]); // entity update
            profileTimes[3][profileTimesOffset[3]] = apt["Entities update"];
            profileTimesOffset[3] = (profileTimesOffset[3] + 1) % IM_ARRAYSIZE(profileTimes[3]);
            ImGui::PlotLines("", profileTimes[3], IM_ARRAYSIZE(profileTimes[3]),
                             profileTimesOffset[3], "", 0.0f, 1000.0f, ImVec2(0, 40));

            // profileTimes[2][profileTimesOffset[2]] = rpt[2].second;
            // profileTimesOffset[2] = (profileTimesOffset[2] + 1) % IM_ARRAYSIZE(profileTimes[2]);
            // ImGui::PlotLines("", profileTimes[2], IM_ARRAYSIZE(profileTimes[2]),
            //                  profileTimesOffset[2], "post process", 0.0f, 5000.0f, ImVec2(0,
            //                  40));

            // ImGui::Text("Frametime:%.2f ms. (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
            //             ImGui::GetIO().Framerate); // frametime
            ImGui::Text("Framecap:%.2f ms.", 1000.0f / ImGui::GetIO().Framerate); // frametime

            ImGui::Text("Entities total:%d",
                        Application::getInstance().getWorld().getNumEntities());
            ImGui::Text("Lights total:%d", GraphicsSystem::getInstance().getNumLights());

            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
                if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
                if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
                // if (&yes && ImGui::MenuItem("Close")) yes = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace Nova