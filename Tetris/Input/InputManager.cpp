#include "InputManager.h"
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

InputManager::InputManager() {}
InputManager::~InputManager() {}

void InputManager::startUp() {}

void InputManager::shutDown() {}

void InputManager::pollKeyboardInputs(u32 kbCode, bool wasDown, bool isDown){
    if(wasDown != isDown){
    switch(kbCode){
        case 'W':
            controller.forward = isDown;
        break;
        case 'A':
            controller.backward = isDown;
        break;
        case 'S':
            controller.left = isDown;
        break;
        case 'D':
            controller.right = isDown;
        break;
        case 'J':
            controller.actButton = isDown;
        break;
        case 'K':
            controller.backButton = isDown;
        break;
        case 'L':
            controller.xButton = isDown;
        break;
        case 'M':
            controller.yButton = isDown;
        break;
        case VK_LEFT:
            controller.left = isDown;
            break;
        case VK_RIGHT:
            controller.right = isDown;
            break;
        case VK_UP:
            controller.forward = isDown;
            break;
        case VK_DOWN:
            controller.backward = isDown;
            break;
    }
    }
}

void InputManager::pollGamepadInputs() {
  DWORD dwResult;
  for (DWORD i = 0; i < XUSER_MAX_COUNT; i += 1) {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    dwResult = XInputGetState(i, &state);
    if (dwResult == ERROR_SUCCESS) {
      // Controller is connected
      controller.actButton = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
      controller.backButton = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
      controller.forward = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
      controller.backward = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
      controller.left = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
      controller.right = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
      controller.xButton = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
      controller.yButton = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
      XINPUT_VIBRATION vibe;
      vibe.wLeftMotorSpeed = 0;
      vibe.wRightMotorSpeed = 0;
      if (controller.actButton) {
          vibe.wLeftMotorSpeed = 60000;
          vibe.wRightMotorSpeed = 60000;
      }
      XInputSetState(i, &vibe);
    } else {
      // No Controller
    }
  }
  return;
}