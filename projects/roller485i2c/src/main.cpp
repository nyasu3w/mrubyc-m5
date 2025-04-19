
#include <Wire.h>
#include <ESP.h>  // for restart()
#include "c_roller485.h"

// from common_main.cpp
extern void m_setup();
extern void m_loop();

void setup() {
  m_setup();
  Wire.begin();
  class_roller485_init();
}

void loop() {
  m_loop();
  ESP.restart();
}