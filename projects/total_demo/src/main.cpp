
#include <Wire.h>
#include <ESP.h>  // for restart()

// from common_main.cpp
extern void m_setup();
extern void m_loop();

void setup() {
  m_setup();
  Wire.begin();
  // place to add additional definitions such as class_carddb_init()
}

void loop() {
  m_loop();
  ESP.restart();
}