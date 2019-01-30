project(gli)

cmake_minimum_required(VERSION 3.0)

include(ExternalProject)
ExternalProject_Add(gli
  GIT_REPOSITORY    https://github.com/g-truc/gli
  GIT_TAG           master
  BUILD_COMMAND     ""
  INSTALL_COMMAND   "")

ExternalProject_Get_Property(gli source_dir)
set(GLI_INCLUDE_DIR ${source_dir} CACHE INTERNAL "")

add_dependencies(gli glm)
