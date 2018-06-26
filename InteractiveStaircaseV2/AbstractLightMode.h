#ifndef ABSTRACT_LIGHT_MODE_H
#define ABSTRACT_LIGHT_MODE_H
   
class AbstractLightMode {
  public:
    virtual void onUpdate() = 0;
    virtual void clearPreviousFrame() {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
    }
};
   
#endif
