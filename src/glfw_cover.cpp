/** Digital 3D framework: glfw C++ wrapper definitions  

 Copyright (c) 2021 Igor Balezin <bigov@yandex.com>

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would
    be appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not
    be misrepresented as being the original software.

 3. This notice may not be removed or altered from any source
    distribution.
*/

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "glfw_cover.hpp"

namespace dig3d
{
// Инициализация статических членов
bool glfw_cover::gl_is_loaded = false;
GLFWwindow* glfw_cover::gl_win::pointer = nullptr;
interface_gl_context* glfw_cover::error_observer = nullptr;
interface_gl_context* glfw_cover::gl_win::cursor_observer = nullptr;
interface_gl_context* glfw_cover::gl_win::button_observer = nullptr;
interface_gl_context* glfw_cover::gl_win::keyboard_observer = nullptr;
interface_gl_context* glfw_cover::gl_win::position_observer = nullptr;
interface_gl_context* glfw_cover::gl_win::size_observer = nullptr;
interface_gl_context* glfw_cover::gl_win::char_observer = nullptr;
interface_gl_context* glfw_cover::gl_win::close_observer = nullptr;
interface_gl_context* glfw_cover::gl_win::focuslost_observer = nullptr;

///
/// \brief glfw_cover::glfw_cover
///
glfw_cover::glfw_cover(void)
{
  if (!glfwInit()) std::cerr << "Fatal error: can't init GLFW lib." << std::endl;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, 1);

#ifndef NDEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#endif
}

glfw_cover::gl_win::gl_win(const char* row)
{
  title = row;
}

//
// Инициализация LG функцй (glad)
// (можно запускать только(??) после создания контекста)
// 
void glfw_cover::load_gl(void)
{
  if(gl_is_loaded) return;
  gladLoadGL(glfwGetProcAddress);
  //if(!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
  //{
  //  if(!gladLoadGL()) std::cerr << "Critical error: can't load GLAD." << std::endl;
 // }
  gl_is_loaded = true;
}


///
/// Настройка окна и обработчиков ввода
///
void glfw_cover::init_window(gl_win& Win)
{
  Win.pointer = glfwCreateWindow(Win.width, Win.height, Win.title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(Win.pointer);
  load_gl();

  glfwSetErrorCallback(callback_error);
  Win.init_callbacks();

  glfwSetWindowSizeLimits(Win.pointer, static_cast<int>(Win.min_w), static_cast<int>(Win.min_h), GLFW_DONT_CARE, GLFW_DONT_CARE);
  glfwSetWindowSize(Win.pointer, static_cast<int>(Win.width), static_cast<int>(Win.height));
  glfwSetWindowPos(Win.pointer, static_cast<int>(Win.left), static_cast<int>(Win.top));

  glfwShowWindow(Win.pointer);
  glfwSwapInterval(0);  // Vertical sync is "OFF". When param is 1 - will be ON
  glfwSetInputMode(Win.pointer, GLFW_STICKY_KEYS, 0);
}


void glfw_cover::gl_win::init_callbacks(void)
{
  glfwSetKeyCallback(pointer, callback_keyboard);
  glfwSetCharCallback(pointer, callback_char);
  glfwSetMouseButtonCallback(pointer, callback_button);
  glfwSetCursorPosCallback(pointer, callback_cursor);
  glfwSetFramebufferSizeCallback(pointer, callback_size);
  glfwSetWindowPosCallback(pointer, callback_position);
  glfwSetWindowCloseCallback(pointer, callback_close);
  glfwSetWindowFocusCallback(pointer, callback_focus);
}

void glfw_cover::gl_win::show(void)
{
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
  ImGui_ImplGlfw_InitForOpenGL(pointer, true);
  const char* glsl_version = "#version 150";
  ImGui_ImplOpenGL3_Init(glsl_version);
  ImVec4 clear_color = ImVec4(171.f/255, 211.f/255, 239.f/255, 1.f);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

  while (!glfwWindowShouldClose(pointer))
    {
        glfwSwapBuffers(pointer);
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
        glfwGetFramebufferSize(pointer, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

}


///
///  Destructor
///
glfw_cover::~glfw_cover(void)
{
  glfwTerminate();
}

glfw_cover::gl_win::~gl_win(void)
{
  if(nullptr != pointer) glfwDestroyWindow(pointer);
  pointer = nullptr;
}

void glfw_cover::set_error_observer(interface_gl_context& ref)
{
  error_observer = &ref;
}
void glfw_cover::gl_win::set_cursor_observer(interface_gl_context& ref)
{
  cursor_observer = &ref;
}
void glfw_cover::gl_win::set_mbutton_observer(interface_gl_context& ref)
{
  button_observer = &ref;
}
void glfw_cover::gl_win::set_keyboard_observer(interface_gl_context& ref)
{
  keyboard_observer = &ref;
}
void glfw_cover::gl_win::set_position_observer(interface_gl_context& ref)
{
  position_observer = &ref;
}
void glfw_cover::gl_win::set_size_observer(interface_gl_context& ref)
{
  size_observer = &ref;
}
void glfw_cover::gl_win::set_char_observer(interface_gl_context& ref)
{
  char_observer = &ref;
}
void glfw_cover::gl_win::set_close_observer(interface_gl_context& ref)
{
  close_observer = &ref;
}
void glfw_cover::gl_win::set_focuslost_observer(interface_gl_context& ref)
{
  focuslost_observer = &ref;
}


///
/// \brief wglfw::swap_buffers
///
void glfw_cover::gl_win::swap_buffers(void)
{
  glfwSwapBuffers(pointer);
  glfwPollEvents();
}


///
/// \brief wglfw::cursor_hide
///
void glfw_cover::gl_win::cursor_hide(void)
{
  glfwSetInputMode(pointer, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  // отключить ввод символов
  glfwSetCharCallback(pointer, nullptr);
}


///
/// \brief wglfw::cursor_restore
///
void glfw_cover::gl_win::cursor_restore(void)
{
  glfwSetInputMode(pointer, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // разрешить ввод символов
  glfwSetCharCallback(pointer, callback_char);
}


///
/// \brief wglfw::set_cursor_pos
/// \param x
/// \param y
///
void glfw_cover::gl_win::set_cursor_pos(double x, double y)
{
  glfwSetCursorPos(pointer, x, y);
}


///
/// \brief wglfw::get_frame_buffer_size
/// \param width
/// \param height
///
void glfw_cover::gl_win::get_frame_size(int* width, int* height)
{
  glfwGetFramebufferSize(pointer, width, height);
}


///
/// \brief wglfw::error_callback
/// \param error
/// \param description
///
/// \details Errors callback
///
/// \todo Если тут настроить обработчик через вызов метода
/// "наблюдателя", то можно полностью сделать класс wglfw
/// независимым, свободным от внешних связей модулем.
///
void glfw_cover::callback_error(int error, const char* description)
{
  std::string Message = "GLFW error " + std::to_string(error) + ": " + description;
  if(error_observer != nullptr) error_observer->event_error(Message.c_str());
}


///
/// Обработчик перемещений курсора мыши
/// \param ptWin - указатель окна
/// \param xpos  - X координата курсора в окне
/// \param ypos  - Y координата курсора в окне
///
void glfw_cover::gl_win::callback_cursor(GLFWwindow*, double x, double y)
{
  if(cursor_observer != nullptr) cursor_observer->event_cursor(x, y);
}


///
/// \brief window_glfw::mouse_button_callback
/// \param window
/// \param button
/// \param action
/// \param mods
///
void glfw_cover::gl_win::callback_button(GLFWwindow*, int button, int action, int mods)
{
  if(button_observer != nullptr) button_observer->event_mouse_btns(button, action, mods);
}


///
/// Keys events callback
///
void glfw_cover::gl_win::callback_keyboard(GLFWwindow*, int key, int scancode, int action, int mods)
{
  if(keyboard_observer != nullptr) keyboard_observer->event_keyboard(key, scancode, action, mods);
}


///
/// GLFW window moving callback
///
void glfw_cover::gl_win::callback_position(GLFWwindow*, int left, int top)
{
  if(position_observer != nullptr) position_observer->event_reposition(left, top);
}


///
/// \brief wglfw::resize_callback
/// \param WindowPointer
/// \param width
/// \param height
/// \details GLFW framebuffer and window-data callback resize
///
void glfw_cover::gl_win::callback_size(GLFWwindow*, int width, int height)
{
  if(size_observer != nullptr) size_observer->event_resize(width, height);
}


///
/// \brief glfw_wr::character_callback
/// \param window
/// \param key
///
void glfw_cover::gl_win::callback_char(GLFWwindow*, unsigned int ch)
{
  if(char_observer != nullptr) char_observer->event_character(ch);
}


///
/// \brief wglfw::window_close_callback
/// \param w
///
void glfw_cover::gl_win::callback_close(GLFWwindow*)
{
  if(close_observer != nullptr) close_observer->event_close();
}


///
/// \brief wglfw::window_focus_callback
/// \param w
///
void glfw_cover::gl_win::callback_focus(GLFWwindow*,  int focused)
{
  if(0 == focused)
  {
    if(focuslost_observer != nullptr) focuslost_observer->event_focus_lost();
  }
}

} //namespace dig3d
