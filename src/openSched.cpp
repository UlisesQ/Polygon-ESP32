//******************************************************************************************************************************
//  OPENSCHED: OPEN AND LOAD A FIRING SCHEDULE FILE / DISPLAY ON SCREEN
//******************************************************************************************************************************
#include <Arduino.h>
#include <SPIFFS.h>
#include "variables.h"
#include "openSched.h"
#include "intLength.h"
void openSched() {


  // Setup all variables
  int col = 1;          // Column number (of text file).  First column is one.
  int row = 1;          // Row number (of text file).  First row is one.
  char tempChar;        // Temporary character holder (read one at a time from file)
  char tempLine[21];    // Temporary character array holder
  int tempLoc = 0;      // Current location of next character to place in tempLine array
  char schedDesc2[21];  // Schedule description #2 (second line of text file)
  char schedDesc3[21];  // Schedule description #3 (third line of text file)
  tempo = 0;
  // Clear the arrays
  memset(schedDesc1, 0, sizeof(schedDesc1));
  memset(segRamp, 0, sizeof(segRamp));
  memset(segTemp, 0, sizeof(segTemp));
  memset(segHold, 0, sizeof(segHold));



  // Make sure you can open the file
  sprintf(tempLine, "/%d.txt", schedNum);
  File myFile = SPIFFS.open(tempLine, FILE_READ);

  if (!myFile) {
    lcd.clear();
    lcd.print(F("SELECCIONE PROGRAMA"));
    lcd.setCursor(0, 1);
    lcd.print("PROGRAMA ");
    lcd.print(schedNum);
    lcd.setCursor(0, 2);
    lcd.print(F("[VACIO]"));
    schedOK = false;
    return;
  }


  // Load the data
  while (myFile.available() > 0) {

    // Read a single character
    tempChar = myFile.read();

    if (tempChar == 13) {       // Carriage return: Read another char (it is always a line feed / 10).  Add null to end.
      myFile.read();
      tempLine[tempLoc] = '\0';
    }
    else if (tempChar == 44) {  // Comma: Add null to end.
      tempLine[tempLoc] = '\0';
    }
    else if (tempLoc <= 19) {   // Add it to the temp line array
      tempLine[tempLoc] = tempChar;
      tempLoc = tempLoc + 1;
    }

    if (row == 1 && tempChar == 13) {
      memcpy(schedDesc1, tempLine, 21);
    }
    else if (row == 2 && tempChar == 13) {
      memcpy(schedDesc2, tempLine, 21);
    }
    else if (row == 3 && tempChar == 13) {
      memcpy(schedDesc3, tempLine, 21);
    }
    else if (row >= 4 && col == 1 && tempChar == 44) {
      segRamp[row - 4] = atoi(tempLine);
    }
    else if (row >= 4 && col == 2 && tempChar == 44) {
      segTemp[row - 4] = atoi(tempLine);
    }
    else if ((row >= 4 && col == 3 && tempChar == 13) || myFile.available() == 0) {
      segHold[row - 4] = atoi(tempLine);
    }

    if (tempChar == 13) {  // End of line.  Reset everything and goto next line
      memset(tempLine, 0, 21);
      tempLoc = 0;
      row = row + 1;
      col = 1;
    }

    if (tempChar == 44) {  // Comma.  Reset everything and goto 1st column
      memset(tempLine, 0, 21);
      tempLoc = 0;
      col = col + 1;
    }

  }  // end of while(myFile.available ...

  // Close the file
  myFile.close();

  //Empty if no data on file
  if (segRamp[0] == 0.00) {
    lcd.clear();
    lcd.print(F("SELECCIONE PROGRAMA"));
    lcd.setCursor(0, 1);
    lcd.print("PROGRAMA ");
    lcd.print(schedNum);
    lcd.setCursor(0, 2);
    lcd.print(F("[VACIO]"));
    schedOK = false;
    return;
  }

  // Set some variables
  lastSeg = row - 3;
  schedOK = true;


  // Calculate duration of firing.
  for (i = 0; i < 20; i++) {
    if (segTemp[i] != 0) {
      if (i == 0) {
        tempo += (segHold[i] + (((segTemp[i] - 24) / segRamp[i]) * 60));
      }
      if (i > 0) {
        if (segTemp[i] > segTemp[i - 1]) {
          tempo += (segHold[i] + (((segTemp[i] - segTemp[i - 1]) / segRamp[i]) * 60));
        }
        if (segTemp[i] < segTemp[i - 1]) {
          tempo += (segHold[i] + (((segTemp[i - 1] - segTemp[i]) / segRamp[i])) * 60);
        }
      }
    }
  }
  tempo = tempo / 60;
  hour = (int)tempo;
  minute = (tempo - hour) * 60;



  // Fix Ramp values so it will show the correct sign (+/-).  This will help to determine when to start hold.
  for (i = 0; i < lastSeg; i++) {
    segRamp[i] = abs(segRamp[i]);
    if (i >= 1) {
      if (segTemp[i] < segTemp[i - 1]) {
        segRamp[i] = -segRamp[i];
      }
    }
  }

  // Display on the screen
  lcd.clear();
  lcd.print(F("SELECCIONE PROGRAMA"));
  lcd.setCursor(0, 1);
  lcd.print(schedDesc1);
  lcd.setCursor(0, 2);
  lcd.print(schedDesc2);
  lcd.setCursor(0, 3);
  lcd.print(F("DURACION: "));
  if (hour < 10) {
    lcd.print(F("0"));
  }
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10) {
    lcd.print(F("0"));
  }
  lcd.print(minute);

  // Cut down schedule description so it shows better on other screen when running
  schedDesc1[14 - intLength(schedNum)] = '\0';

}