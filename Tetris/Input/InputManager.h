#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <cstdint>
struct Controller {
  bool forward;
  bool backward;
  bool left;
  bool right;
  bool actButton;
  bool backButton;
  bool xButton;
  bool yButton;
  bool shlButton;
  bool shrButton;
};

class InputManager {

public:
  InputManager();
  ~InputManager();
  void startUp();
  void shutDown();
  void pollGamepadInputs();
  void pollKeyboardInputs(uint32_t kbCode, bool wasDown, bool isDown);
  Controller controller;
};
#endif