#
# Specify minimum CMake version and project name
#
cmake_minimum_required (VERSION 3.0)
project (glowbox)

#
# CMake setup
#
set (CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_VERBOSE_MAKEFILE 0) # 1 should be used for debugging
set (CMAKE_SUPPRESS_REGENERATION TRUE) # Suppresses ZERO_CHECK
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
  if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
  endif()
endif()

#
# GLFW options
#
option (GLFW_INSTALL OFF)
option (GLFW_BUILD_DOCS OFF)
option (GLFW_BUILD_EXAMPLES OFF)
option (GLFW_BUILD_TESTS OFF)
add_subdirectory (lib/glfw)


# SFML options
# We only need sound for this project, so everything else can be turned off
option (SFML_BUILD_GRAPHICS OFF)
option (SFML_BUILD_WINDOW OFF)
option (SFML_BUILD_NETWORK OFF)
add_subdirectory(lib/SFML)

#
# Set include paths
#
include_directories (src/
                     lib/glad/include/
                     lib/glfw/include/
                     lib/lodepng/
                     lib/glm/
                     lib/stb/
                     lib/arrrgh/
                     lib/SFML/include/)

#
# Add files
#
file (GLOB         VENDORS_SOURCES lib/glad/src/glad.c
								   lib/lodepng/lodepng.cpp)
file (GLOB_RECURSE PROJECT_HEADERS src/*.hpp
                                   src/*.h)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp
                                   src/*.cxx
                                   src/*.cc
                                   src/*.c)
file (GLOB_RECURSE PROJECT_SHADERS res/shaders/*.comp
                                   res/shaders/*.frag
                                   res/shaders/*.geom
                                   res/shaders/*.vert)
file (GLOB         PROJECT_CONFIGS CMakeLists.txt
                                   README.rst
                                  .gitignore
                                  .gitmodules)

#
# Organizing files
#
source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("shaders" FILES ${PROJECT_SHADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})
source_group ("libraries" FILES ${VENDORS_SOURCES})

#
# Set executable and target link libraries
#
add_definitions (-DGLFW_INCLUDE_NONE
                 -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                                ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                                ${VENDORS_SOURCES})
target_link_libraries (${PROJECT_NAME}
                       glfw
                       sfml-audio
                       ${GLFW_LIBRARIES}
                       ${GLAD_LIBRARIES})
