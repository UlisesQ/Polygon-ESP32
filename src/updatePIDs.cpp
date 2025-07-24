// PIDUpdater.cpp
#include "updatePIDs.h"
#include "variables.h"
#include "readTemps.h"
#include "PID_v1.h"

void updatePIDs() {
  // Get the last target temperature
  if (segNum == 1) {  // Set to room temperature for first segment
    if (tempScale == 'C') {
      lastTemp = 24;
    }
    if (tempScale == 'F') {
      lastTemp = 75;
    }
  }
  else {
    lastTemp = segTemp[segNum - 2];
  }

  // Calculate the new setpoint value.  Don't set above / below target temp
  if (segPhase == 0) {
    rampHours = (millis() - rampStart) / 3600000.0;
    calcSetPoint = lastTemp + (segRamp[segNum - 1] * rampHours);  // Ramp
    if (segRamp[segNum - 1] >= 0 && calcSetPoint >= segTemp[segNum - 1]) {
      calcSetPoint = segTemp[segNum - 1];
    }
    if (segRamp[segNum - 1] < 0 && calcSetPoint <= segTemp[segNum - 1]) {
      calcSetPoint = segTemp[segNum - 1];
    }
  }
  else {
    calcSetPoint = segTemp[segNum - 1];  // Hold
  }

  // Read the temperatures
  readTemps();

  // Set the target temp.  Add any offset.
  pidSetPoint = calcSetPoint + tempOffset;

  // Update the PID based on new variables
  if (!isnan(pidInput)) {          //added to fix issue of a zero readiing stopping the control even though it sees a temp lower than target.
    pidCont.Compute();
  }
}