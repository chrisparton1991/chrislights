#include <FastLED.h>
#include "Globals.h"
#include "AbstractLightMode.h"
#include "ParticleLightMode.h"
#include "RainbowChaserLightMode.h"
#include "SplashLightMode.h"
#include "TrailLightMode.h"

#define NUM_LIGHT_MODES 4

bool modeChangePressed = false;
uint8_t mode = 0;
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

  EVERY_N_MILLIS(10) {
    lightMode->clearPreviousFrame();
    lightMode->onUpdate();
    FastLED.show();
  }
}

void setLightMode(int modeIndex) {
  if (lightMode) {
    delete lightMode;
  }

  if (modeIndex == 0) {
    lightMode = new TrailLightMode(CRGB::Blue);
  } else if (modeIndex == 1) {
    lightMode = new RainbowChaserLightMode();
  } else if (modeIndex == 2) {
    lightMode = new ParticleLightMode();
  } else if (modeIndex == 3) {
    lightMode = new SplashLightMode();
  }
}

