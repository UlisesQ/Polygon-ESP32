#include "updateSeg.h"
#include <Arduino.h> 
#include "variables.h"

void updateSeg() {
    // Start the hold phase
    if (t0 == false) {
        if ((segPhase == 0 && segRamp[segNum - 1] < 0 && pidInput <= (segTemp[segNum - 1] + tempRange)) ||
            (segPhase == 0 && segRamp[segNum - 1] >= 0 && pidInput >= (segTemp[segNum - 1] - tempRange))) {
            segPhase = 1;
            holdStart = millis();
        }

        // Go to the next segment
        if (segPhase == 1 && millis() - holdStart >= segHold[segNum - 1] * 60000) {
            segNum = segNum + 1;
            segPhase = 0;
            rampStart = millis();
        }

        // Check if complete / turn off all zones
        if (segNum - 1 > lastSeg) {
            digitalWrite(heaterPin, LOW);
            screenNum = 4;
        }
    }
}