//******************************************************************************************************************************
//  SETUPPIDS: INITIALIZE THE PID LOOPS
//******************************************************************************************************************************
#include "variables.h"
#include "setupPIDs.h"


void setupPIDs() {

  pidCont.SetSampleTime(pidCycle);
  pidCont.SetOutputLimits(0, pidCycle);
  pidCont.SetMode(AUTOMATIC);

}