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
      break;
    case SLOW_BLINK:
      pulseStrip(strip, 1000, false, stripMode.color);
      break;
    case FAST_BLINK:
      pulseStrip(strip, 400, false, stripMode.color);
      break;
    case SLOW_PULSE:
      pulseStrip(strip, 4000, true, stripMode.color);
      break;
    case FAST_PULSE:
      pulseStrip(strip, 1000, true, stripMode.color);
      break;
    case SIMPLE_CHASE:
      simpleChase(strip, 3000, 1, stripMode.color);
      break;
    case CHASE_WITH_TAIL:
      simpleChase(strip, 3000, 7, stripMode.color);
      break;
    case RAINBOW:
      rainbow(strip, 4000);
      break;
    default:
      break;
  } 
}

void i2cReceived(int bitsReceived) {
  
  digitalWrite(13, HIGH);;
  
  /* Read the initial byte from the message and 
  *  store it as the register bit */
  int registerId = Wire.read();
  //Serial.println(registerId);
  
  /* If the register byte is 42, extract LED data 
  *  and assign it to the ledMode global */
  if (registerId == 42) {
 
    int integerCount = 0;
    char* recievedChars = new char[Wire.available()+1];
    int recievedCharsLength = 0;
    
    RGB recievedRgb = {0, 0, 0};
        
    while(Wire.available()) {
      char recievedByte = Wire.read();
      
      recievedChars[recievedCharsLength] = recievedByte;
      recievedCharsLength++;
            
      if (recievedByte == *";") {
        integerCount++;
        
        int recievedInt = atoi(recievedChars);
        
        switch (integerCount) {
          case 1:
            stripMode.intensity = recievedInt;
            break;
          case 2: 
            recievedRgb.red = recievedInt;
            break;
          case 3: 
            recievedRgb.green = recievedInt;
            break;
          case 4: 
            recievedRgb.blue = recievedInt;
            break;          
          case 5:
            stripMode.effect = static_cast<LedEffect>(recievedInt);
            break;
        }
        memset(recievedChars, 0, strlen(recievedChars));
        recievedCharsLength = 0;
      } else if (recievedByte == *"\n") {
        stripMode.color = recievedRgb;
        memset(recievedChars, 0, strlen(recievedChars));
      }
    };
    
    delete[] recievedChars;
    recievedCharsLength = 0;
  }
}
