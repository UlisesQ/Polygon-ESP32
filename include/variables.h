#ifndef VARIABLES_H
#define VARIABLES_H
#include "Arduino.h"
#include <Adafruit_MAX31855.h>
#include "PID_v1.h"
#include <LiquidCrystal_I2C.h>
#include "FS.h"
#include "SPIFFS.h"
#include <Preferences.h>

extern bool t0;
extern bool segPhase;           // Current segment phase.  0 = ramp.  1 = hold.
extern byte segNum;             // Current segment number running in firing schedule.  0 means a schedule hasn't been selected yet.
extern double pidInput;         // Input array for PID loop (actual temp reading from thermocouple).  Don't change.
extern byte tempRange;          // This is how close the temp reading needs to be to the set point to shift to the hold phase (degrees).  Set to zero or a positive integer.
extern unsigned long holdStart; // Exact time the hold phase of the segment started (ms).  Based on millis().
extern unsigned long rampStart; // Exact time the ramp phase of the segment started (ms).  Based on millis().
extern int segHold[20];         // Hold time for each segment (min).  This starts after it reaches target temp.
extern float segTemp[20];       // Target temp for each segment (degrees).
extern float segRamp[20];       // Rate of temp change for each segment (deg/hr).
extern byte lastSeg;            // Last segment number in firing schedule
extern const int heaterPin;     // Pins connected to relays for heating elements.  This is an array for each output pin (zone).
extern byte screenNum;          // Screen number displayed during firing (1 = temps / 2 = schedule info / 3 = tools / 4 = done

extern Adafruit_MAX31855 thermo; // Pins connected to the thermocouple card.  This is an array for each thermocouple (zone).
extern const char tempScale;            // Temperature scale.  F = Fahrenheit.  C = Celsius

extern int lastTemp;               // Last setpoint temperature (degrees)
extern double rampHours;           // Time it has spent in ramp (hours)
extern double calcSetPoint;        // Calculated set point (degrees)
extern const int tempOffset;    // Array to add a temp offset for each zone (degrees).  Use if you have a cold zone in your kiln or if your thermocouple reading is off.  This gets added to the setpoint.
extern double pidSetPoint;          // Setpoint array for PID loop (temp you are trying to reach).  Don't change.
extern PID pidCont; // PID controller array for each zone.  Set arguments 4/5/6 to the Kp, Ki, Kd values after tuning.
extern double pidOutput;            // Output array for PID loop (relay for heater).  Don't change.

extern LiquidCrystal_I2C lcd;             // LCD display (connected to analog inputs / reverse order so I don't have to twist ribbon cable)
extern char schedDesc1[21];        // Schedule description #1 (first line of text file)
extern byte optionNum;          // Option selected from screen #3

//PROGRAM CREATOR
extern File myfile;
extern float tempVal;
extern unsigned int tempRamp;
extern unsigned long starttime;
extern unsigned long starttime2;
extern unsigned int fileNumber;
extern byte count;
extern char fileName[9];
extern byte segmentNum;
extern boolean yes;
extern byte interface;
extern byte segments;
extern byte totalSegments;


//Provisioning
extern const char *DeviceName;
extern Preferences preferences;
extern boolean Provisioned;


// WIFI

// REPLACE with your Domain name and URL path or IP address with path
extern const char *serverName;

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key
extern String apiKeyValue;


//setupPIDs
extern const int pidCycle;             // Time for a complete PID on/off cycle for the heating elements (ms)

//htrControl
extern unsigned long glitchTimer;
extern unsigned long pidStart;     // Exact time you started the new PID cycle (ms).  Based on millis().

//openSched
extern float tempo;
extern int schedNum;           // Current firing schedule number.  This ties to the file name (ex: 1.txt, 2.txt).
extern boolean schedOK;    // Is the schedule you loaded OK?
extern int hour;
extern int minute;
extern int i;

// OTA




#endif // VARIABLES_H