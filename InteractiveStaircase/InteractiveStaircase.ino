#include <FastLED.h>

#define NUM_LEDS 150
#define SENSOR_PIN_START 2 // The first sensor pin
#define DATA_PIN_1 A0 // Left LED strip
#define DATA_PIN_2 A1 // Right LED strip
#define NUM_STEPS 12

struct Step {
  int led;
  float brightness;
};

// The centre LED of each step, with brightness initialised as 0
struct Step steps[] = {
  {5}, {14}, {24}, {33}, {43}, {52}, {62}, {71}, {81}, {90}, {100}, {109}
};

CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // Give the power supply time to stabilise.

  for (int i = 0; i < NUM_STEPS; i++) {
    pinMode(SENSOR_PIN_START + i, INPUT);
  }

  FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812B, DATA_PIN_2, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
}

void loop() {
  EVERY_N_MILLIS(10) {
    readSensors();
    updateLeds();
  }
}

void readSensors() {
  for (int i = SENSOR_PIN_START; i < SENSOR_PIN_START + NUM_STEPS; i++) {
    int sensorValue = digitalRead(i);

    if (sensorValue == LOW) {
      steps[i - SENSOR_PIN_START].brightness += 5.0f;
    } else {
      steps[i - SENSOR_PIN_START].brightness -= 1.0f;
    }

    steps[i - SENSOR_PIN_START].brightness = constrain(steps[i - SENSOR_PIN_START].brightness, 0, 255);
  }
}

void updateLeds() {
  for (int i = 0; i < NUM_STEPS; i++) {
    setStepColor(steps[i], CRGB(0, 0, round(steps[i].brightness)));
  }

  FastLED.show();
}

// Set a given step to the specified color
void setStepColor(Step step, CRGB color) {
  for (int i = step.led - 5; i < step.led + 5; i++) {
    setLed(i, color);
  }
}

// Set an LED color with bounds-checking to avoid writing outside the array
void setLed(int index, CRGB color) {
  if (index >= 0 && index < NUM_LEDS) {
    leds[index] = color;
  }
}

