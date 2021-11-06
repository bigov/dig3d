#
# Main building script
#

SET( DIG_EXT_SRC "${DIG_SOURCE_DIR}/external" )
include_directories( "${CMAKE_BINARY_DIR}/include" )
link_directories ( "${CMAKE_BINARY_DIR}/lib" )

include( ExternalProject )

# Unpack glad
if( "${CMAKE_BUILD_TYPE}" MATCHES "^Release$" )
  SET( GLAD_ZIP "${DIG_EXT_SRC}/glad/CppCoreGL33.zip")
else()
  SET( GLAD_ZIP "${DIG_EXT_SRC}/glad/CppCoreGL33Debug.zip" )
endif()
file( ARCHIVE_EXTRACT INPUT "${GLAD_ZIP}" DESTINATION "${CMAKE_BINARY_DIR}" )

# Get glfw
list( APPEND GLFW_CMAKE_ARGS "-DGLFW_BUILD_DOCS=OFF" )
list( APPEND GLFW_CMAKE_ARGS "-DGLFW_BUILD_TESTS=OFF" )
list( APPEND GLFW_CMAKE_ARGS "-DGLFW_BUILD_EXAMPLES=OFF" )
list( APPEND GLFW_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}" )
list( APPEND GLFW_CMAKE_ARGS "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}" )
ExternalProject_Add( GLFW
  SOURCE_DIR "${DIG_EXT_SRC}/glfw"
  CMAKE_ARGS ${GLFW_CMAKE_ARGS} )

# Get imgui
FILE( MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include/imgui" )
FILE( GLOB IMGUI_SRC_FILES
  "${DIG_EXT_SRC}/imgui/*.cpp"
  "${DIG_EXT_SRC}/imgui/*.h"
  "${DIG_EXT_SRC}/imgui/backends/imgui_impl_glfw.*"
  "${DIG_EXT_SRC}/imgui/backends/imgui_impl_opengl3.*"
  "${DIG_EXT_SRC}/imgui/backends/imgui_impl_opengl3_loader.h"
  )
foreach( FILE_NAME ${IMGUI_SRC_FILES} )
  FILE( COPY "${FILE_NAME}" DESTINATION "${CMAKE_BINARY_DIR}/include/imgui" )
endforeach()

FILE( GLOB IMGUI_FILES "${CMAKE_BINARY_DIR}/include/imgui/*.cpp" )
ADD_LIBRARY( imgui STATIC ${IMGUI_FILES} )

add_executable( dig3d "${DIG_SOURCE_DIR}/src/main.cpp" )
add_dependencies( dig3d GLFW )
target_link_libraries( dig3d glfw3 imgui)
