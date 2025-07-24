#include "variables.h"
#include <Adafruit_MAX31855.h>
#include <LiquidCrystal_I2C.h>
#include "FS.h"
#include "SPIFFS.h"
#include <Preferences.h>
bool t0 = false;
bool segPhase = false;
byte segNum = 0;
double pidInput;
byte tempRange = 3;
unsigned long holdStart;
unsigned long rampStart;
int segHold[20] = {0};
float segTemp[20];
float segRamp[20];
byte lastSeg = 0;
const int heaterPin = 16;
byte screenNum = 9;

Adafruit_MAX31855 thermo = Adafruit_MAX31855( 18, 5 , 19); 
const char tempScale = 'C';      

const int tempOffset = 0;
PID pidCont = PID(&pidInput, &pidOutput, &pidSetPoint, 800, 47.37, 4.93, DIRECT);  // PID controller array for each zone.  Set arguments 4/5/6 to the Kp, Ki, Kd values after tuning.
double pidOutput;
double pidSetPoint;
int lastTemp;
double rampHours;
double calcSetPoint;

        
char schedDesc1[21]; 
byte optionNum = 1;     
LiquidCrystal_I2C lcd(0x27, 20, 4); 

//PROGRAM CREATOR
File myfile;
float tempVal;
unsigned int tempRamp;
unsigned long starttime;
unsigned long starttime2;
unsigned int fileNumber = 13;
byte count = 0;
char fileName[9];
byte segmentNum = 1;
boolean yes = false;
byte interface = 0;
byte segments = 1;
byte totalSegments;

//Provisioning
const char *DeviceName = "Polygon Kioto 001";
Preferences preferences;
boolean Provisioned;


// WIFI
const char *serverName = "http://polygonhornos.com/1.php";
String apiKeyValue = "tPmAT5Ab3j7F9";


//setupPIDs
const int pidCycle = 1000;            

//htrControl
unsigned long glitchTimer;
unsigned long pidStart;  

//openSchedule
int schedNum = 1;           
boolean schedOK = false;    
float tempo = 0;
int i = 0; 
int hour;
int minute;  