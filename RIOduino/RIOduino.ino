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
 
    //Set up variables to keep track of incoming data
    //Total semicolon-delimeted integers recieved so far
    int integerCount = 0;
    //Array to store charecters since the last semicolor or message beggining
    char* recievedChars = new char[Wire.available()+1];
    //Current length of the recievedChars array
    int recievedCharsLength = 0;
    
    /* New RGB struct to store incoming RGB data 
    until it is assigned to the LedMode global */
    RGB recievedRgb = {0, 0, 0};
    
    //while data is availible, process data and add it to the LedMode global    
    while(Wire.available()) {
      //Read the next availible byte and store it for future use
      char recievedByte = Wire.read();
      
      /* Add the recieved yte to the end of the recievedChars array, 
      and iterate the array length variable */
      recievedChars[recievedCharsLength] = recievedByte;
      recievedCharsLength++;
      
      //Depending on what the specific byte is, process the byte and deliver data      
      if (recievedByte == *";") {
        /* If the byte is a semicolon, all of the bytes for the 
        currently-being-read integer have been stored - so, 
        increase the count of total integers recieved */
        integerCount++;
        
        //Parse the string into an integer
        int recievedInt = atoi(recievedChars);
        
        //Depending on which integer it is, store the data in different places
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
        
        /* Clear the recievedChars array for future 
        use, and set it's length to 0 */
        memset(recievedChars, 0, strlen(recievedChars));
        recievedCharsLength = 0;
        
      } else if (recievedByte == *"\n") {
        /* If end-of-line character is recieved, the message is 
        over - store the RGB in the LedMode global and
        clear the recieved chars array */
        stripMode.color = recievedRgb;
        memset(recievedChars, 0, strlen(recievedChars));
      }
    };
    
    /* Once all data is processed, delete the dynamic array
    storing recieved chars and set it's length to 0 */
    delete[] recievedChars;
    recievedCharsLength = 0;
  }
}
