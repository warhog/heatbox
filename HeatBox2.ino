/**
 * DebounceInput Library
 * https://github.com/PaulMurrayCbr/DebounceInput
 */
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*********************************
 * DEBUG FLAG
 * uncomment to disable
 ********************************/
//#define DEBUG


/*********************************
 * lcdml configuration
 ********************************/
// lib config
// button press time in ms
#define _LCDML_DISP_cfg_button_press_time 300
// 0 = no scrollbar, 1 = scrollbar with custom chars
#define _LCDML_DISP_cfg_scrollbar 0
// 0 = nothing, 1 = redefine custom chars on scrolling
#define _LCDML_DISP_cfg_scrollbar_custom_redefine  0
// 0 = HD44780 standard / 1 = HD44780U standard
#define _LCDML_DISP_cfg_lcd_standard 0
#define _LCDML_DISP_cfg_initscreen 1
#define _LCDML_DISP_cfg_initscreen_time 10000

// settings for lcd
#define _LCDML_DISP_cols 16
#define _LCDML_DISP_rows 2
#include <LCDMenuLib.h>

// display address for i2c display driver
#define _LCDML_DISP_addr 0x27
#define _LCDML_DISP_rs 0
#define _LCDML_DISP_rw 1
#define _LCDML_DISP_e 2
#define _LCDML_DISP_dat4 4
#define _LCDML_DISP_dat5 5
#define _LCDML_DISP_dat6 6
#define _LCDML_DISP_dat7 7
#define _LCDML_DISP_backlight 3	
#define _LCDML_DISP_backlight_pol	POSITIVE

// digital lcdml control
#define _LCDML_DISP_cfg_control 2
void LCDML_CONTROL_digital();
// settings
// (0 = low active (pullup), 1 = high active (pulldown) button
#define _LCDML_CONTROL_digital_low_active 0
#define _LCDML_CONTROL_digital_enable_quit 1
#define _LCDML_CONTROL_digital_enable_lr 0
#define _LCDML_CONTROL_digital_enter 6
#define _LCDML_CONTROL_digital_up 9
#define _LCDML_CONTROL_digital_down 7
#define _LCDML_CONTROL_digital_quit 8
  
// menu element count - last element id
// this value must be the same as the last menu element
#define _LCDML_DISP_cnt 19

// init lcdmenulib 
LCDML_DISP_init(_LCDML_DISP_cnt);

// LCDMenuLib_element(id, group, prev_layer_element, new_element_num, lang_char_array, callback_function)
//                                                      1234567890123456
LCDML_DISP_add( 0, _LCDML_G1, LCDML_root        , 1  , "Information"        , LCDML_FUNC_information);
LCDML_DISP_add( 1, _LCDML_G1, LCDML_root        , 2  , "Details"            , LCDML_FUNC_information2);
LCDML_DISP_add( 2, _LCDML_G1, LCDML_root        , 3  , "Settings"           , LCDML_FUNC);
LCDML_DISP_add( 3, _LCDML_G1, LCDML_root_3      , 1  , "PI parameters"      , LCDML_FUNC);
LCDML_DISP_add( 4, _LCDML_G1, LCDML_root_3_1    , 1  , "Prop. gain"         , LCDML_FUNC_kp);
LCDML_DISP_add( 5, _LCDML_G1, LCDML_root_3_1    , 2  , "Integral time"      , LCDML_FUNC_tn);
LCDML_DISP_add( 6, _LCDML_G1, LCDML_root_3_1    , 3  , "Max Integral"       , LCDML_FUNC_imax);
LCDML_DISP_add( 7, _LCDML_G1, LCDML_root_3_1    , 4  , "Back"               , LCDML_FUNC_back);
LCDML_DISP_add( 8, _LCDML_G1, LCDML_root_3      , 2  , "Setpoint"           , LCDML_FUNC_setpoint);
LCDML_DISP_add( 9, _LCDML_G1, LCDML_root_3      , 3  , "Shutdowns"          , LCDML_FUNC);
LCDML_DISP_add(10, _LCDML_G1, LCDML_root_3      , 4  , "Parameter Set"      , LCDML_FUNC);
LCDML_DISP_add(11, _LCDML_G1, LCDML_root_3      , 5  , "Back"               , LCDML_FUNC_back);
LCDML_DISP_add(12, _LCDML_G1, LCDML_root_3_3    , 1  , "Thermal"            , LCDML_FUNC_shutdownthermal);
LCDML_DISP_add(13, _LCDML_G1, LCDML_root_3_3    , 2  , "Voltage"            , LCDML_FUNC_shutdownvoltage);
LCDML_DISP_add(14, _LCDML_G1, LCDML_root_3_3    , 3  , "Current"            , LCDML_FUNC_shutdowncurrent);
LCDML_DISP_add(15, _LCDML_G1, LCDML_root_3_3    , 4  , "Back"               , LCDML_FUNC_back);
LCDML_DISP_add(16, _LCDML_G1, LCDML_root_3_4    , 1  , "Set 1"              , LCDML_FUNC_parameterset1);
LCDML_DISP_add(17, _LCDML_G1, LCDML_root_3_4    , 2  , "Set 2"              , LCDML_FUNC_parameterset2);
LCDML_DISP_add(18, _LCDML_G1, LCDML_root_3_4    , 3  , "Back"               , LCDML_FUNC_back);
LCDML_DISP_add(19, _LCDML_G7, LCDML_root        , 10 , ""                   , LCDML_FUNC_information); // this element is hidden, the group is not enabled in setup
LCDML_DISP_createMenu(_LCDML_DISP_cnt);

/*********************************
 * LCDML BACKEND (core of the menu)
 ********************************/
// define backend function  
// last backend function id
#define _LCDML_BACK_cnt 1

LCDML_BACK_init(_LCDML_BACK_cnt);
LCDML_BACK_new_timebased_static(0, ( 20UL ), _LCDML_start, LCDML_BACKEND_control);
LCDML_BACK_new_timebased_dynamic(1, ( 1000UL ), _LCDML_stop, LCDML_BACKEND_menu);
LCDML_BACK_create();


/*********************************
 * pin configuration
 ********************************/
// pin for the heat mat output (needs pwm capability)
#define PIN_HEAT_MAT 10
// pin for voltage measurement
#define PIN_VOLTAGE A0
// pin for current measurement
#define PIN_CURRENT A2
// pin for eeprom reset jumper
#define PIN_EEPROM 4
// pin for the onewire sensors
#define PIN_ONEWIRE 12
// pin for buttons ground
#define PIN_BUTTONS 5

/*********************************
 * internal variables
 ********************************/
// fault limits
// maximum allowed temperature
uint8_t maxTemperature = 50;
// minimum allowed supply voltage
double minVoltage = 10.0;
// maximum allowed supply current draw
uint8_t maxCurrent = 5;

uint8_t degree[8]  = { 140, 146, 146, 140, 128, 128, 128, 128 };

uint8_t currentParameterSet = 0;
  
// error code flag (use bits for below errors)
uint8_t errorCode;
#define ERROR_CODE_OVER_TEMPERATURE 1
#define ERROR_CODE_UNDER_VOLTAGE 2
#define ERROR_CODE_OVER_CURRENT 3
#define ERROR_CODE_ONEWIRE_FAULT 4

/*********************************
 * globals for pi controller
 ********************************/
// sample time for pid in millis
const unsigned long sampleTimePIController = 2500;
// min value for i portion
const double iMin = 0.0;
// min value for pi output
const double piMin = 0.0;
// max value for pi output
const double piMax = 100.0;

double setpoint = 38;
// pi controller proportional gain
double kp = 50;
// pi controller integral time
double tn = 60;
// pi controller integral gain (always calculated)
double ki = kp / tn;
// last output from pi controller
double outputPercent = 0;
// max value for i portion
double iMax = 50.0;
// sumation of error over loops
double errorSum = 0.0;

// last run of pid calucaltion
unsigned long lastRunPiCalculation = abs(millis() - sampleTimePIController);


/*********************************
 * globals for temperature measurement
 ********************************/
unsigned long timeProcessTemperature = 0;
const unsigned int timeTemperatureRequest = 1000;
unsigned long lastRunTemperatureRequest = abs(millis() - timeTemperatureRequest);
// last measured temperautre
double lastTemperature = 0.0;
// onewire / ds18x libs
OneWire oneWire(PIN_ONEWIRE);
DallasTemperature sensors(&oneWire);


/*********************************
 * globals for power and current
 ********************************/
double lastVoltage = 0.0;
double lastCurrent = 0;
unsigned long lastRunVoltageCurrentMeasure = millis();
const uint16_t intervalVoltageCurrentMeasure = 100;
uint16_t supplyVoltageRaw = 0;
uint16_t supplyCurrentRaw = 0;
uint8_t voltageCurrentCycle = 0;


/*********************************
 * eeprom stuff
 ********************************/
// version of eeprom config
#define CONFIG_VERSION "c5"
// struct for eeprom config
struct storeStruct {
  char version[3];
  uint8_t maxTemperature;
  double minVoltage;
  uint8_t maxCurrent;
  double kp;
  double tn;
  double iMax;
  double setpoint;
} storage = {
  CONFIG_VERSION,
  maxTemperature,
  minVoltage,
  maxCurrent,
  kp,
  tn,
  iMax,
  setpoint
};

/*********************************
 * SETUP
 ********************************/
void setup() {  
  Serial.begin(115200);

#ifdef DEBUG
  Serial.println(F("setting up lcd"));
#endif
  LCDML_DISP_groupEnable(_LCDML_G1);
  LCDML_setup(_LCDML_BACK_cnt);
  lcd.createChar(0, degree);

  lcd.clear();
  lcd.print(F("starting up"));
  
#ifdef DEBUG
  Serial.println(F("setting pin modes"));
#endif
  pinMode(PIN_HEAT_MAT, OUTPUT);
  analogWrite(PIN_HEAT_MAT, 0);

  pinMode(_LCDML_CONTROL_digital_enter, INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_digital_up, INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_digital_down, INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_digital_quit, INPUT_PULLUP);
  pinMode(PIN_BUTTONS, OUTPUT);
  digitalWrite(PIN_BUTTONS, LOW);
  pinMode(PIN_EEPROM, INPUT_PULLUP);

setupdigital();
#ifdef DEBUG
  Serial.println(F("setting up sensors"));
#endif
  setupTemperature();

#ifdef DEBUG
  Serial.println(F("loading parameter set"));
#endif
  lcd.clear();
  lcd.print(F("sel. param. set "));
  lcd.setCursor(0, 1);
  lcd.print(F("set 1      set 2"));

  unsigned long wait = millis();
  while (millis() < (wait + 5000)) {
    uint8_t buttons = 0x00;
    bitWrite(buttons, 0, digitalRead(_LCDML_CONTROL_digital_enter));
    bitWrite(buttons, 1, digitalRead(_LCDML_CONTROL_digital_up));
    bitWrite(buttons, 2, digitalRead(_LCDML_CONTROL_digital_down));
    bitWrite(buttons, 3, digitalRead(_LCDML_CONTROL_digital_quit));
  
    if (buttons > 0) {
      if (bitRead(buttons, 0) || bitRead(buttons, 2)) {
        currentParameterSet = 1;
      }
      break;
    }
  }
  delay(200);

  if (digitalRead(PIN_EEPROM) == LOW) {
#ifdef DEBUG
    Serial.println(F("saving default eeprom data"));
#endif
    saveConfig(currentParameterSet);
  } else {
#ifdef DEBUG
    Serial.println(F("loading eeprom data"));
#endif
    loadConfig(currentParameterSet);
  }

    LCDML_run(_LCDML_priority);
    LCDML_DISP_jumpToFunc(LCDML_FUNC_information);     

}

/*********************************
 * LOOP
 ********************************/
void loop() { 
  LCDML_CONTROL_digital();
  
  // update display
  if (errorCode == 0) {
    if (LCDML_BUTTON_checkAny()) {
      // reset init screen time
      g_lcdml_initscreen = millis();
#ifdef DEBUG
      Serial.println(F("resetting init screen time"));
#endif
    }
    
    if ((millis() - g_lcdml_initscreen) >= _LCDML_DISP_cfg_initscreen_time) {
      // reset init screen time
      g_lcdml_initscreen = millis();
      // jump to infoscreen
      LCDML_DISP_jumpToFunc(LCDML_FUNC_information);     
#ifdef DEBUG
      Serial.println(F("goto init screen"));
#endif
    }
    
    LCDML_run(_LCDML_priority);
    
    // update pi loop
    piCalculate(lastTemperature);

  } else {
    analogWrite(PIN_HEAT_MAT, 0);
    lcd.setCursor(0, 0);
    if (bitRead(errorCode, ERROR_CODE_OVER_TEMPERATURE)) {
      lcd.print(F("Temperature too"));
      lcd.setCursor(0, 1);
      lcd.print(F("high: "));
      lcd.print(lastTemperature);
      lcd.print(F(" "));
      lcd.print(char(0));
      lcd.print(F("C        "));
    } else if (bitRead(errorCode, ERROR_CODE_UNDER_VOLTAGE)) {
      lcd.print(F("Voltage too     "));
      lcd.setCursor(0, 1);
      lcd.print(F("low: "));
      lcd.print(lastVoltage);
      lcd.print(F(" Volt        "));
    } else if (bitRead(errorCode, ERROR_CODE_OVER_CURRENT)) {
      lcd.print(F("Current too     "));
      lcd.setCursor(0, 1);
      lcd.print(F("high: "));
      lcd.print(lastCurrent);
      lcd.print(F(" Ampere     "));
    } else if (bitRead(errorCode, ERROR_CODE_ONEWIRE_FAULT)) {
      lcd.print(F("OneWire sensor     "));
      lcd.setCursor(0, 1);
      lcd.print(F("fault             "));
    } else {
      lcd.print(F("unknown error"));
      lcd.setCursor(0, 1);
      lcd.print(errorCode);
      lcd.print(F("                   "));
    }
  }

  requestTemperature();
  processTemperature();
  checkOvertemperature();
  checkInputVoltageCurrent();
  
}

