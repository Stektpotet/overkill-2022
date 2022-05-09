#pragma once

// System Headers
#include <glad/glad.h>

// Standard headers
#include <string>

// Constants
constexpr const int         WINDOW_WIDTH     = 1920;
constexpr const int         WINDOW_HEIGHT    = 1080;
const std::string           WINDOW_TITLE     = "Final Project";
constexpr const GLint       WINDOW_RESIZABLE = GL_FALSE;
constexpr const int         WINDOW_SAMPLES   = 16;

struct CommandLineOptions {
    bool enableMusic;
    bool enableAutoplay;
};
