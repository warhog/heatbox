#include <DebounceInput.h>

const unsigned long RETRIGGER_TIME = 500;

DebouncedInput pin1;
DebouncedInput pin2;
DebouncedInput pin3;
DebouncedInput pin4;

unsigned long startButton1 = 0;
unsigned long startButton2 = 0;
unsigned long startButton3 = 0;
unsigned long startButton4 = 0;
unsigned int repeatedButton1 = 0;
unsigned int repeatedButton2 = 0;
unsigned int repeatedButton3 = 0;
unsigned int repeatedButton4 = 0;


void setupdigital() {

  pin1.attach(_LCDML_CONTROL_digital_enter);
  pin2.attach(_LCDML_CONTROL_digital_up);
  pin3.attach(_LCDML_CONTROL_digital_down);
  pin4.attach(_LCDML_CONTROL_digital_quit);

}


void LCDML_CONTROL_digital() {
  pin1.read();
  pin2.read();
  pin3.read();
  pin4.read();

  if (pin1.rising()) {
    startButton1 = millis();
    LCDML_BUTTON_enter();
  } else if (pin1.falling()) {
    startButton1 = 0;
    repeatedButton1 = 0;
  }

  if (startButton1 != 0 && ((repeatedButton1 < 5 && millis() > (startButton1 + RETRIGGER_TIME)) || (repeatedButton1 >= 5 && millis() > (startButton1 + (RETRIGGER_TIME / 2))))) {
    startButton1 = millis();
    repeatedButton1++;
    LCDML_BUTTON_enter();
  }

  if (pin2.rising()) {
    startButton2 = millis();
    LCDML_BUTTON_up();
  } else if (pin2.falling()) {
    startButton2 = 0;
    repeatedButton2 = 0;
  }

  if (startButton2 != 0 && ((repeatedButton2 < 5 && millis() > (startButton2 + RETRIGGER_TIME)) || (repeatedButton2 >= 5 && millis() > (startButton2 + (RETRIGGER_TIME / 2))))) {
    startButton2 = millis();
    repeatedButton2++;
    LCDML_BUTTON_up();
  }

  if (pin3.rising()) {
    startButton3 = millis();
    repeatedButton3++;
    LCDML_BUTTON_down();
  } else if (pin3.falling()) {
    startButton3 = 0;
    repeatedButton3 = 0;
  }

  if (startButton3 != 0 && ((repeatedButton3 < 5 && millis() > (startButton3 + RETRIGGER_TIME)) || (repeatedButton3 >= 5 && millis() > (startButton3 + (RETRIGGER_TIME / 2))))) {
    startButton3 = millis();
    repeatedButton3++;
    LCDML_BUTTON_down();
  }

  if (pin4.rising()) {
    startButton4 = millis();
    repeatedButton4++;
    LCDML_BUTTON_quit();
  } else if (pin4.falling()) {
    startButton4 = 0;
    repeatedButton4 = 0;
  }

  if (startButton4 != 0 && ((repeatedButton4 < 5 && millis() > (startButton4 + RETRIGGER_TIME)) || (repeatedButton4 >= 5 && millis() > (startButton4 + (RETRIGGER_TIME / 2))))) {
    startButton4 = millis();
    repeatedButton4++;
    LCDML_BUTTON_quit();
  }
  
}
