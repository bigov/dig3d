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

#include "glfw_cover.h"

namespace dig3d
{
// Инициализация статических членов
bool glfw_cover::gl_is_loaded = false;
GLFWwindow* glfw_window::win_ptr = nullptr;
interface_gl_context* glfw_cover::error_observer = nullptr;
interface_gl_context* glfw_window::cursor_observer = nullptr;
interface_gl_context* glfw_window::button_observer = nullptr;
interface_gl_context* glfw_window::keyboard_observer = nullptr;
interface_gl_context* glfw_window::position_observer = nullptr;
interface_gl_context* glfw_window::size_observer = nullptr;
interface_gl_context* glfw_window::char_observer = nullptr;
interface_gl_context* glfw_window::close_observer = nullptr;
interface_gl_context* glfw_window::focuslost_observer = nullptr;


dig3dapp::dig3dapp(std::string Title)
{
  MainWin = &GlfwCover.create_window(Title.c_str());
  MainWin->set_keyboard_observer(*this);
  MainWin->show();
}

void dig3dapp::event_keyboard(int, int, int, int)
{
  GlfwCover.delete_window(*MainWin);
}

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

glfw_window::glfw_window(const char* row)
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
  if (gladLoadGL(glfwGetProcAddress) == 0)
    std::cerr << "Critical error: can't load GLAD." << std::endl;
  gl_is_loaded = true;
}


///
/// Настройка окна и обработчиков ввода
///
glfw_window& glfw_cover::create_window(const char* row)
{
  AppWindows.emplace_back(row);
  glfw_window& WinRef = AppWindows.back();

  WinRef.win_ptr = glfwCreateWindow(WinRef.width, WinRef.height, WinRef.title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(WinRef.win_ptr);
  load_gl();

  glfwSetErrorCallback(callback_error);
  WinRef.init_callbacks();

  glfwSetWindowSizeLimits(WinRef.win_ptr, static_cast<int>(WinRef.min_w), static_cast<int>(WinRef.min_h), GLFW_DONT_CARE, GLFW_DONT_CARE);
  glfwSetWindowSize(WinRef.win_ptr, static_cast<int>(WinRef.width), static_cast<int>(WinRef.height));
  glfwSetWindowPos(WinRef.win_ptr, static_cast<int>(WinRef.left), static_cast<int>(WinRef.top));

  glfwShowWindow(WinRef.win_ptr);
  glfwSwapInterval(0);  // Vertical sync is "OFF". When param is 1 - will be ON
  glfwSetInputMode(WinRef.win_ptr, GLFW_STICKY_KEYS, 0);
  return WinRef;
}


void glfw_cover::delete_window(glfw_window& Window)
{
  glfwMakeContextCurrent(nullptr);
  if(AppWindows.empty()) return;

  auto it = std::find_if(AppWindows.begin(), AppWindows.end(),
                         [&Window](const auto& itWin){ return itWin.win_ptr == Window.win_ptr; });

  glfwSetWindowShouldClose(it->win_ptr, GL_TRUE);
  // Если есть, то удалить
  if(it != AppWindows.end())
  {
    AppWindows.erase(it);
  }

}

void glfw_window::init_callbacks(void)
{
  glfwSetKeyCallback(win_ptr, callback_keyboard);
  glfwSetCharCallback(win_ptr, callback_char);
  glfwSetMouseButtonCallback(win_ptr, callback_button);
  glfwSetCursorPosCallback(win_ptr, callback_cursor);
  glfwSetFramebufferSizeCallback(win_ptr, callback_size);
  glfwSetWindowPosCallback(win_ptr, callback_position);
  glfwSetWindowCloseCallback(win_ptr, callback_close);
  glfwSetWindowFocusCallback(win_ptr, callback_focus);
}


void glfw_window::show(void)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  [[maybe_unused]] ImGuiIO& io = ImGui::GetIO();
  //(void)io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(win_ptr, true);
  const char* glsl_version = "#version 150";
  ImGui_ImplOpenGL3_Init(glsl_version);
  ImVec4 clear_color = ImVec4(171.f/255, 211.f/255, 239.f/255, 1.f);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

  while (!glfwWindowShouldClose(win_ptr))
    {
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
        glfwGetFramebufferSize(win_ptr, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(win_ptr);
        glClear(GL_COLOR_BUFFER_BIT);
        //glfwWaitEvents(); // статическое окно
        glfwPollEvents();   // динамическая картинка
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


///
///  Destructor
///
glfw_cover::~glfw_cover(void)
{
  if(!AppWindows.empty())
    for(auto& Window: AppWindows) delete_window(Window);
  glfwTerminate();
}


glfw_window::~glfw_window(void)
{
  if(nullptr != win_ptr) glfwDestroyWindow(win_ptr);
  win_ptr = nullptr;
}

void glfw_cover::set_error_observer(interface_gl_context& ref)
{
  error_observer = &ref;
}
void glfw_window::set_cursor_observer(interface_gl_context& ref)
{
  cursor_observer = &ref;
}
void glfw_window::set_mbutton_observer(interface_gl_context& ref)
{
  button_observer = &ref;
}
void glfw_window::set_keyboard_observer(interface_gl_context& ref)
{
  keyboard_observer = &ref;
}
void glfw_window::set_position_observer(interface_gl_context& ref)
{
  position_observer = &ref;
}
void glfw_window::set_size_observer(interface_gl_context& ref)
{
  size_observer = &ref;
}
void glfw_window::set_char_observer(interface_gl_context& ref)
{
  char_observer = &ref;
}
void glfw_window::set_close_observer(interface_gl_context& ref)
{
  close_observer = &ref;
}
void glfw_window::set_focuslost_observer(interface_gl_context& ref)
{
  focuslost_observer = &ref;
}


///
/// \brief wglfw::swap_buffers
///
void glfw_window::swap_buffers(void)
{
  glfwSwapBuffers(win_ptr);
  glfwPollEvents();
}


///
/// \brief wglfw::cursor_hide
///
void glfw_window::cursor_hide(void)
{
  glfwSetInputMode(win_ptr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  // отключить ввод символов
  glfwSetCharCallback(win_ptr, nullptr);
}


///
/// \brief wglfw::cursor_restore
///
void glfw_window::cursor_restore(void)
{
  glfwSetInputMode(win_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // разрешить ввод символов
  glfwSetCharCallback(win_ptr, callback_char);
}


///
/// \brief wglfw::set_cursor_pos
/// \param x
/// \param y
///
void glfw_window::set_cursor_pos(double x, double y)
{
  glfwSetCursorPos(win_ptr, x, y);
}


///
/// \brief wglfw::get_frame_buffer_size
/// \param width
/// \param height
///
void glfw_window::get_frame_size(int* width, int* height)
{
  glfwGetFramebufferSize(win_ptr, width, height);
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
void glfw_window::callback_cursor(GLFWwindow*, double x, double y)
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
void glfw_window::callback_button(GLFWwindow*, int button, int action, int mods)
{
  if(button_observer != nullptr) button_observer->event_mouse_btns(button, action, mods);
}


///
/// Keys events callback
///
void glfw_window::callback_keyboard(GLFWwindow*, int key, int scancode, int action, int mods)
{
  if(keyboard_observer != nullptr) keyboard_observer->event_keyboard(key, scancode, action, mods);
}


///
/// GLFW window moving callback
///
void glfw_window::callback_position(GLFWwindow*, int left, int top)
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
void glfw_window::callback_size(GLFWwindow*, int width, int height)
{
  if(size_observer != nullptr) size_observer->event_resize(width, height);
}


///
/// \brief glfw_wr::character_callback
/// \param window
/// \param key
///
void glfw_window::callback_char(GLFWwindow*, unsigned int ch)
{
  if(char_observer != nullptr) char_observer->event_character(ch);
}


///
/// \brief wglfw::window_close_callback
/// \param w
///
void glfw_window::callback_close(GLFWwindow*)
{
  if(close_observer != nullptr) close_observer->event_close();
}


///
/// \brief wglfw::window_focus_callback
/// \param w
///
void glfw_window::callback_focus(GLFWwindow*,  int focused)
{
  if(0 == focused)
  {
    if(focuslost_observer != nullptr) focuslost_observer->event_focus_lost();
  }
}

} //namespace dig3d
