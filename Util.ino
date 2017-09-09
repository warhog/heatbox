/*********************************
 * DOUBLEMAP
 * arduino map function with double support
 ********************************/
uint8_t doubleMap(double x, double in_min, double in_max, long out_min, long out_max) {
  return (uint8_t)((x - in_min) * (double)(out_max - out_min) / (in_max - in_min) + (double)out_min);
}

/*********************************
 * LCDPRINTDOUBLE
 * taken from
 * http://forum.arduino.cc/index.php?topic=44216.0
 ********************************/
void lcdPrintDouble (double val, byte precision) {
  // prints val on a ver 0012 text lcd with number of decimal places determine by precision
  // precision is a number from 0 to 6 indicating the desired decimial places
  // example: printDouble( 3.1415, 2); // prints 3.14 (two decimal places)

  if (val < 0.0) {
    lcd.print('-');
    val = -val;
  }

  lcd.print (int(val));  //prints the int part
  if ( precision > 0) {
    lcd.print("."); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision - 1;
    while (precision--)
      mult *= 10;

    if (val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val) - val ) * mult;
    unsigned long frac1 = frac;
    while ( frac1 /= 10 )
      padding--;
    while (  padding--)
      lcd.print("0");
    lcd.print(frac, DEC) ;
  }
}

/*********************************
 * EEPROM FUNCTIONS
 *********************************/
void loadConfig(uint8_t parameterSet) {
#ifdef DEBUG
  Serial.print(F("loading eeprom set "));  
  Serial.println(parameterSet);
#endif
  uint16_t start = sizeof(storeStruct) * parameterSet;
  if (EEPROM.read(start) == CONFIG_VERSION[0] && EEPROM.read(start + 1) == CONFIG_VERSION[1]) {
    for (unsigned int i = 0; i < sizeof(storage); i++) {
      *((char*)&storage + i) = EEPROM.read(i + start);
    }
    maxTemperature = storage.maxTemperature;
    minVoltage = storage.minVoltage;
    maxCurrent = storage.maxCurrent;
    kp = storage.kp;
    tn = storage.tn;
    iMax = storage.iMax;
    setpoint = storage.setpoint;
  } else {
#ifdef DEBUG
    Serial.println(F("wrong eeprom config version"));
#endif
    saveConfig(parameterSet);
  }
}

void saveConfig(uint8_t parameterSet) {
#ifdef DEBUG
  Serial.print(F("store eeprom set "));  
  Serial.println(parameterSet);
#endif
  uint16_t start = sizeof(storeStruct) * parameterSet;

  storage.maxTemperature = maxTemperature;
  storage.minVoltage = minVoltage;
  storage.maxCurrent = maxCurrent;
  storage.kp = kp;
  storage.tn = tn;
  storage.iMax = iMax;
  storage.setpoint = setpoint;
  for (unsigned int i = 0; i < sizeof(storage); i++) {
    EEPROM.write(i + start, *((char*)&storage + i));
  }
}
