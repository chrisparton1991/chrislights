#include <FastLED.h>
#include "Globals.h"
#include "AbstractLightMode.h"
#include "FireLightMode.h"
#include "ParticleLightMode.h"
#include "RainbowChaserLightMode.h"
#include "SplashLightMode.h"
#include "TrailLightMode.h"

#define NUM_LIGHT_MODES 5

bool modeChangePressed = false;
uint8_t mode = 0;
uint32_t lastMillis = 0;
AbstractLightMode* lightMode = 0;

void setup() {
  delay(3000); // Give the power supply time to stabilise.
  Serial.begin(115200);

  pinMode(MODE_CHANGE_PIN, INPUT_PULLUP);
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    pinMode(stepPins[i], INPUT);
  }

  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW); // Use A5 as a ground pin.

  FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812B, DATA_PIN_2, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

  setLightMode(0);
}

void loop() {
  EVERY_N_MILLIS(50) {
    bool modeButtonState = digitalRead(MODE_CHANGE_PIN) == LOW;
    if (modeChangePressed && !modeButtonState) {
      mode = (mode + 1) % NUM_LIGHT_MODES;
      setLightMode(mode);
    }

    modeChangePressed = modeButtonState;
  }

  uint32_t elapsedMs = millis() - lastMillis;
  uint8_t minMs = 1000 / lightMode->getFps();
  
  if (elapsedMs >= minMs) {
    lastMillis = millis();
    lightMode->clearPreviousFrame();
    lightMode->onUpdate();
    FastLED.show();
  }
}

void setLightMode(int modeIndex) {
  if (lightMode) {
    delete lightMode;
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  if (modeIndex == 0) {
    lightMode = new TrailLightMode();
  } else if (modeIndex == 1) {
    lightMode = new RainbowChaserLightMode();
  } else if (modeIndex == 2) {
    lightMode = new ParticleLightMode();
  } else if (modeIndex == 3) {
    lightMode = new SplashLightMode();
  } else if (modeIndex == 4) {
    lightMode = new FireLightMode();
  }
}

