/**
 * 
 * 
 */
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <iostream>
#include <dig3d_config.hpp>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %i: %s\n", error, description);
}

int main(int, char**)
{
#ifndef NDEBUG
  auto logger = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>(
      "DIG", "debug_log.txt");
  logger->info("start session");
#endif

  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) exit(EXIT_FAILURE);

  std::string APP_TITLE = "Dig 3D version ";
  APP_TITLE += APP_VERSION;
    
#ifndef NDEBUG
  APP_TITLE += " [Debug]";
#else
  APP_TITLE += " [Release]";
#endif

    window = glfwCreateWindow(1000, 600, APP_TITLE.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(0);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(171.f/255, 211.f/255, 239.f/255, 1.f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT);
        //glfwWaitEvents(); // статическое окно
        glfwPollEvents();   // динамическая картинка

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos( ImVec2(2, 2) );
        ImGui::SetNextWindowSize( ImVec2(114, 32) );
        
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Border] = ImVec4(0.4f, 0.4f, 0.4f, 0.2f);
        style.Colors[ImGuiCol_Text] = ImVec4(0.7f, 1.f, 0.7f, 1.f);
        ImGui::SetNextWindowBgAlpha(0.25f);

        {
          ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_Tooltip;

          ImGui::Begin("FPS", nullptr, window_flags);
          ImGui::Text(" FPS: %.1f ", ImGui::GetIO().Framerate);
          ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
