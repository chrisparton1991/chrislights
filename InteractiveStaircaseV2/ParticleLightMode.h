#ifndef PARTICLE_LIGHT_MODE_H
#define PARTICLE_LIGHT_MODE_H

#define NUM_PARTICLES 30
#define NUM_COLORS 4

#include "Globals.h"
#include "AbstractLightMode.h"

struct Particle {
  float position;
  float velocity;
  float acceleration;
  uint8_t distanceFromAttractor;
};

CRGB particleColors[NUM_COLORS] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow};
   
class ParticleLightMode : public AbstractLightMode {
  public:
    ParticleLightMode();
    void clearPreviousFrame();
    void onUpdate();
  private:
    Particle particles[NUM_PARTICLES] = {};
    void attractParticles(uint8_t attractPosition);
    void applyPhysics(Particle& particle);
    void renderParticle(Particle& particle, uint8_t particleIndex);
};

ParticleLightMode::ParticleLightMode() {
  for (uint8_t i = 0; i < NUM_PARTICLES; i++) {
    particles[i].position = random8(NUM_LEDS);
  }
}

void ParticleLightMode::clearPreviousFrame() {
  fadeToBlackBy(leds, NUM_LEDS, 128); // Simulate particle trails by darkening the previous frame data.
}

void ParticleLightMode::onUpdate() {
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    bool isActive = digitalRead(stepPins[i]) == LOW;

    if (isActive) {
      attractParticles(getStepLeds(i).middle);
    }
  }

  for (uint8_t i = 0; i < NUM_PARTICLES; i++) {
    applyPhysics(particles[i]);
    renderParticle(particles[i], i);
  }
}

void ParticleLightMode::attractParticles(uint8_t attractPosition) {
  for (uint8_t i = 0; i < NUM_PARTICLES; i++) {
    Particle& particle = particles[i];

    float distance = attractPosition - particle.position;
    float volatility = (i % 3 + 2) * 4;
    float direction = constrain(.5f / distance, -volatility, volatility);
    particle.acceleration += direction;
  }
}

void ParticleLightMode::applyPhysics(Particle& particle) {
  particle.velocity = constrain(particle.velocity, -.5f, .5f);
  particle.velocity += particle.acceleration;
  particle.position += particle.velocity;
  particle.position = constrain(particle.position, -10, NUM_LEDS + 10);
  particle.acceleration = 0;
  particle.velocity *= .99f;
}

void ParticleLightMode::renderParticle(Particle& particle, uint8_t particleIndex) {
  float position = round(particle.position);

  uint8_t brightness = UINT8_MAX;
  float absVelocity = abs(particle.velocity);
  if (absVelocity <= .5f) {
    brightness = floor(absVelocity / .5f * UINT8_MAX);
  }

  CRGB color = CRGB(particleColors[particleIndex % NUM_COLORS]).fadeToBlackBy(UINT8_MAX - brightness);
  setLed(particle.position, color);
}
   
#endif
