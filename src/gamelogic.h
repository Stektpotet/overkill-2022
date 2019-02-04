#pragma once


#include <utilities/window.hpp>

void initGame(GLFWwindow* window, CommandLineOptions options);
void updateFrame(GLFWwindow* window);
void renderFrame(GLFWwindow* window);