#include <Wire.h>
#include <Adafruit_NeoPixel.h>

/***********************************
******** Definitions ***************
***********************************/

//Enumeration of LED strip effects, paired with integers corresponding to I2C messages
enum LedEffect {
  
  STATIC_COLOR = 0,
  
  SLOW_BLINK = 100,
  FAST_BLINK = 101, 
  SLOW_PULSE = 102, 
  FAST_PULSE = 103, 
  
  SIMPLE_CHASE = 200, 
  CHASE_WITH_TAIL = 201, 
  SMOOTH_SIMPLE_CHASE = 202, 
  SMOOTH_CHASE_WITH_TAIL = 203, 
  
  BLINK_RAINBOW = 300, 
  SMOOTH_RAINBOW = 301, 
  SMOOTH_RAINBOW_CHASE = 302, 
  
  BLINK_FULL_RANDOM = 400, 
  FADE_FULL_RANDOM = 401, 
  BLINK_RANDOM_INTENSITY = 402, 
  FADE_RANDOM_INTENSITY = 403, 
  BLINK_RANDOM_COLOR = 404, 
  FADE_RANDOM_COLOR = 405
};

//A data struct to hold the current status of the attached LED strip
struct LedMode {
  //Strip intensity, from 0-255
  int intensity;
  //Packed RGB color integer
  uint32_t color;
  //Specific identifier corresponding to a strip effect
  LedEffect effect;  
};

/***********************************
******** Globals *******************
***********************************/

/*   Global variable to store LED strip state
*    Will be set upon pertinent I2C message recieval, and 
*    is used in loop() to set the strip values */
LedMode stripMode;

//Robot LED strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(180, 6, NEO_GRB + NEO_KHZ800);

/***********************************
******** Arduino *******************
***********************************/

void setup() {

  //Set up I2C slave device on address 84
  Wire.begin(84);
  Wire.onReceive(i2cReceived);
  
  //Set up Serial communication for debugging output
  Serial.begin(9600);

  //Set up LED strip and initiate communication
  strip.begin();
  strip.setBrightness(255);
  strip.show();
  
  //Set up LED for testing
  pinMode(13, OUTPUT);
  
}

void loop() {
  
  switch(stripMode.effect) {
    case STATIC_COLOR:
      setStrip(strip, stripMode.intensity, stripMode.color);
      Serial.println("Static color");
      break;
    default:
      Serial.println("Other effect");
      break;
    
  }
}

void i2cReceived(int bitsReceived) {
  
  /* Read the initial byte from the message and 
  *  store it as the register bit */
  int registerId = Wire.read();
  Serial.println(registerId);
  
  /* If the register byte is 42, extract LED data 
  *  and assign it to the ledMode global */
  if (registerId == 42) {
    int intensity = Wire.read();
    Serial.println(intensity);
    int color = Wire.read();
    Serial.println(color);
    int effect = Wire.read();
    Serial.println(effect);
    
    stripMode.intensity = intensity;
    stripMode.color = color;
    stripMode.effect = static_cast<LedEffect>(effect);
  }
  
  Serial.println();
}
