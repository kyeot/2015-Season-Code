#include <Adafruit_NeoPixel.h>

//Set a single light to a color
void setLight(int led, uint32_t color);
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

//Set all lights in Adafruit_NeoPixel array to same color
void setStrip(Adafruit_NeoPixel &pixelStrip, int intensity, uint32_t color) {
  
    for (int i = 0; i < pixelStrip.numPixels(); i++) {
        pixelStrip.setPixelColor(i, color);
    }
    pixelStrip.setBrightness(intensity);
    pixelStrip.show();
}

/*  Takes a value from 0 to 1 and returns a color corresponding
*   to the input value's position along a colorspectrum */
uint32_t colorSpectrum(float proportion);

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
};
