#ifndef BRIDGE_LIGHT_MODE_H
#define BRIDGE_LIGHT_MODE_H

#include "Globals.h"
#include "AbstractLightMode.h"
   
class BridgeLightMode : public AbstractLightMode {
  public:
    void onUpdate();
  private:
    CRGB color;
    float stepBrightness[NUM_STEPS] = {};

    void readSensors();
    void updateLeds();
    void setStepColor(uint8_t stepIndex, CRGB color);
};

void BridgeLightMode::onUpdate() {
    readSensors();
    updateLeds();
}

void BridgeLightMode::readSensors() {
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    bool isActive = digitalRead(stepPins[i]) == LOW;
    stepBrightness[i] += (isActive ? 5.0f : -1.0f);
    stepBrightness[i] = constrain(stepBrightness[i], 0, 255);
  }
}

void BridgeLightMode::updateLeds() {
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    CRGB finalColor = CRGB(color);
    finalColor.fadeToBlackBy(255 - round(stepBrightness[i]));
    setStepColor(i, finalColor);
  }
}

// Set a given step to the specified color
void TrailLightMode::setStepColor(uint8_t stepIndex, CRGB color) {
  StepLeds stepLeds = getStepLeds(stepIndex);
  for (uint8_t i = stepLeds.start; i <= stepLeds.end; i++) {
    setLed(i, color);
  }
}
   
#endif
