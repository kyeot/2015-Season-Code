//Set all lights in Adafruit_NeoPixel array to same color
void setStrip(Adafruit_NeoPixel &pixelStrip, int intensity, uint32_t color) {

    for (int i = 0; i < pixelStrip.numPixels(); i++) {
        pixelStrip.setPixelColor(i, color);
    }
    pixelStrip.setBrightness(intensity);
    pixelStrip.show();
}
