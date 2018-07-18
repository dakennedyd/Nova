#include "windows/InputSystem.h"
#include "Crc.h"
#include "Settings.h"
#include "logger/Logger.h"
#include "math/Vector.h"
#include "windows/Window.h"
#include <GLFW/glfw3.h>

namespace Nova
{
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto &is = InputSystem::getInstance();
    auto &keyboard = InputSystem::getInstance().getKeyboard();
    const std::string &actionString(
        EngineSettings::getInstance().getString("Default-ActionContext", is.getKeyString(key)));

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        keyboard.setKeyState(key, true);
        keyboard.notifyObservers();
        is.activateAction(SID(actionString.c_str()));
    }
    else if (action == GLFW_RELEASE)
    {
        // LOG_DEBUG("key number:" << key << "pressed");
        keyboard.setKeyState(key, false);
        is.deActivateAction(SID(actionString.c_str()));
    };
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    auto &is = InputSystem::getInstance();
    auto &mouse = InputSystem::getInstance().getMouse();
    const std::string &actionString(EngineSettings::getInstance().getString(
        "Default-ActionContext", is.getMouseButtonString(button)));
    // if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    if (action == GLFW_PRESS)
    {
        mouse.setButtonState(button, true);
        is.activateAction(SID(actionString.c_str()));
    }
    else if (action == GLFW_RELEASE)
    {
        mouse.setButtonState(button, false);
        is.deActivateAction(SID(actionString.c_str()));
    };
}

void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    auto &mouse = InputSystem::getInstance().getMouse();
    mouse.wheel = yoffset;
}

static void mouseCursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    // auto &is = InputSystem::getInstance();
    auto &mouse = InputSystem::getInstance().getMouse();
    // mouse.setPosition(static_cast<int>(std::floor(xpos)),
    // static_cast<int>(std::floor(ypos)));
    // mouse.mPreviousX = mouse.mX;
    // mouse.mPreviousY = mouse.mY;
    // mouse.mX = static_cast<int>(std::floor(xpos));
    // mouse.mY = static_cast<int>(std::floor(ypos));
    mouse.mX = xpos;
    mouse.mY = ypos;
}

void InputSystem::startUp()
{
    LOG_INFO("Initiating input subsystem");
    auto &window = Window::getInstance();
    glfwSetKeyCallback(window.mGLFWindow, keyCallback);
    glfwSetMouseButtonCallback(window.mGLFWindow, mouseButtonCallback);
    glfwSetCursorPosCallback(window.mGLFWindow, mouseCursorPosCallback);
    glfwSetScrollCallback(window.mGLFWindow, mouseScrollCallback);

    mKeyMap[Keys::KEY_A] = "key_a";
    mKeyMap[Keys::KEY_B] = "key_b";
    mKeyMap[Keys::KEY_C] = "key_c";
    mKeyMap[Keys::KEY_D] = "key_d";
    mKeyMap[Keys::KEY_E] = "key_e";
    mKeyMap[Keys::KEY_F] = "key_f";
    mKeyMap[Keys::KEY_G] = "key_g";
    mKeyMap[Keys::KEY_H] = "key_h";
    mKeyMap[Keys::KEY_I] = "key_i";
    mKeyMap[Keys::KEY_J] = "key_j";
    mKeyMap[Keys::KEY_K] = "key_k";
    mKeyMap[Keys::KEY_L] = "key_l";
    mKeyMap[Keys::KEY_M] = "key_m";
    mKeyMap[Keys::KEY_N] = "key_n";
    mKeyMap[Keys::KEY_O] = "key_o";
    mKeyMap[Keys::KEY_P] = "key_p";
    mKeyMap[Keys::KEY_Q] = "key_q";
    mKeyMap[Keys::KEY_R] = "key_r";
    mKeyMap[Keys::KEY_S] = "key_s";
    mKeyMap[Keys::KEY_T] = "key_t";
    mKeyMap[Keys::KEY_U] = "key_u";
    mKeyMap[Keys::KEY_V] = "key_v";
    mKeyMap[Keys::KEY_W] = "key_w";
    mKeyMap[Keys::KEY_X] = "key_x";
    mKeyMap[Keys::KEY_Y] = "key_y";
    mKeyMap[Keys::KEY_Z] = "key_z";
    mKeyMap[Keys::KEY_UP] = "key_up";
    mKeyMap[Keys::KEY_DOWN] = "key_down";
    mKeyMap[Keys::KEY_LEFT] = "key_left";
    mKeyMap[Keys::KEY_RIGHT] = "key_right";
    mKeyMap[Keys::KEY_SPACE] = "key_space";
    mKeyMap[Keys::KEY_0] = "key_0";
    mKeyMap[Keys::KEY_1] = "key_1";
    mKeyMap[Keys::KEY_2] = "key_2";
    mKeyMap[Keys::KEY_3] = "key_3";
    mKeyMap[Keys::KEY_4] = "key_4";
    mKeyMap[Keys::KEY_5] = "key_5";
    mKeyMap[Keys::KEY_6] = "key_6";
    mKeyMap[Keys::KEY_7] = "key_7";
    mKeyMap[Keys::KEY_8] = "key_8";
    mKeyMap[Keys::KEY_9] = "key_9";
    mKeyMap[Keys::KEY_F1] = "key_f1";
    mKeyMap[Keys::KEY_F2] = "key_f2";
    mKeyMap[Keys::KEY_F3] = "key_f3";
    mKeyMap[Keys::KEY_F4] = "key_f4";
    mKeyMap[Keys::KEY_F5] = "key_f5";
    mKeyMap[Keys::KEY_F6] = "key_f6";
    mKeyMap[Keys::KEY_F7] = "key_f7";
    mKeyMap[Keys::KEY_F8] = "key_f8";
    mKeyMap[Keys::KEY_F9] = "key_f9";
    mKeyMap[Keys::KEY_F10] = "key_f10";
    mKeyMap[Keys::KEY_F11] = "key_f11";
    mKeyMap[Keys::KEY_F12] = "key_f12";

    mMouseMap[Keys::MOUSE_LEFT] = "mouse_left";
    mMouseMap[Keys::MOUSE_MIDDLE] = "mouse_middle";
    mMouseMap[Keys::MOUSE_RIGHT] = "mouse_right";
}

void InputSystem::processInputs()
{
    glfwPollEvents();
    // auto &window = Window::getInstance();
    // handleKeys(window.mGLFWindow);
    // if(!window.isCursorShown()) handleMouse(window.mGLFWindow);
}

/*const float moveSpeed = 0.05f;
const float turnSpeed = 2.0f;
bool wireframeOn = false;*/

/*void handleKeys(void* window)
{
        FPSCamera* pCCamera = (FPSCamera*)GraphicsSystem::getInstance().getCurrentCamera();
        if (GLFW_PRESS == glfwGetKey((GLFWwindow*)window, GLFW_KEY_A))
        {
                pCCamera->moveLeft(moveSpeed);
        }
        if (GLFW_PRESS == glfwGetKey((GLFWwindow*)window, GLFW_KEY_D))
        {
                pCCamera->moveRight(moveSpeed);
        }
        if (GLFW_PRESS == glfwGetKey((GLFWwindow*)window, GLFW_KEY_W))
        {
                pCCamera->moveForward(moveSpeed);
        }
        if (GLFW_PRESS == glfwGetKey((GLFWwindow*)window, GLFW_KEY_S))
        {
                pCCamera->moveBackwards(moveSpeed);
        }
        if (GLFW_PRESS == glfwGetKey((GLFWwindow*)window, GLFW_KEY_SPACE))
        {
                pCCamera->moveUp(moveSpeed);
        }
        if (GLFW_PRESS == glfwGetKey((GLFWwindow*)window, GLFW_KEY_X))
        {
                pCCamera->moveDown(moveSpeed);
        }
}*/

/*float previousMouseX = 0.0f;
float previousMouseY = 0.0f;
//glfwGetCursorPos((GLFWwindow*)window, &previousMouseX, &previousMouseY);
float mouseSensitivity = 0.2f;
float mouseXDelta = 0.0f;
float mouseYDelta = 0.0f;
Vec3 dir;
void handleMouse(void* window)
{
        FPSCamera* pCCamera = (FPSCamera*)GraphicsSystem::getInstance().getCurrentCamera();
        double xpos = 0, ypos = 0;
        glfwGetCursorPos((GLFWwindow*)window, &xpos, &ypos);
        //LOG("Mouse:X->" << xpos << " Y->" << ypos);
        mouseXDelta = static_cast<float>(xpos) - previousMouseX;
        mouseYDelta = static_cast<float>(ypos) - previousMouseY;
        pCCamera->rotate(pCCamera->getUp(), toRadians(mouseXDelta * mouseSensitivity));
//this is ugly

        dir.rotateSelf(pCCamera->getLeft(), -toRadians(mouseYDelta * mouseSensitivity));
        if (dir.getY() < 0.99f && dir.getY() > -0.99f)	//lock the camera so it doesn't fail - this
is a ugly hack....
        {
                pCCamera->rotate(pCCamera->getLeft(), -toRadians(mouseYDelta * mouseSensitivity));
        }
        dir = pCCamera->getDirection();
        //dir.debugPrint();
        previousMouseX = static_cast<float>(xpos);
        previousMouseY = static_cast<float>(ypos);
}*/

void InputSystem::Keyboard::registerObserver(IKeyboardObserver *observer)
{
    mObservers.push_back(observer);
}

void InputSystem::Keyboard::removeObserver(IKeyboardObserver *observer)
{
    for (int i = 0; i < mObservers.size(); i++)
    {
        if (mObservers[i] == observer)
        {
            mObservers.erase(mObservers.begin() + i);
        }
    }
}

void InputSystem::Keyboard::notifyObservers()
{
    for (IKeyboardObserver *observer : mObservers)
    {
        observer->onKeyPress();
    }
}
InputSystem::Mouse::Mouse()
{
    float s = EngineSettings::getInstance().getInteger("Input", "mouse_sensitivity") * 0.0001f;
    if (s >= 0.01f)
    {
        mSensitivity = 0.01f;
    }
    else if (s <= 0.0001f)
    {
        mSensitivity = 0.0001f;
    }
    else
    {
        mSensitivity = s;
    }
}
Vec3 InputSystem::Mouse::getPosition() const
{
    /*double x, y;
    glfwGetCursorPos(Window::getInstance().mGLFWindow, &x, &y);
    return PairOfFloats(static_cast<float>(x), static_cast<float>(y));*/
    return Vec3(static_cast<float>(mX), static_cast<float>(mY), 0.0f);
}
Vec3 InputSystem::Mouse::getMotionVector() const
{
    return Vec3(static_cast<float>(mX - mPreviousX), static_cast<float>(mY - mPreviousY), 0.0f);
}
} // namespace Nova