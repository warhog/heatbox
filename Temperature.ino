/*********************************
 * SETUP TEMPERATURE
 ********************************/
 void setupTemperature() {

#ifdef DEBUG
  Serial.println(F("setup temperature sensor"));
#endif
  sensors.begin();
  sensors.setWaitForConversion(false);

}


bool lockTemperatureRequest = false;
bool lockTemperatureProcess = true;
/*********************************
 * REQUEST TEMPERATURE
 ********************************/
 void requestTemperature() {

  unsigned long timeDiff = abs(millis() - lastRunTemperatureRequest);
  if (timeDiff >= timeTemperatureRequest && !lockTemperatureRequest) {
    lastRunTemperatureRequest = millis();
#ifdef DEBUG
    Serial.println(F("request data from temperature sensor"));
#endif
    sensors.requestTemperatures();
    // wait at least 750ms, to be save wait 1000ms
    timeProcessTemperature = millis() + 1000;
    lockTemperatureRequest = true;
    lockTemperatureProcess = false;
  }

}

/*********************************
 * PROCESS TEMPERATURE
 ********************************/
void processTemperature() {

  if (millis() > timeProcessTemperature && !lockTemperatureProcess) {
#ifdef DEBUG
    Serial.println(F("process data from temperature sensor"));
#endif
    // get temperature from sensors
    float temperature = sensors.getTempCByIndex(0);
#ifdef DEBUG
    Serial.print(F("result: "));
    Serial.println(temperature);
#endif
    if (temperature == DEVICE_DISCONNECTED_C) {
      bitSet(errorCode, ERROR_CODE_ONEWIRE_FAULT);
    } else {
      bitClear(errorCode, ERROR_CODE_ONEWIRE_FAULT);
    }
    lastTemperature = temperature;
    lastRunTemperatureRequest = millis();
    lockTemperatureRequest = false;
    lockTemperatureProcess = true;
    
  }
  
}


void checkOvertemperature() {
  if (lastTemperature >= (double)maxTemperature) {
    bitSet(errorCode, ERROR_CODE_OVER_TEMPERATURE);
  } else {
    bitClear(errorCode, ERROR_CODE_OVER_TEMPERATURE);
  }
}
