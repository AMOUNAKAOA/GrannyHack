#ifndef MENU
#define MENU

#include "ImGuiStuff.h"

namespace Menu {

    ImVec4 color = ImVec4(1, 1, 1, 1);

    inline void DrawMenu() {

        {
            static int counter = 0;
            static float f;

            ImGui::Begin("Vartora menu"); // Create a window called "Hello, world!" and append into it.
            ImGui::Text("Thanks Azonyx for this template");               // Display some text (you can use a format strings too)
            ImGui::SliderFloat("Speed", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            if (ImGui::Button(
                    "niggas"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("niggas = %d", counter);

            char str1[128] = "Hello, world!";
            ImGui::InputTextWithHint("text input", "enter text here", str1, IM_ARRAYSIZE(str1));
            ImGui::LabelText("test", "%s", str1);
            ImGui::End();
        }
    }

    inline void DrawImGui() {
        if (init) {

            ImGuiIO &io = ImGui::GetIO();

            static bool WantTextInputLast = false;
//            if (io.WantTextInput && !WantTextInputLast)
//                displayKeyboard(true);
//            WantTextInputLast = io.WantTextInput;

            ImGui_ImplOpenGL3_NewFrame();

            ImGui_ImplAndroid_NewFrame(Unity::Screen::Width.Get(), Unity::Screen::Height.Get());

            ImGui::NewFrame();
            DrawMenu();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            /*
             * Don't uncomment this after updating imgui this part crash xD
             * I have to update imgui else InputTextWithHint bork on touch.
             */
//            io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_Tab]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_PageUp]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_PageDown]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_Escape]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_Delete]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_Home]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_End]] = false;
//            io.KeysDown[io.KeyMap[ImGuiKey_Insert]] = false;
            ImGui::EndFrame();

        }
    }
}
#endif MENU