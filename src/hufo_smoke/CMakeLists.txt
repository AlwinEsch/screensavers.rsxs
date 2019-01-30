project(screensaver.rsxs.hufo_smoke)

cmake_minimum_required(VERSION 3.0)

enable_language(CXX)

message(STATUS "--------------------------------------------------------------------------------")
message(STATUS "Create \"${CMAKE_PROJECT_NAME}\"")

unset(ADDON_DEPENDS)
unset(USED_SOURCES)
set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS ${BASE_DEFINITIONS})

set(HUFO_SMOKE_SOURCES ${CMAKE_CURRENT_LIST_DIR}/main.cpp
                       ${CMAKE_CURRENT_LIST_DIR}/FMotion.cpp)
set(HUFO_SMOKE_HEADERS ${CMAKE_CURRENT_LIST_DIR}/main.h
                       ${CMAKE_CURRENT_LIST_DIR}/FMotion.h)

build_addon(screensaver.rsxs.hufo_smoke HUFO_SMOKE DEPLIBS)
