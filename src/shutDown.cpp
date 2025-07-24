//******************************************************************************************************************************
//  SHUTDOWN: SHUT DOWN SYSTEM
//******************************************************************************************************************************
#include <Arduino.h>
#include "variables.h"
#include "shutDown.h"

void shutDown() {


  digitalWrite(heaterPin, LOW);

  // Disable interrupts / Infinite loop
  cli();
  while (1);

}
