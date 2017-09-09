const unsigned int VOLTAGE_CURRENT_NR_OF_CYCLES = 20;
const unsigned long CURRENT_WAIT_TIME = 5000;
boolean currentWait = true;

void checkInputVoltageCurrent() {

  unsigned long timeDiff = abs(millis() - lastRunVoltageCurrentMeasure);
  if (timeDiff >= intervalVoltageCurrentMeasure) {
    lastRunVoltageCurrentMeasure = millis();
    
    supplyVoltageRaw += analogRead(PIN_VOLTAGE);
    supplyCurrentRaw += analogRead(PIN_CURRENT);

    if (voltageCurrentCycle == VOLTAGE_CURRENT_NR_OF_CYCLES) {
      voltageCurrentCycle = 0;
      supplyVoltageRaw /= VOLTAGE_CURRENT_NR_OF_CYCLES;
      supplyCurrentRaw /= VOLTAGE_CURRENT_NR_OF_CYCLES;
  
      // 3.968 is the factor of the potential divider
      double supplyVoltage = (5.0 / 1024) * supplyVoltageRaw * 3.968;
#ifdef DEBUG
      Serial.print(F("raw voltage: "));
      Serial.println(supplyVoltageRaw);
      Serial.print(F("calculated voltage: "));
      Serial.println(supplyVoltage);
#endif
      // test if disabling the heat mat is required
      if (supplyVoltage < minVoltage) {
        bitSet(errorCode, ERROR_CODE_UNDER_VOLTAGE);
      } else {
        bitClear(errorCode, ERROR_CODE_UNDER_VOLTAGE);
      }
      lastVoltage = supplyVoltage;

      if (currentWait && millis() > CURRENT_WAIT_TIME) {
        currentWait = false;
      }
      if (!currentWait) {
        // 3.968 is the factor of the potential divider
        double supplyCurrent = 0.0;
        if (supplyCurrentRaw > 512) {
          supplyCurrent = ((double)(supplyCurrentRaw - 512)) * (20.0 / 409.0);
        }
        supplyCurrent = abs(supplyCurrent);
#ifdef DEBUG
        Serial.print(F("raw current: "));
        Serial.println(supplyCurrentRaw);
        Serial.print(F("calculated current: "));
        Serial.println(supplyCurrent);
#endif
        // test if disabling the heat mat is required
        if (supplyCurrent > maxCurrent) {
          bitSet(errorCode, ERROR_CODE_OVER_CURRENT);
        } else {
          bitClear(errorCode, ERROR_CODE_OVER_CURRENT);
        }
        lastCurrent = supplyCurrent;
      }
    
      supplyVoltageRaw = 0;
      supplyCurrentRaw = 0;
    }
    
    voltageCurrentCycle++;
  }

}

