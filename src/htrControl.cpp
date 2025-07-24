//******************************************************************************************************************************
//  HTRCONTROL: TURN HEATERS ON OR OFF
//******************************************************************************************************************************

#include <Arduino.h>
#include "variables.h"
#include "htrControl.h"
#include "shutDown.h"
void htrControl()
{

  if (pidOutput >= millis() - pidStart + 1)
  {
    digitalWrite(heaterPin, HIGH);
  }
  else
  {
    digitalWrite(heaterPin, LOW);
  }

  // ShutDown if thermocouple gets disconected for too long
  if ((isnan(pidInput) || pidInput == 0) && t0 == false)
  {
    t0 = true;
    glitchTimer = millis();
    delay(500);
  }
  if (t0 && millis() - glitchTimer > 120000)
  {
    lcd.clear();
    lcd.print(F("       ERROR:"));
    lcd.setCursor(0, 1);
    lcd.print(F("Falla de Termocupla!"));
    lcd.setCursor(5, 2);
    lcd.print(F("Reiniciar"));
    shutDown();
  }
  if (!isnan(pidInput) && t0 && pidInput != 0)
  {
    t0 = false;
    glitchTimer = 4000000000;
    delay(500);
  }
}