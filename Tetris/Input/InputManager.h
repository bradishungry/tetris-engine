#pragma once
#include "definitions.h"
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
  void pollKeyboardInputs(u32 kbCode, bool wasDown, bool isDown);
  Controller controller;
};
