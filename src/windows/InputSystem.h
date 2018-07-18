#pragma once
#include "IInputSystem.h"
#include "ISingleton.h"
#include "ISubSystem.h"
#include "math/Vector.h"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>

namespace Nova
{
namespace Keys
{
const int KEY_A = GLFW_KEY_A;
const int KEY_B = GLFW_KEY_B;
const int KEY_C = GLFW_KEY_C;
const int KEY_D = GLFW_KEY_D;
const int KEY_E = GLFW_KEY_E;
const int KEY_F = GLFW_KEY_F;
const int KEY_G = GLFW_KEY_G;
const int KEY_H = GLFW_KEY_H;
const int KEY_I = GLFW_KEY_I;
const int KEY_J = GLFW_KEY_J;
const int KEY_K = GLFW_KEY_K;
const int KEY_L = GLFW_KEY_L;
const int KEY_M = GLFW_KEY_M;
const int KEY_N = GLFW_KEY_N;
const int KEY_O = GLFW_KEY_O;
const int KEY_P = GLFW_KEY_P;
const int KEY_Q = GLFW_KEY_Q;
const int KEY_R = GLFW_KEY_R;
const int KEY_S = GLFW_KEY_S;
const int KEY_T = GLFW_KEY_T;
const int KEY_U = GLFW_KEY_U;
const int KEY_V = GLFW_KEY_V;
const int KEY_W = GLFW_KEY_W;
const int KEY_X = GLFW_KEY_X;
const int KEY_Y = GLFW_KEY_Y;
const int KEY_Z = GLFW_KEY_Z;
const int KEY_0 = GLFW_KEY_0;
const int KEY_1 = GLFW_KEY_1;
const int KEY_2 = GLFW_KEY_2;
const int KEY_3 = GLFW_KEY_3;
const int KEY_4 = GLFW_KEY_4;
const int KEY_5 = GLFW_KEY_5;
const int KEY_6 = GLFW_KEY_6;
const int KEY_7 = GLFW_KEY_7;
const int KEY_8 = GLFW_KEY_8;
const int KEY_9 = GLFW_KEY_9;
const int KEY_SPACE = GLFW_KEY_SPACE;
const int KEY_ESC = GLFW_KEY_ESCAPE;
const int KEY_ENTER = GLFW_KEY_ENTER;
const int KEY_UNKOWN = GLFW_KEY_UNKNOWN;
const int KEY_F1 = GLFW_KEY_F1;
const int KEY_F2 = GLFW_KEY_F2;
const int KEY_F3 = GLFW_KEY_F3;
const int KEY_F4 = GLFW_KEY_F4;
const int KEY_F5 = GLFW_KEY_F5;
const int KEY_F6 = GLFW_KEY_F6;
const int KEY_F7 = GLFW_KEY_F7;
const int KEY_F8 = GLFW_KEY_F8;
const int KEY_F9 = GLFW_KEY_F9;
const int KEY_F10 = GLFW_KEY_F10;
const int KEY_F11 = GLFW_KEY_F11;
const int KEY_F12 = GLFW_KEY_F12;
const int KEY_LEFT = GLFW_KEY_LEFT;
const int KEY_RIGHT = GLFW_KEY_RIGHT;
const int KEY_UP = GLFW_KEY_UP;
const int KEY_DOWN = GLFW_KEY_DOWN;
const int KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
const int KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
const int KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
const int MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT;
const int MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
const int MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
}; // namespace Keys
// enum KeyState {pressed, released};
// void handleKeys(void* window);
// void handleMouse(void* window);
/*struct PairOfFloats
{
        PairOfFloats(float xp, float yp)
                :x(xp), y(yp) {}
        float x;
        float y;
};*/

class IKeyboardObserver
{
  public:
    virtual void onKeyPress() = 0;
};

template <typename T> class ISubject
{
  public:
    virtual void registerObserver(T *observer) = 0;
    virtual void removeObserver(T *observer) = 0;
    virtual void notifyObservers() = 0;
};

class InputSystem : public IInputSystem, public ISingleton<InputSystem>, public ISubSystem
{
  public:
    class Keyboard : public ISubject<IKeyboardObserver>
    {
      public:
        void registerObserver(IKeyboardObserver *observer) override;
        void removeObserver(IKeyboardObserver *observer) override;
        void notifyObservers() override;
        // const std::unordered_map<int, bool>& getState() const { return mState; };
        bool getKeyState(int keyCode) { return mState[keyCode]; };
        void setKeyState(int keyCode, bool keyState) { mState[keyCode] = keyState; };

      private:
        std::vector<IKeyboardObserver *> mObservers;
        std::unordered_map<int, bool> mState;
    };
    class Mouse
    {
      public:
        Mouse();
        bool getButtonState(int button) { return mButtonState[button]; };
        void setButtonState(int button, bool state) { mButtonState[button] = state; };
        // void setPosition(int x, int y) { mX = x; mY = y; };
        // std::array<int, 2> getCurrentPosition() { return std::array<int, 2>{mX, mY}; };
        float getSensitivity() { return mSensitivity; };
        Vec3 getPosition() const;
        Vec3 getMotionVector() const;
        // bool mouseMoved();// { if (mX == mPreviousX && mY == mPreviousY) return false; else
        // return true; }
        double mX = 0, mY = 0;
        double mPreviousX = 0, mPreviousY = 0;
        double wheel = 0;

      private:
        // bool mButtonState[3];
        // std::array<bool, 3> mButtonState;
        std::unordered_map<int, bool> mButtonState;
        float mSensitivity;
    };
    InputSystem() = default;
    ~InputSystem() = default;

    void startUp() override;
    void shutDown() override{};

    void processInputs() override;

    Keyboard &getKeyboard() { return mKeyboard; }
    Mouse &getMouse() { return mMouse; }
    bool getActionStatus(int action) { return mActions[action]; };
    void activateAction(int action) { mActions[action] = true; };
    void deActivateAction(int action) { mActions[action] = false; };
    const std::string &getKeyString(int key) { return mKeyMap[key]; };
    const std::string &getMouseButtonString(int button) { return mMouseMap[button]; };

  private:
    std::unordered_map<int, std::string> mKeyMap;
    std::unordered_map<int, std::string> mMouseMap;
    std::unordered_map<int, bool> mActions;
    Keyboard mKeyboard;
    Mouse mMouse;
};

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
} // namespace Nova
