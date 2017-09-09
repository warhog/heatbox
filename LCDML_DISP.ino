
// every "disp menu function" constist of three functions 
// - void LCDML_DISP_setup(func_name)    is called only if it is started
// - void LCDML_DISP_loop(func_name)     is called continues if function quit on every button even or trigger event
// - void LCDML_DISP_loop_end(func_name) is called when quit button or LCDML_DISP_funcend is called


// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_information) {
// *********************************************************************
  // setup function
  LCDML_DISP_triggerMenu(1000);
}

void LCDML_DISP_loop(LCDML_FUNC_information) {

  g_lcdml_initscreen = millis();
  lcd.setCursor(0, 0);
  lcd.print(lastTemperature);
  lcd.print(char(0));
  lcd.print(F("C, "));
  lcdPrintDouble(outputPercent, 1);
  lcd.print(F("%   "));

  lcd.setCursor(0, 1);

  lcdPrintDouble(lastVoltage, 1);
  lcd.print(F("V, "));
  lcdPrintDouble(lastCurrent, 1);
  lcd.print(F("A     "));
  lcd.setCursor(14, 1);
  lcd.print(F("S"));
  lcd.print(currentParameterSet + 1);

  if(LCDML_BUTTON_checkAny()) { // check if any button is presed (enter, up, down, left, right)
    // LCDML_DISP_funcend calls the loop_end function
    LCDML_DISP_funcend();
  } 
}

void LCDML_DISP_loop_end(LCDML_FUNC_information) {
  // this functions is ever called when a DISP function is quit
  // you can here reset some global vars or do nothing  
}  

uint8_t currentInfoPage = 0;
// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_information2) {
// *********************************************************************
  // setup function
  LCDML_DISP_triggerMenu(250);
  currentInfoPage = 0;
}
void LCDML_DISP_loop(LCDML_FUNC_information2) {

  g_lcdml_initscreen = millis();
  lcd.clear();
  lcd.setCursor(0, 0);
  if (currentInfoPage == 0) {
    lcd.print(F("Temperature:"));
    lcd.setCursor(0, 1);
    lcd.print(lastTemperature);
    lcd.print(F(" "));
    lcd.print(char(0));
    lcd.print(F("C   "));
  } else if (currentInfoPage == 1) {
    lcd.print(F("Output: "));
    lcd.print(outputPercent);
    lcd.print(F(" %  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Raw: "));
    lcd.print(doubleMap(outputPercent, piMin, piMax, 0, 255));
  } else if (currentInfoPage == 2) {
    lcd.print(F("Voltage: "));
    lcd.print(lastVoltage);
    lcd.print(F(" V  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Raw: "));
    lcd.print(analogRead(PIN_VOLTAGE));
  } else if (currentInfoPage == 3) {
    lcd.print(F("Current: "));
    lcd.print(lastCurrent);
    lcd.print(F(" A  "));
    lcd.setCursor(0, 1);
    lcd.print(F("Raw: "));
    lcd.print(analogRead(PIN_CURRENT));
  }

  if(LCDML_BUTTON_checkAny()) { // check if any button is presed (enter, up, down, left, right)
    // LCDML_DISP_funcend calls the loop_end function
    if (LCDML_BUTTON_checkUp()) {
      if (currentInfoPage < 3) {
        currentInfoPage++;
      }
      LCDML_BUTTON_resetUp();
    } else if (LCDML_BUTTON_checkDown()) {
      if (currentInfoPage > 0) {
        currentInfoPage--;
      }
      LCDML_BUTTON_resetDown();
    } else {
      LCDML_DISP_funcend();
    }
  } 
}

void LCDML_DISP_loop_end(LCDML_FUNC_information2) {
  // this functions is ever called when a DISP function is quit
  // you can here reset some global vars or do nothing  
}  


// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_setpoint) {
// *********************************************************************
  LCDML_DISP_triggerMenu(100);
}

void LCDML_DISP_loop(LCDML_FUNC_setpoint) { 
  // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
  // the quit button works in every DISP function without any checks; it starts the loop_end function 
  
  LCDML_DISP_resetIsTimer();

  lcd.setCursor(0, 0);
  lcd.print(F("Setpoint"));

  lcd.setCursor(0, 1);
  lcd.print(setpoint);
  lcd.print(F(" "));
  lcd.print(char(0));
  lcd.print(F("C   "));

  if (LCDML_BUTTON_checkAny()) {
    if (LCDML_BUTTON_checkUp()) {
      setpoint += 0.5;
      LCDML_BUTTON_resetUp();
    }
  
    if (LCDML_BUTTON_checkDown()) {
      setpoint -= 0.5;
      LCDML_BUTTON_resetDown();
    }
    setpoint = constrain(setpoint, 25, maxTemperature);
  
    if (LCDML_BUTTON_checkEnter()) {
      LCDML_BUTTON_resetEnter();
      LCDML_DISP_funcend();
    }
  }
 
}

void LCDML_DISP_loop_end(LCDML_FUNC_setpoint) {
  saveConfig(currentParameterSet);
}

// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_kp) {
// *********************************************************************
  LCDML_DISP_triggerMenu(100);
}

void LCDML_DISP_loop(LCDML_FUNC_kp) { 
  lcd.setCursor(0, 0);
  lcd.print(F("Prop. gain"));

  lcd.setCursor(0, 1);
  lcd.print(F("Kp: "));
  lcd.print(kp);
  lcd.print(F(" %/"));
  lcd.print(char(0));
  lcd.print(F("C   "));


  if (LCDML_BUTTON_checkAny()) {
    if (LCDML_BUTTON_checkUp()) {
      kp++;
      LCDML_BUTTON_resetUp();
    }
  
    if (LCDML_BUTTON_checkDown()) {
      kp--;
      LCDML_BUTTON_resetDown();
    }
    kp = constrain(kp, 0, 100);

    if (LCDML_BUTTON_checkEnter()) {
      LCDML_BUTTON_resetEnter();
      LCDML_DISP_funcend();
    }
  }
}

void LCDML_DISP_loop_end(LCDML_FUNC_kp) {
  saveConfig(currentParameterSet);
}

// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_tn) {
// *********************************************************************
  LCDML_DISP_triggerMenu(100);
}

void LCDML_DISP_loop(LCDML_FUNC_tn) { 
  lcd.setCursor(0, 0);
  lcd.print(F("Integral time"));

  lcd.setCursor(0, 1);
  lcd.print(F("Tn: "));
  lcd.print(tn);
  lcd.print(F(" s   "));

  if (LCDML_BUTTON_checkAny()) {
    if (LCDML_BUTTON_checkUp()) {
      tn += 1;
      LCDML_BUTTON_resetUp();
    }
  
    if (LCDML_BUTTON_checkDown()) {
      tn -= 1;
      LCDML_BUTTON_resetDown();
    }
    tn = constrain(tn, 0, 512);
  
    if (LCDML_BUTTON_checkEnter()) {
      LCDML_BUTTON_resetEnter();
      LCDML_DISP_funcend();
    }
  }
 
}

void LCDML_DISP_loop_end(LCDML_FUNC_tn) {
  ki = kp / tn;
  saveConfig(currentParameterSet);
  errorSum = 0;
}

// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_imax) {
// *********************************************************************
  LCDML_DISP_triggerMenu(100);
}

void LCDML_DISP_loop(LCDML_FUNC_imax) { 
  lcd.setCursor(0, 0);
  lcd.print(F("Max integral"));

  lcd.setCursor(0, 1);
  lcd.print(iMax);
  lcd.print(F(" %   "));

  if (LCDML_BUTTON_checkAny()) {
    if (LCDML_BUTTON_checkUp()) {
      iMax += 0.5;
      LCDML_BUTTON_resetUp();
    }
  
    if (LCDML_BUTTON_checkDown()) {
      iMax -= 0.5;
      LCDML_BUTTON_resetDown();
    }
    iMax = constrain(iMax, 0, 75);
    
    if (LCDML_BUTTON_checkEnter()) {
      LCDML_BUTTON_resetEnter();
      LCDML_DISP_funcend();
    }
  }

}

void LCDML_DISP_loop_end(LCDML_FUNC_imax) {
  saveConfig(currentParameterSet);
  errorSum = 0;
}

double maxTemperatureTemp;
// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_shutdownthermal) {
// *********************************************************************
  LCDML_DISP_triggerMenu(100);
  maxTemperatureTemp = maxTemperature;
}

void LCDML_DISP_loop(LCDML_FUNC_shutdownthermal) { 
  lcd.setCursor(0, 0);
  lcd.print(F("Shtdwn. thermal"));

  lcd.setCursor(0, 1);
  lcd.print(maxTemperatureTemp);
  lcd.print(F(" "));
  lcd.print(char(0));
  lcd.print(F("C   "));

  if (LCDML_BUTTON_checkAny()) {
    if (LCDML_BUTTON_checkUp()) {
      maxTemperatureTemp++;
      LCDML_BUTTON_resetUp();
    }
  
    if (LCDML_BUTTON_checkDown()) {
      maxTemperatureTemp--;
      LCDML_BUTTON_resetDown();
    }
    maxTemperatureTemp = constrain(maxTemperatureTemp, 30, 50);

    if (LCDML_BUTTON_checkEnter()) {
      LCDML_BUTTON_resetEnter();
      LCDML_DISP_funcend();
    }
  }

}

void LCDML_DISP_loop_end(LCDML_FUNC_shutdownthermal) {
  maxTemperature = maxTemperatureTemp;
  saveConfig(currentParameterSet);
}

double minVoltageTemp;
// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_shutdownvoltage) {
// *********************************************************************
  LCDML_DISP_triggerMenu(100);
  minVoltageTemp = minVoltage;
}

void LCDML_DISP_loop(LCDML_FUNC_shutdownvoltage) { 
  lcd.setCursor(0, 0);
  lcd.print(F("Shtdwn. voltage"));

  lcd.setCursor(0, 1);
  lcd.print(minVoltageTemp);
  lcd.print(F(" Volts   "));

  if (LCDML_BUTTON_checkAny()) {
    if (LCDML_BUTTON_checkUp()) {
      minVoltageTemp += 0.1;
      LCDML_BUTTON_resetUp();
    }
  
    if (LCDML_BUTTON_checkDown()) {
      minVoltageTemp -= 0.1;
      LCDML_BUTTON_resetDown();
    }
    minVoltageTemp = constrain(minVoltageTemp, 10, 14);

    if (LCDML_BUTTON_checkEnter()) {
      LCDML_BUTTON_resetEnter();
      LCDML_DISP_funcend();
    }
  }

}

void LCDML_DISP_loop_end(LCDML_FUNC_shutdownvoltage) {
  minVoltage = minVoltageTemp;
  saveConfig(currentParameterSet);
}

double maxCurrentTemp;
// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_shutdowncurrent) {
// *********************************************************************
  LCDML_DISP_triggerMenu(100);
  maxCurrentTemp = maxCurrent;
}

void LCDML_DISP_loop(LCDML_FUNC_shutdowncurrent) { 
  lcd.setCursor(0, 0);
  lcd.print(F("Shtdwn. current"));

  lcd.setCursor(0, 1);
  lcd.print(maxCurrentTemp);
  lcd.print(F(" Ampere   "));

  if (LCDML_BUTTON_checkAny()) {
    if (LCDML_BUTTON_checkUp()) {
      maxCurrentTemp += 1;
      LCDML_BUTTON_resetUp();
    }
  
    if (LCDML_BUTTON_checkDown()) {
      maxCurrentTemp -= 1;
      LCDML_BUTTON_resetDown();
    }
    maxCurrentTemp = constrain(maxCurrentTemp, 5, 15);

    if (LCDML_BUTTON_checkEnter()) {
      LCDML_BUTTON_resetEnter();
      LCDML_DISP_funcend();
    }
  }

}

void LCDML_DISP_loop_end(LCDML_FUNC_shutdowncurrent) {
  maxCurrent = maxCurrentTemp;
  saveConfig(currentParameterSet);
}

// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_parameterset1) {
// *********************************************************************
}

void LCDML_DISP_loop(LCDML_FUNC_parameterset1) { 
  currentParameterSet = 0;
  loadConfig(currentParameterSet);
  LCDML_BUTTON_quit();
  LCDML_DISP_funcend();
}

void LCDML_DISP_loop_end(LCDML_FUNC_parameterset1) {
}

// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_parameterset2) {
// *********************************************************************
}

void LCDML_DISP_loop(LCDML_FUNC_parameterset2) { 
  currentParameterSet = 1;
  loadConfig(currentParameterSet);
  LCDML_BUTTON_quit();
  LCDML_DISP_funcend();
}

void LCDML_DISP_loop_end(LCDML_FUNC_parameterset2) {
}


// *********************************************************************
void LCDML_DISP_setup(LCDML_FUNC_back) {
// ********************************************************************* 
  LCDML_DISP_funcend();  
}

void LCDML_DISP_loop(LCDML_FUNC_back) {
}

void LCDML_DISP_loop_end(LCDML_FUNC_back) {
  LCDML.goBack();
}
