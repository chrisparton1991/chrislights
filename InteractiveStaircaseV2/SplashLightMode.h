#ifndef SPLASH_LIGHT_MODE_H
#define SPLASH_LIGHT_MODE_H

#include "Globals.h"
#include "AbstractLightMode.h"

#define FILL_DURATION 500
#define INACTIVE_STEP 0
#define NO_ACTIVE_STEPS UINT8_MAX
   
class SplashLightMode : public AbstractLightMode {
  public:
    void clearPreviousFrame();
    void onUpdate();
  private:
    uint32_t inactiveTime = 0;
    uint32_t stepTimes[NUM_STEPS] = {};
    CRGB stepColors[NUM_STEPS] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Gold, CRGB::Purple, CRGB::Orange, CRGB::Magenta, CRGB::Cyan, CRGB::LightCoral, CRGB::LightSeaGreen, CRGB::MidnightBlue, CRGB::MintCream};

    void readSensors();
    void updateLeds();
    uint8_t getActiveStepIndex();
    void setStepColor(uint8_t stepIndex, CRGB color);
};

void SplashLightMode::clearPreviousFrame() {
  // No-op, leave the previous frame data intact.
}

void SplashLightMode::onUpdate() {
    readSensors();
    updateLeds();
}

void SplashLightMode::readSensors() {
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    bool isActive = digitalRead(stepPins[i]) == LOW;
    bool wasActive = stepTimes[i] != INACTIVE_STEP;

    if (isActive) {
      stepTimes[i] = wasActive ? stepTimes[i] : millis();
    } else {
      stepTimes[i] = INACTIVE_STEP;
    }
  }
}

void SplashLightMode::updateLeds() {
  uint8_t activeStepIndex = getActiveStepIndex();
  uint32_t stepTime = 0;
  CRGB color = CRGB::Black;
  
  if (activeStepIndex == NO_ACTIVE_STEPS) {
    return; // Do nothing
//    inactiveStepTime = inactiveStepTime == 0 ? millis() : inactiveStepTime;
//    stepTime = inactiveStepTime;
  } else {
    inactiveTime = 0;
    stepTime = millis() - stepTimes[activeStepIndex];
    color = stepColors[activeStepIndex];
  }

  stepTime = constrain(stepTime, 0, FILL_DURATION);
  uint8_t distance = map(stepTime, 0, FILL_DURATION, 0, NUM_LEDS);
  uint8_t startLed = max(0, getStepLeds(activeStepIndex).middle - distance);
  uint8_t endLed = min(NUM_LEDS - 1, getStepLeds(activeStepIndex).middle + distance);

  for (uint8_t i = startLed; i <= endLed; i++) {
    setLed(i, color);
  }
}

uint8_t SplashLightMode::getActiveStepIndex() {
  uint16_t activeStepIndex = NO_ACTIVE_STEPS;
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    bool isStepActive = stepTimes[i] != INACTIVE_STEP;
    bool isFirstActiveStep = (activeStepIndex == NO_ACTIVE_STEPS);
    bool isStepMostRecent = stepTimes[i] > stepTimes[activeStepIndex];
    if (isStepActive && (isFirstActiveStep || isStepMostRecent)) {
      activeStepIndex = i;
    }
  }

  return activeStepIndex;
}
   
#endif
