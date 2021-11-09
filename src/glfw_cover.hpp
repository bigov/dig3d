/** Digital 3D framework: glfw C++ wrapper declarations 

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

#ifndef GLFW_COVER_HPP
#define GLFW_COVER_HPP

#include <iostream>
#include <list>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace dig3d
{

// Интерфейс графического окна для обмена данными с OpenGL контентом
class interface_gl_context
{
public:
    interface_gl_context(void) = default;
    virtual ~interface_gl_context(void) = default;

    virtual void event_error(const char*) {}
    virtual void event_character(unsigned int) {}
    virtual void event_mouse_btns(int, int, int) {}
    virtual void event_keyboard(int, int, int, int) {}
    virtual void event_reposition(int, int) {}
    virtual void event_resize(int, int) {}
    virtual void event_cursor(double, double) {}
    virtual void event_close(void) {}
    virtual void event_focus_lost(void) {}
};

class glfw_cover
{
  public:
    class glfw_window
    {
      public:
        glfw_window(const char* title = "\0");

        // Запретить копирование объекта
        glfw_window(const glfw_window&) = delete;
        glfw_window& operator=(const glfw_window&) = delete;

        // Запретить перенос объекта
        glfw_window(glfw_window&&) = delete;
        glfw_window& operator=(glfw_window&&) = delete;

        ~glfw_window(void);

        std::string title{};
        unsigned int width=600;
        unsigned int height=400;
        unsigned int min_w=300;
        unsigned int min_h=200;
        unsigned int left=20;
        unsigned int top=40;

        void set_window(unsigned int width=10, unsigned int height=10, unsigned int min_w=0,
                    unsigned int min_h=0, unsigned int left=0, unsigned int top=0);
        void swap_buffers(void);
        void cursor_hide(void);
        void cursor_restore(void);
        void set_cursor_pos(double x, double y);
        void get_frame_size(int* width, int* height);

        static void set_mbutton_observer(interface_gl_context& ref);   // кнопки мыши
        static void set_char_observer(interface_gl_context& ref);      // ввод текста (символ)
        static void set_close_observer(interface_gl_context& ref);     // закрытие окна
        static void set_cursor_observer(interface_gl_context& ref);    // курсор мыши в окне
        static void set_focuslost_observer(interface_gl_context& ref); // утеря фокуса
        static void set_keyboard_observer(interface_gl_context& ref);  // клавиши клавиатуры
        static void set_position_observer(interface_gl_context& ref);  // положение окна
        static void set_size_observer(interface_gl_context& ref);      // размер окна
        
        static void init_callbacks(void);
        static void show(void);

        friend class glfw_cover;

      private:
        static GLFWwindow* win_ptr;

        static interface_gl_context* button_observer;
        static interface_gl_context* char_observer;
        static interface_gl_context* close_observer;
        static interface_gl_context* cursor_observer;
        static interface_gl_context* focuslost_observer;
        static interface_gl_context* keyboard_observer;
        static interface_gl_context* position_observer;
        static interface_gl_context* size_observer;

        static void callback_button(GLFWwindow* ptr, int button, int action, int mods);
        static void callback_char(GLFWwindow* ptr, unsigned int);
        static void callback_close(GLFWwindow* ptr);
        static void callback_cursor(GLFWwindow* ptr, double xpos, double ypos);
        static void callback_focus(GLFWwindow* ptr, int focused);
        static void callback_keyboard(GLFWwindow* ptr, int key, int scancode, int action, int mods);
        static void callback_position(GLFWwindow* ptr, int, int);
        static void callback_size(GLFWwindow* ptr, int, int);
    };

    glfw_cover(void);
    ~glfw_cover(void);
    // Запретить копирование объекта
    glfw_cover(const glfw_cover&) = delete;
    glfw_cover& operator=(const glfw_cover&) = delete;

    // Запретить перенос объекта
    glfw_cover(glfw_cover&&) = delete;
    glfw_cover& operator=(glfw_cover&&) = delete;
    
    static void set_error_observer(interface_gl_context& ref);    // отслеживание ошибок
    static void load_gl(void);
    static void init_window(glfw_window& w);

  private:
    static bool gl_is_loaded;
    static interface_gl_context* error_observer;
    static void callback_error(int error_id, const char* description);
  };

} //namespace dig3d

#endif //GLFW_COVER_HPP
