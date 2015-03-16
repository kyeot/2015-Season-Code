#ifndef LEDCONTROL_H
#define LEDCONTROL_H

//Data struct to hold simple R, G, and B values
struct RGB {
  int red;
  int green;
  int blue;
};

const RGB eotBlue = {35, 0, 250};
const RGB eotRed = {250, 0, 0};

#endif
