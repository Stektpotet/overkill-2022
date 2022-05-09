#pragma once

#include <GLFW/glfw3.h>

namespace OK
{
    class Input
    {
        //enum class NavigationKey
        //{
        //    I = 0, J = 1, K = 2, L = 3, H = 4, Y = 5, //Flightcam controls
        //    N = 6, M = 7,
        //    W = 8, A = 9, S = 10, D = 11, Q = 12, E = 13, //glider controls
        //    MAX_KEYS = 14
        //};

    private:
        static const int ASCII_MAP_OFFSET = GLFW_KEY_SPACE;
        static const int ASCII_MAP_MAX = GLFW_KEY_Z;
        static const int ASCII_MAP_SIZE = ASCII_MAP_MAX - ASCII_MAP_OFFSET;

        static bool m_asciiMap[ASCII_MAP_SIZE];

        static bool m_isCursorCaptured;

        static bool m_leftButtonDown;
        static bool m_rightButtonDown;

    public:
        static double m_cursorY;       // CursorX and Y is [0, WINDOW_SIZE].
        static double m_cursorX;
        static float m_cursor_raw_deltaY;
        static float m_cursor_raw_deltaX;
        static float m_cursor_deltaY;
        static float m_cursor_deltaX;
        static float m_sensitivity;
        //static bool m_navKeyPressed[NavigationKey::MAX_KEYS];// Which navigation keys are pressed. WASD-QE keys.
        static bool m_shift; 
        static bool m_ctrl;

        static void OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods);
        static void OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods);
        static void OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods);
        static void OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
        static void OnCursorHover(GLFWwindow* window, double x, double y);
        static void OnScrollChange(GLFWwindow* window, double x, double y);
        static void OnMouseClick(GLFWwindow* window, int button, int action, int mods);

        static void clearMap();
        static bool getKey(int glfw_key);
    };

}
