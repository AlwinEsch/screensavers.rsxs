cmake_minimum_required(VERSION 3.0)

project(kodiOpenGL)

set(CMAKE_POSITION_INDEPENDENT_CODE 1)

set(SOURCES Texture.cpp
            ErrorCheck.cpp
            Shader.cpp)

set(HEADERS Texture.h
            ErrorCheck.h
            Shader.h
            Time.h
            GL.h)

add_library(kodiOpenGL STATIC ${SOURCES} ${HEADERS})
add_dependencies(kodiOpenGL gli glm)
target_include_directories(kodiOpenGL PUBLIC ${GLI_INCLUDE_DIR} ${GLM_INCLUDE_DIR})
