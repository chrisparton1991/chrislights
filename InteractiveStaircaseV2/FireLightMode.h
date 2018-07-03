#ifndef FIRE_LIGHT_MODE_H
#define FIRE_LIGHT_MODE_H

#include "Globals.h"
#include "AbstractLightMode.h"

#define COOLING  50
#define SPARKING 200

// Based on https://raw.githubusercontent.com/FastLED/FastLED/master/examples/Fire2012/Fire2012.ino
class FireLightMode : public AbstractLightMode {
  public:
    uint8_t getFps();
    void onUpdate();
  private:
    uint8_t heat[NUM_LEDS] = {};
    
    void coolLeds();
    void diffuseHeat();
    uint16_t getHeat(uint8_t ledIndex);
    void addSparks();
    void updateLeds();
};

uint8_t FireLightMode::getFps() {
   return 30; // The fire looks better at a lower FPS.
}

void FireLightMode::onUpdate() {
  coolLeds();
  diffuseHeat();
  addSparks();
  updateLeds(); 
}

void FireLightMode::coolLeds() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }
}

void FireLightMode::diffuseHeat() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    heat[i] = (
      getHeat(i - 1) + getHeat(i - 2) + getHeat(i - 2) +
      getHeat(i + 1) + getHeat(i + 2) + getHeat(i + 2)
    ) / 6;
  }
}

uint16_t FireLightMode::getHeat(uint8_t ledIndex) {
  if (ledIndex < 0 || ledIndex >= NUM_LEDS) {
    return 0;
  } else {
    return heat[ledIndex];
  }
}

void FireLightMode::addSparks() {
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    bool isActive = digitalRead(stepPins[i]) == LOW;
    if (isActive && random8() < SPARKING) {
      uint8_t ledIndex = random8(getStepLeds(i).start, getStepLeds(i).end);
      heat[ledIndex] = qadd8(heat[ledIndex], random8(200,255));
    }
  }
}

void FireLightMode::updateLeds() {
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = HeatColor(heat[i]);
  }
}
   
#endif
