project(screensavers.rsxs)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

cmake_policy(SET CMP0054 NEW)

find_package(Kodi REQUIRED)
find_package(glm REQUIRED)
find_package(gli REQUIRED)

#set(APP_RENDER_SYSTEM "gles")

if(APP_RENDER_SYSTEM STREQUAL "gl" OR NOT APP_RENDER_SYSTEM)
  find_package(OpenGl REQUIRED)
  set(DEPLIBS ${OPENGL_LIBRARIES})
  set(includes ${OPENGL_INCLUDE_DIR})
  add_definitions(${OPENGL_DEFINITIONS})
elseif(APP_RENDER_SYSTEM STREQUAL "gles")
  find_package(OpenGLES REQUIRED)
  set(DEPLIBS ${OPENGLES_LIBRARIES})
  set(includes ${OPENGLES_INCLUDE_DIR})
  add_definitions(${OPENGLES_DEFINITIONS})
endif()

include_directories(${KODI_INCLUDE_DIR}/..) # Hack way with "/..", need bigger Kodi cmake rework to match right include ways (becomes done in future)

add_subdirectory(lib/kodi/OpenGL)
add_subdirectory(lib/Implicit)
add_subdirectory(lib/Rgbhsl)
add_subdirectory(lib/rsMath)

list(APPEND DEPENDS rsMath kodiOpenGL)
list(APPEND DEPLIBS rsMath kodiOpenGL Implicit Rgbhsl)

if(NOT ${CORE_SYSTEM_NAME} STREQUAL "")
  if(${CORE_SYSTEM_NAME} STREQUAL "osx" OR ${CORE_SYSTEM_NAME} STREQUAL "ios")
    list(APPEND DEPLIBS "-framework CoreVideo")
  endif()
endif()

get_directory_property(BASE_DEFINITIONS COMPILE_DEFINITIONS)
message(STATUS "Compile definitions: ${BASE_DEFINITIONS}")
if(CMAKE_BUILD_TYPE)
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

include(src/biof/CMakeLists.txt)
include(src/busyspheres/CMakeLists.txt)
include(src/colorfire/CMakeLists.txt)
include(src/cyclone/CMakeLists.txt)
include(src/drempels/CMakeLists.txt)
include(src/euphoria/CMakeLists.txt)
include(src/feedback/CMakeLists.txt)
include(src/fieldlines/CMakeLists.txt)
include(src/flocks/CMakeLists.txt)
include(src/flux/CMakeLists.txt)
include(src/helios/CMakeLists.txt)
include(src/hufo_smoke/CMakeLists.txt)
include(src/hufo_tunnel/CMakeLists.txt)
include(src/hyperspace/CMakeLists.txt)
include(src/lattice/CMakeLists.txt)
include(src/lorenz/CMakeLists.txt)
include(src/matrixview/CMakeLists.txt)
include(src/plasma/CMakeLists.txt)
include(src/skyrocket/CMakeLists.txt)
include(src/solarwinds/CMakeLists.txt)
include(src/spirographx/CMakeLists.txt)
include(src/sundancer2/CMakeLists.txt)

include(CPack)

