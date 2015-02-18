#include <Wire.h>
#include <Adafruit_NeoPixel.h>

struct LedMode {
  
  //Strip intensity, from 0-255
  int intensity;
  
  //Packed RGB color integer
  uint32_t color;
  
  //Specific identifier corresponding to a strip effect
  int effect;  
};

/*   Global variable to store LED strip state
*    Will be set upon pertinent I2C message recieval, and 
*    is used in loop() to set the strip values */
LedMode stripMode;

//Robot LED strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(180, 6, NEO_GRB + NEO_KHZ800);

void setup() {

  //Set up I2C slave device on address 84
  Wire.begin(84);
  Wire.onReceive(i2cRecieved);
  
  //Set up Serial communication for debugging output
  Serial.begin(9600);

  //Set up LED strip and initiate communication
  strip.begin();
  strip.show();
  
  //Set up LED for testing
  pinMode(13, OUTPUT);
  
}

void loop() {
  delay(100);
}

void i2cRecieved(int bitsRecieved) {
  
  int registerId = Wire.read();
  Serial.println(registerId);
  
  //Read all but one availible bytes
  while (Wire.available() > 0) {
    
    //Read each byte
    char c = Wire.read();
    //Print each byte to connected Serial bus
    Serial.println(c);
        
    digitalWrite(13, HIGH);
    
    delay(900);
  }
  
  Serial.println();
  
 /* //Read the final byte as an integer
  int finalByte = Wire.read();
  //Print it to connected Serial devices
  Serial.println(finalByte);*/
  
  digitalWrite(13, LOW);
}


//Set all lights in Adafruit_NeoPixel array to same color
void SetStrip(Adafruit_NeoPixel &pixelStrip, uint32_t color) {

    for (int i = 0; i < pixelStrip.numPixels(); i++) {
        pixelStrip.setPixelColor(i, color);
    }
    pixelStrip.show();
}
