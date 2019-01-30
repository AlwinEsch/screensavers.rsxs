project(screensaver.rsxs.drempels)

cmake_minimum_required(VERSION 3.0)

enable_language(CXX)

message(STATUS "--------------------------------------------------------------------------------")
message(STATUS "Create \"${CMAKE_PROJECT_NAME}\"")

unset(ADDON_DEPENDS)
unset(USED_SOURCES)
set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS ${BASE_DEFINITIONS})

find_package(ImageMagick COMPONENTS MagickWand REQUIRED)

set(DREMPELS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/main.cpp
                     ${CMAKE_CURRENT_LIST_DIR}/gpoly.cpp
                     ${CMAKE_CURRENT_LIST_DIR}/noise1234.c
                     ${CMAKE_CURRENT_LIST_DIR}/TexMgr.cpp)

set(DREMPELS_HEADERS ${CMAKE_CURRENT_LIST_DIR}/main.h
                     ${CMAKE_CURRENT_LIST_DIR}/gpoly.h
                     ${CMAKE_CURRENT_LIST_DIR}/noise1234.h
                     ${CMAKE_CURRENT_LIST_DIR}/TexMgr.h)

include_directories(${ImageMagick_INCLUDE_DIRS})
set(DREMPELS_DEPLIBS ${DEPLIBS} ${ImageMagick_LIBRARIES})
build_addon(screensaver.rsxs.drempels DREMPELS DREMPELS_DEPLIBS)

message(FATAL_ERROR "----------> ${ImageMagick_INCLUDE_DIRS}")
