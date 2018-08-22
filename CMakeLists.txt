cmake_minimum_required(VERSION 3.10)
project(ProjGraf)

set(CMAKE_CXX_STANDARD 11)

add_executable(ProjGraf main.cpp controller.h controller.cpp mesh.cpp mesh.h plane.h plane.cpp terrain.cpp terrain.h point3.h perlinnoise.cpp perlinnoise.h HUD.cpp HUD.h texture.cpp texture.h skybox.cpp skybox.h utils.h meteorshower.cpp meteorshower.h)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

find_package(GLEW REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${GLEW_LIBRARY})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})