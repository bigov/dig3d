/** Digital 3D framework 
  
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

#include <dig3d_config.hpp>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "glfw_cover.hpp"

int main(int, char**)
{
  std::string APP_TITLE = "Dig 3D version ";
  APP_TITLE += APP_VERSION;
    
#ifndef NDEBUG
  APP_TITLE += " [Debug]";
  auto logger = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>(
      "DIG", "debug_log.txt");
  logger->info("start session");
#else
  APP_TITLE += " [Release]";
#endif

  using namespace dig3d;

  glfw_cover App {};
  glfw_cover::gl_win MainWin { APP_TITLE.c_str() };
  App.init_window(MainWin);
  MainWin.show();
  
  exit(EXIT_SUCCESS);
}
