#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#include "LedControl.h"

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
  
  RAINBOW = 300
};

//A data struct to hold the current status of the attached LED strip
struct LedMode {
  //Strip intensity, from 0-255
  int intensity;
  //RGB color struct
  RGB color;
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
  
  /*switch(stripMode.effect) {
    case STATIC_COLOR:
      setStrip(strip, stripMode.intensity, stripMode.color);
      Serial.println("Static color");
      break;
    default:
      Serial.println("Other effect");
      break;
    
  } */
  
  RGB blue = {35, 0, 250};
  RGB red = {250, 0, 0};
  
  //Pulse the strip in KYEOT fashion!
  pulseStrip(strip, 1500, true, blue);
  pulseStrip(strip, 1500, true, red);
  rainbow(strip, 3000);
  simpleChase(strip, 6000, 7, blue);
  simpleChase(strip, 6000, 7, red);
  rainbow(strip, 3000);
}

void i2cReceived(int bitsReceived) {
  
  /* Read the initial byte from the message and 
  *  store it as the register bit */
  int registerId = Wire.read();
  Serial.println("Register: " + registerId);
  
  /* If the register byte is 42, extract LED data 
  *  and assign it to the ledMode global */
  if (registerId == 42) {
    /*int intensity = Wire.read();
    Serial.println("Intensity: " + intensity);
    int color = Wire.read();
    Serial.println("Color: " + color);
    int effect = Wire.read();
    Serial.println("Effect: " + effect);
    
    stripMode.intensity = intensity;
    stripMode.color = color;
    stripMode.effect = static_cast<LedEffect>(effect);*/
    
    while(Wire.available()) {
      char recievedByte = Wire.read();
      Serial.print(" ");
      Serial.print(recievedByte);
      Serial.print(" ");
    }
  }
  
  Serial.println();
}
