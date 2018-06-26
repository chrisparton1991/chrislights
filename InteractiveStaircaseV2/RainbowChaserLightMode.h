#ifndef BRIDGE_LIGHT_MODE_H
#define BRIDGE_LIGHT_MODE_H

#define NO_ACTIVE_STEP 255

#include "Globals.h"
#include "AbstractLightMode.h"
   
class RainbowChaserLightMode : public AbstractLightMode {
  public:
    void onUpdate();
  private:
    uint8_t progress = 0;
    float stepBrightness[NUM_STEPS] = {};
    
    void readSensors();
    uint8_t findFirstActiveStep();
    uint8_t findLastActiveStep();
    void updateLeds();
    void setStepBrightness(uint8_t stepIndex, float brightness);
};

void RainbowChaserLightMode::onUpdate() {
    readSensors();
    updateLeds();
}

void RainbowChaserLightMode::readSensors() {
  uint8_t firstStep = findFirstActiveStep();
  uint8_t lastStep = findLastActiveStep();

  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    if (firstStep != NO_ACTIVE_STEP && (i >= firstStep && i <= lastStep)) {
      stepBrightness[i] -= 30.0f;
    } else {
      stepBrightness[i] += 30.0f;
    }

    stepBrightness[i] = constrain(stepBrightness[i], 0, 255);
  }
}

uint8_t RainbowChaserLightMode::findFirstActiveStep() {
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    if (digitalRead(stepPins[i]) == LOW) {
      return i;
    }
  }

  return NO_ACTIVE_STEP;
}

uint8_t RainbowChaserLightMode::findLastActiveStep() {
  for (uint8_t i = NUM_STEPS - 1; i >= 0; i--) {
    if (digitalRead(stepPins[i]) == LOW) {
      return i;
    }
  }

  return NO_ACTIVE_STEP;
}

void RainbowChaserLightMode::updateLeds() {
  fill_rainbow(leds, NUM_LEDS, progress++);
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    setStepBrightness(i, stepBrightness[i]);
  }
}

void RainbowChaserLightMode::setStepBrightness(uint8_t stepIndex, float brightness) {
  StepLeds stepLeds = getStepLeds(stepIndex);
  for (uint8_t i = stepLeds.start; i <= stepLeds.end; i++) {
    leds[i].fadeToBlackBy(255 - brightness);
  }
}
   
#endif