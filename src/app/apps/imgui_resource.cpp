#include "app/apps/imgui_resource.h"

void ImguiResourceManagerApp::init(InitData& init_data) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // by default, disable mouse & keyboard inputs
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
    // disable input capture
    ImGui_ImplGlfw_InitForOpenGL(init_data.window, true);
    ImGui_ImplOpenGL3_Init();
}

void ImguiResourceManagerApp::tickpre() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImguiResourceManagerApp::tickpost() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiResourceManagerApp::tick(TickData& tick_data) {
    // ImGui::Begin("Hello Imgui");
    // ImGui::End();
}


