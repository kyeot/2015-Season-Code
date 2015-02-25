#include <Adafruit_NeoPixel.h>

/*  Defines a constant used in all effect functions that 
*   controls how quickly the LED strip will update throughout
*   an effect's playback */
const int effectResolutionInMs = 10;

//Blink all lights in strip with same color
void pulseStrip(int delay, bool fade, uint32_t color);
//Chase lights down led strip
void simpleChase(int delay, int tailLength, bool fade, uint32_t color);
//Rainbow pattern across all lights
void rainbow(int delay, bool fade);
//Cycle rainbow light colors throuhgout chase
void rainbowChase(int delay, int tailLength, bool fade);
//Send random colors to each light
void randomLights(int delay, bool fade);

//Blink all of the lights in the strip
void pulseStrip(Adafruit_NeoPixel &pixelStrip, int duration, bool fade, uint32_t color) {
  
  /*  Begin looping through one iteration of the effect, and
  *   declar a time-tracking incremement variable */
  for (int time = 0; time < duration; time += effectResolutionInMs) {
    //Calculate progress through the effect and store it as a proportion
    float effectProportionComplete = (float)time / (float)duration;
    //Map current progress in effect to a sine wave, then an intensity value
    int currentIntensity = mapToWave(effectProportionComplete) * 255;
    /*  If fade mode is off, set the intensity to either 
    *   on or off based off effect progress */
    if (!fade) {
      currentIntensity = currentIntensity > (255 / 2) ? 200 : 0;
    }
    //Set the strip's state for this iteration of the effect
    setStrip(pixelStrip, currentIntensity, color);
    //Delay until the effect needs updating again
    delay(effectResolutionInMs);
  }
}

//Set all lights in Adafruit_NeoPixel array to same color
void setStrip(Adafruit_NeoPixel &pixelStrip, int intensity, uint32_t color) {
  
    for (int i = 0; i < pixelStrip.numPixels(); i++) {
        pixelStrip.setPixelColor(i, color);
    }
    pixelStrip.setBrightness(intensity);
    pixelStrip.show();
}

/*  Takes a value from 0 to 1 and returns a color corresponding
*   to the input value's position along a color spectrum. */
uint32_t colorSpectrum(float proportion) {
  
  //Convert the proportion into a byte, between 0-255
  byte WheelPosition = proportion * 255;
  //Use the position to create and return color values based on position
  WheelPosition = 255 - WheelPosition;
  if(WheelPosition < 85) {
    return Adafruit_NeoPixel::Color(255 - WheelPosition * 3, 0, WheelPosition * 3);
  } else if(WheelPosition < 170) {
    WheelPosition = WheelPosition - 85;
    return Adafruit_NeoPixel::Color(0, WheelPosition * 3, 255 - WheelPosition * 3);
  } else {
    WheelPosition = WheelPosition - 170;
    return Adafruit_NeoPixel::Color(WheelPosition * 3, 255 - WheelPosition * 3, 0);
  }
}

/*  Takes a linear proportion from 0 to 1 and returns a corresponding 
*   value between 0 and 1, related to the input's placement on a sine wave. 
*   Input = 0,     Output = 0
*   Input = 0.25,  Output = 0.5
*   Input = 0.5,   Output = 1
*   Input = 0.75,  Output = 0.5
*   Input = 1,     Output = 0 */
float mapToWave(float proportion) {
  
  float pi = 3.14159;
  float mappedProportion = (0.5 * sin((2 * pi * proportion) - (0.5 * pi))) + 0.5;
  return mappedProportion;
}
