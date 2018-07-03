#ifndef TRAIL_LIGHT_MODE_H
#define TRAIL_LIGHT_MODE_H

#define NO_ACTIVE_STEP 255

#include "Globals.h"
#include "AbstractLightMode.h"
   
class TrailLightMode : public AbstractLightMode {
  public:
    void onUpdate();
  private:
    uint8_t progress = 0;
    float stepBrightness[NUM_STEPS] = {};
    
    void readSensors();
    uint8_t findFirstActiveStep();
    uint8_t findLastActiveStep();
    void updateLeds();
    float getStepBrightness(uint8_t stepIndex);
    void setStepBrightness(uint8_t stepIndex, float brightness);
};

void TrailLightMode::onUpdate() {
    readSensors();
    updateLeds();
}

void TrailLightMode::readSensors() {
  uint8_t firstStep = findFirstActiveStep();
  uint8_t lastStep = findLastActiveStep();

  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    if (firstStep != NO_ACTIVE_STEP && (i >= firstStep && i <= lastStep)) {
      stepBrightness[i] -= 20.0f;
    } else {
      stepBrightness[i] += 1.0f;
    }

    stepBrightness[i] = constrain(stepBrightness[i], 0, 255);
  }
}

uint8_t TrailLightMode::findFirstActiveStep() {
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    if (digitalRead(stepPins[i]) == LOW) {
      return i;
    }
  }

  return NO_ACTIVE_STEP;
}

uint8_t TrailLightMode::findLastActiveStep() {
  for (uint8_t i = NUM_STEPS - 1; i >= 0; i--) {
    if (digitalRead(stepPins[i]) == LOW) {
      return i;
    }
  }

  return NO_ACTIVE_STEP;
}

void TrailLightMode::updateLeds() {
  fill_rainbow(leds, NUM_LEDS, progress++);
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    setStepBrightness(i, getStepBrightness(i));
  }
}

float TrailLightMode::getStepBrightness(uint8_t stepIndex) {
  uint8_t startIndex = (stepIndex == 0) ? 0 : stepIndex - 1;
  uint8_t endIndex = (stepIndex == NUM_STEPS - 1) ? NUM_STEPS - 1 : stepIndex + 1;

  float lowestBrightness = stepBrightness[startIndex];
  for (uint8_t i = startIndex; i <= endIndex; i++) {
    lowestBrightness = min(lowestBrightness, stepBrightness[i]);
  }

  return lowestBrightness;
}

void TrailLightMode::setStepBrightness(uint8_t stepIndex, float brightness) {
  StepLeds stepLeds = getStepLeds(stepIndex);
  for (uint8_t i = stepLeds.start; i <= stepLeds.end; i++) {
    leds[i].fadeToBlackBy(255 - brightness);
  }
}
   
#endif
