// TempReader.cpp
#include "readTemps.h"
#include "variables.h" // Assuming thermo is defined in this header


void readTemps() {
  if (tempScale == 'C') {
    pidInput = thermo.readCelsius();
  }
  if (tempScale == 'F') {
    pidInput = thermo.readFahrenheit();
  }
}