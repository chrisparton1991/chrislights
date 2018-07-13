#ifndef PARTICLE_LIGHT_MODE_H
#define PARTICLE_LIGHT_MODE_H

#define NUM_PARTICLES NUM_LEDS / 4 + 1
#define NUM_COLORS 4

#include "Globals.h"
#include "AbstractLightMode.h"

struct Particle {
  uint8_t startPosition;
  float position;
  float velocity;
  float acceleration;
};

CRGB particleColors[NUM_COLORS] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow};
   
class ParticleLightMode : public AbstractLightMode {
  public:
    ParticleLightMode();
    void onUpdate();
  private:
    Particle particles[NUM_PARTICLES] = {};
    void attractParticles(uint8_t attractPosition);
    void attractParticle(uint8_t particleIndex, uint8_t attractPosition);
    void applyPhysics(uint8_t particleIndex, bool hasAttractor);
    void renderParticle(Particle& particle, uint8_t particleIndex);
};

ParticleLightMode::ParticleLightMode() {
  for (uint8_t i = 0; i < NUM_PARTICLES; i++) {
    Particle &particle = particles[i];

    particle.startPosition = map(i, 0, NUM_PARTICLES, 0, NUM_LEDS - 1);
    particle.position = particle.startPosition;
  }
}

void ParticleLightMode::onUpdate() {
  bool hasAttractor = false;
  
  for (uint8_t i = 0; i < NUM_STEPS; i++) {
    bool isActive = digitalRead(stepPins[i]) == LOW;

    if (isActive) {
      hasAttractor = true;
      attractParticles(getStepLeds(i).middle);
    }
  }

  for (uint8_t i = 0; i < NUM_PARTICLES; i++) {
    applyPhysics(i, hasAttractor);
    renderParticle(particles[i], i);
  }
}

void ParticleLightMode::attractParticles(uint8_t attractPosition) {
  for (uint8_t i = 0; i < NUM_PARTICLES; i++) {
    attractParticle(i, attractPosition);
  }
}

void ParticleLightMode::attractParticle(uint8_t particleIndex, uint8_t attractPosition) {
  Particle& particle = particles[particleIndex];

  float distance = attractPosition - particle.position;
  float volatility = (particleIndex % 3 + 1) * 6;
  float direction = constrain(.5f / distance, -volatility, volatility);
  particle.acceleration += direction;
}

void ParticleLightMode::applyPhysics(uint8_t particleIndex, bool hasAttractor) {
  if (!hasAttractor) {
    Particle &particle = particles[particleIndex];

    float displacement = ceil(particle.startPosition - particle.position);
    particle.position += displacement / 20.0f;
    particle.velocity = 0;
    return;
  }

  Particle &particle = particles[particleIndex];
  particle.acceleration *= hasAttractor ? 1.0f : 3.0f;
  particle.velocity = constrain(particle.velocity, -.3f, .3f);
  particle.velocity += particle.acceleration;
  
  particle.position += particle.velocity;
  particle.position = constrain(particle.position, -10, NUM_LEDS + 10);
  particle.velocity *= .99f;
  particle.acceleration = 0;
}

void ParticleLightMode::renderParticle(Particle& particle, uint8_t particleIndex) {
  CRGB color = CRGB(particleColors[particleIndex % NUM_COLORS]);

  setLed(particle.position, color);
  setLed(particle.position + 1, color);
}
   
#endif
