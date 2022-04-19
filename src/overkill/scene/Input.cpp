#include "Input.hpp"
#include <iostream>
#include <glm/common.hpp>
#include <glad/glad.h>
#include <utilities/window.hpp>

namespace OK
{
    bool Input::m_isCursorCaptured = false;

    bool Input::m_leftButtonDown = false;
    bool Input::m_rightButtonDown = false;

    double Input::m_cursorX = WINDOW_WIDTH * 0.5;
    double Input::m_cursorY = WINDOW_HEIGHT * 0.5;
    float Input::m_cursor_deltaY = 0;
    float Input::m_cursor_deltaX = 0;
    float Input::m_cursor_raw_deltaY = 0;
    float Input::m_cursor_raw_deltaX = 0;
    float Input::m_sensitivity = 0.5;
    //bool  Input::m_navKeyPressed[] = {
    //    false, false, false, false, false, false, //cam
    //    false, false, //zoom
    //    false, false, false, false, false, false //glider
    //}; // Which navigation keys are pressed. WASD-QE keys. -> but now IJKL-YH

    bool  Input::m_shift{ false };

    bool  Input::m_asciiMap[Input::ASCII_MAP_SIZE]; //keyboard map from ascii value 32 -> 90

    void Input::clearMap()
    {
        for (int i = 0; i < Input::ASCII_MAP_SIZE; i++)
        {
            m_asciiMap[i] = false; //reset previous input state
        }
    }

    void Input::OnInputKeyPress(GLFWwindow* window, int keyCode, int /*scanCode*/, int mods)
    {
        // LOG_DEBUG("Pressing %i, as char: %c\n", keyCode, char(keyCode));
        int index = keyCode - Input::ASCII_MAP_OFFSET;
        if ((index < Input::ASCII_MAP_SIZE))
        {
            m_asciiMap[index] = true;
        }
        switch (keyCode)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, 1);
            break;

        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            m_shift = true;
            break;
        //
        //case GLFW_KEY_I:
        //    m_navKeyPressed[I] = true;
        //    break;
        //case GLFW_KEY_K:
        //    m_navKeyPressed[K] = true;
        //    break;
        //case GLFW_KEY_L:
        //    m_navKeyPressed[L] = true;
        //    break;
        //case GLFW_KEY_J:
        //    m_navKeyPressed[J] = true;
        //    break;
        //case GLFW_KEY_H:
        //    m_navKeyPressed[H] = true;
        //    break;
        //case GLFW_KEY_Y:
        //    m_navKeyPressed[Y] = true;
        //    break;

        //    //ZOOM
        //case GLFW_KEY_N:
        //    m_navKeyPressed[N] = true;
        //    break;
        //case GLFW_KEY_M:
        //    m_navKeyPressed[M] = true;
        //    break;

        //    //GLIDER CONTROLS
        //case GLFW_KEY_W:
        //    m_navKeyPressed[W] = true;
        //    break;
        //case GLFW_KEY_A:
        //    m_navKeyPressed[A] = true;
        //    break;
        //case GLFW_KEY_S:
        //    m_navKeyPressed[S] = true;
        //    break;
        //case GLFW_KEY_D:
        //    m_navKeyPressed[D] = true;
        //    break;
        //case GLFW_KEY_Q:
        //    m_navKeyPressed[Q] = true;
        //    break;
        //case GLFW_KEY_E:
        //    m_navKeyPressed[E] = true;
        //    break;
        }

    }

    void Input::OnInputKeyHold(GLFWwindow* /*window*/, int /*keyCode*/, int /*scanCode*/, int /*mods*/)
    {
        // LOG_DEBUG("Holding %i, as char: %c\n", keyCode, char(keyCode));
    }

    void Input::OnInputKeyUnpress(GLFWwindow* /*window*/, int keyCode, int /*scanCode*/, int mods)
    {
        // LOG_DEBUG("Unpressed %i, as char: %c\n", keyCode, char(keyCode));
        int index = keyCode - Input::ASCII_MAP_OFFSET;
        if ((index < Input::ASCII_MAP_SIZE))
        {
            m_asciiMap[index] = false;
        }
        switch (keyCode)
        {
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            m_shift = false;
            break;
        }
    }

    void Input::OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
            OnInputKeyPress(window, keyCode, scanCode, mods);
            break;
        case GLFW_REPEAT:
            OnInputKeyHold(window, keyCode, scanCode, mods);
            break;
        case GLFW_RELEASE:
            OnInputKeyUnpress(window, keyCode, scanCode, mods);
            break;
        }
    }

    void Input::OnCursorHover(GLFWwindow* window, double x, double y)
    {
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        m_cursor_raw_deltaX = float(x - Input::m_cursorX);
        m_cursor_raw_deltaY = float(y - Input::m_cursorY);

        Input::m_cursorX = x;
        Input::m_cursorY = y;

        m_cursor_deltaX = m_cursor_raw_deltaX * m_sensitivity; 
        m_cursor_deltaY = m_cursor_raw_deltaY * m_sensitivity;
    }                   

    void Input::OnScrollChange(GLFWwindow* /*window*/, double /*x*/, double /*y*/)
    {
        //    printf("Scroll: x: %f,\ty:%f\t\tfovy:%f\n", x, y, m_fovy);
    }

    void Input::OnMouseClick(GLFWwindow* window, int button, int action, int /*mods*/)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
#ifdef __APPLE__
                m_isCursorCaptured = true;
#endif
                // printf("Left button pressed.\n");
                m_leftButtonDown = true;
            }
            else if (action == GLFW_RELEASE)
            {

#ifdef __APPLE__
                m_isCursorCaptured = false;
#endif
                // printf("Left button released.\n");       
                m_leftButtonDown = false;
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
#ifndef __APPLE__
                if (m_isCursorCaptured)
                {
                    m_isCursorCaptured = false;
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                else
                {
                    m_isCursorCaptured = true;
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
#endif
                // printf("Right button pressed.\n");
                m_rightButtonDown = true;
            }
            else if (action == GLFW_RELEASE)
            {
                // printf("Right button released.\n");       
                m_rightButtonDown = false;
            }
        }
    }
    bool Input::getKey(const int glfw_key)
    {
        int index = glfw_key - Input::ASCII_MAP_OFFSET;
        if (index > Input::ASCII_MAP_MAX || index < 0)
        {
            std::cout << "WARN: [Input] Pressed an untracked key: " << glfw_key << std::endl;
            return false;
        }
        return Input::m_asciiMap[index];
    }

}
