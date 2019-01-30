project(glm)

cmake_minimum_required(VERSION 3.0)

include(ExternalProject)
ExternalProject_Add(glm
  GIT_REPOSITORY    https://github.com/g-truc/glm
  GIT_TAG           47031aa4b7b079716e4cac496819e7f464b6a713
  BUILD_COMMAND     ""
  INSTALL_COMMAND   "")

ExternalProject_Get_Property(glm source_dir)
set(GLM_INCLUDE_DIR ${source_dir} CACHE INTERNAL "")
