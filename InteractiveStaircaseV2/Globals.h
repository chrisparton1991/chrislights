#ifndef GLOBALS_H
#define GLOBALS_H

#define NUM_LEDS 124
#define NUM_STEPS 12
#define MODE_CHANGE_PIN A2
#define DATA_PIN_1 A0 // Left LED strip
#define DATA_PIN_2 A1 // Right LED strip

struct StepLeds {
  uint8_t start;
  uint8_t middle;
  uint8_t end;
};

CRGB leds[NUM_LEDS];
uint8_t stepPins[NUM_LEDS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

uint8_t getStepStartLed(uint8_t stepIndex) {
  return round((float) stepIndex * NUM_LEDS / NUM_STEPS);
}

StepLeds getStepLeds(uint8_t stepIndex) {
  uint8_t start = getStepStartLed(stepIndex);
  uint8_t end = getStepStartLed(stepIndex + 1) - 1;
  return {start, start + (end - start) / 2, end};
}

// Set an LED color with bounds-checking to avoid writing outside the array
void setLed(uint8_t index, CRGB color) {
  if (index >= 0 && index < NUM_LEDS) {
    leds[index] = color;
  }
}

#endif
