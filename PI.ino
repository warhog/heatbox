/*********************************
 * PI CONTROLLER
 ********************************/
void piCalculate(double lastTemperature) {
  
  unsigned long timeDiff = abs(millis() - lastRunPiCalculation);
  if (timeDiff >= sampleTimePIController) {
    // reached sample time, calculate new pid output
    lastRunPiCalculation = millis();
    // calculate error between setpoint and input
    double error = setpoint - lastTemperature;
#ifdef DEBUG
    Serial.print(F("timeDiff: "));
    Serial.println(timeDiff);
    Serial.print(F("Kp: "));
    Serial.print(kp);
    Serial.print(F(", Ki: "));
    Serial.println(ki);
    Serial.print(F("error: "));
    Serial.println(error);
    Serial.print(F("pTerm: "));
    Serial.println(kp * error);
#endif

    // calculate i portion
    errorSum += error;
    // anti-windup #1
    double maxErrorSum = iMax / ki / (timeDiff / 1000.0);
#ifdef DEBUG
    Serial.print(F("anti windup maxErrorSum: "));
    Serial.println(maxErrorSum);
#endif
    if (errorSum > maxErrorSum) {
      errorSum = maxErrorSum;
    } else if (errorSum < 0) {
      errorSum = 0;
    }
    double iTerm = ki * (timeDiff / 1000.0) * errorSum;
#ifdef DEBUG
    Serial.print(F("errorSum: "));
    Serial.println(errorSum);
    Serial.print(F("iTerm without antiwindup #2: "));
    Serial.println(iTerm);
#endif
    // anti-windup #2
    if (iTerm > iMax) {
      iTerm = iMax;
    } else if (iTerm < iMin) {
      iTerm = iMin;
    }
#ifdef DEBUG
    Serial.print(F("iTerm: "));
    Serial.println(iTerm);
#endif

    // calculate output
    outputPercent = (kp * error) + iTerm;
#ifdef DEBUG
    Serial.print(F("outputPercent before limiting: "));
    Serial.println(outputPercent);
#endif
    if (outputPercent > piMax) {
      outputPercent = piMax;
    } else if (outputPercent < piMin) {
      outputPercent = piMin;
    }
#ifdef DEBUG
    Serial.print(F("outputPercent: "));
    Serial.println(outputPercent);
    Serial.println(doubleMap(outputPercent, piMin, piMax, 0, 255));
#endif
    // output results
    analogWrite(PIN_HEAT_MAT, doubleMap(outputPercent, piMin, piMax, 0, 255));
      
  }
  
}
