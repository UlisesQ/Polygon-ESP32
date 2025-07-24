#include <Arduino.h>
#include <LiquidCrystal_I2C.h> // LCD display library
#include <Wire.h>
#include <Adafruit_MAX31855.h> // Thermocouple card library
#include <PID_v1.h>            // PID temp control library
#include <SPI.h>               // Serial Peripheral Interface library
#include "FS.h"
#include "SPIFFS.h"
#include "WiFiProv.h"
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <Preferences.h>
#include "intLength.h"
#include "updateSeg.h"
#include "updateLCD.h"
#include "variables.h"
#include "readTemps.h"
#include "updatePIDs.h"
#include "firmwareUpdate.h"
#include "FirmwareVersionCheck.h"
#include "WIFII.h"
#include "eraseCredentials.h"
#include "SysProvEvent.h"
#include "shutDown.h"
#include "openSched.h"
#include "setupPIDs.h"
#include "btnBounce.h"
#include "htrControl.h"




const int lcdRefresh = 2500; // Refresh rate to update screen when running (ms)
const int maxTemp = 1302;    // Maximum temperature (degrees).  If reached, will shut down.
const int upPin = 4;         // Pin # connected to up arrow button
const int downPin = 17;      // Pin # connected to down arrow button
const int selectPin = 15;    // Pin # connected to select / start button
unsigned long lcdStart;      // Exact time you refreshed the lcd screen (ms).  Based on millis().
unsigned long schedStart;    // Exact time you started running the schedule (ms).  Based on millis().
unsigned long overheatTimer;
boolean t1 = false;

//******************************************************************************************************************************
//  SETUP: INITIAL SETUP (RUNS ONCE DURING START)
//******************************************************************************************************************************
void setup()
{
  // Setup all pin modes on board.  Remove INPUT_PULLUP if you have resistors in your wiring.

  // WIFI ETC...
  preferences.begin("Provisioned", false);
  Provisioned = preferences.getBool("prov", false);

  Serial.begin(115200);
  Serial.println(Provisioned);
  WiFi.setHostname(DeviceName);
  WiFi.begin();


  Wire.setClock(10000);
  Wire.beginTransmission(0);
  Wire.write(0x0C);
  Wire.write(1000);
  Wire.endTransmission();

  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(heaterPin, OUTPUT);

  // Setup lcd display (20 columns x 4 rows)
  lcd.init();
  lcd.backlight();



  // Setup SD card
  if (!SPIFFS.begin(true))
  {
    lcd.print(F("       ERROR:"));
    lcd.setCursor(0, 2);
    lcd.print(F("Memoria No Disponible"));
    lcd.setCursor(0, 3);
    lcd.print(F("Reiniciar"));
    shutDown();
  }


  // Intro screens

  lcd.print(F("   POLYGON HORNOS"));
  lcd.setCursor(3, 1);
  lcd.print(F("--------------"));
  lcd.setCursor(1, 3);
  lcd.print(F("Polygon1100TK v2.6"));
  delay(2000);
  lcd.clear();

  lcd.print(F("       INICIO"));
  lcd.setCursor(2, 1);
  lcd.print(F("Iniciar programa"));
  lcd.setCursor(2, 2);
  lcd.print(F("Crear programa"));
  lcd.setCursor(2, 3);
  lcd.print(F("Configuracion"));
  lcd.setCursor(0, optionNum);
  lcd.print(F(">"));
  lcd.setCursor(19, optionNum);
  lcd.print(F("<"));
  Serial.begin(115200);
}

//******************************************************************************************************************************
//  LOOP: MAIN LOOP (CONTINUOUS)
//******************************************************************************************************************************
void loop()
{

  // NEW INTERFACE

  if (interface == 0) {

    // Up arrow button
    if (digitalRead(upPin) == LOW)
    {
      if (optionNum >= 2)
      {
        optionNum = optionNum - 1;
      }
      updateLCD();
      btnBounce(upPin);
    }

    // Down arrow button
    if (digitalRead(downPin) == LOW)
    {
      if (optionNum <= 2)
      {
        optionNum = optionNum + 1;
      }
      updateLCD();
      btnBounce(downPin);
    }

    // Select button
    if (digitalRead(selectPin) == LOW)
    {
      if (optionNum == 1)
      {
        openSched();
        screenNum = 1;
        interface = 2;
        delay(300);
      }
      if (optionNum == 2)
      {
        interface = 1;
        screenNum = 10;
        updateLCD();
        delay(300);
      }

      if (optionNum == 3)
      {
        interface = 3;
        screenNum = 10;
        optionNum = 1;
        updateLCD();
        delay(300);
      }
    }
  }

  if (interface == 1)
  {
    if (screenNum == 6)
    {
      sprintf(fileName, "/%d.txt", fileNumber); // Create name for SD file

      if (digitalRead(upPin) == LOW)
      {
        fileNumber = fileNumber + 1; // Change name for file +
        updateLCD();
        delay(300);
      }
      if (digitalRead(downPin) == LOW && fileNumber > 13)
      {
        fileNumber = fileNumber - 1; // Change name for file -
        updateLCD();
        delay(300);
      }
      if (digitalRead(selectPin) == LOW && SPIFFS.exists(fileName) == true)
      {
        btnBounce(selectPin);
        screenNum = 7;
        updateLCD();
        delay(300);
      }
      if (digitalRead(selectPin) == LOW && SPIFFS.exists(fileName) == false)
      {
        btnBounce(selectPin);
        screenNum = 5;
        updateLCD();
        delay(300);
      }
    }

    if (screenNum == 7)
    {
      if (yes == true && digitalRead(downPin) == LOW)
      {
        yes = false;
        updateLCD();
        delay(300);
      }
      if (yes == false && digitalRead(upPin) == LOW)
      {
        yes = true;
        updateLCD();
        delay(300);
      }
      if (yes == false && digitalRead(selectPin) == LOW)
      {
        btnBounce(selectPin);
        screenNum = 6;
        updateLCD();
        delay(300);
      }
      if (yes == true && digitalRead(selectPin) == LOW)
      {
        btnBounce(selectPin);
        SPIFFS.remove(fileName);
        myfile.close();
        screenNum = 5;
        updateLCD();
        delay(300);
      }
    }

    if (screenNum == 5)
    {
      if (digitalRead(upPin) == LOW && segments < 20)
      {
        segments = segments + 1;
        btnBounce(upPin);
        updateLCD();
        delay(150);
      }

      if (digitalRead(downPin) == LOW && segments > 1)
      {
        segments = segments - 1;
        updateLCD();
        btnBounce(downPin);
        delay(150);
      }

      if (digitalRead(selectPin) == LOW && segments > 0)
      {
        screenNum = 8;
        totalSegments = segments;
        btnBounce(selectPin);
        updateLCD();
        myfile = SPIFFS.open(fileName, FILE_WRITE);
        myfile.print(fileNumber);
        myfile.print(" PERSONALIZADO ");
        myfile.print(fileNumber - 12);
        myfile.print("\r\n");
        myfile.print("ETAPAS: ");
        myfile.print(segments);
        myfile.print("\r\n\r\n");
        myfile.close();
        delay(200);
      }
    }
    if (screenNum == 8)
    {
      if (segments > 0)
      {
        if (count == 0 || count == 2)
        {
          while (digitalRead(upPin) == LOW && (millis() - starttime) < 1000)
          {

            if (count == 0)
            {
              tempVal = tempVal + 0.5;
            }
            else
            {
              tempVal = tempVal + 1;
            }
            updateLCD();
            delay(250);
          }

          if (digitalRead(upPin) == LOW && (millis() - starttime) >= 1000)
          {

            tempVal = tempVal + 10;
            updateLCD();
            delay(100);
          }
          if (digitalRead(upPin) == HIGH)
          {
            starttime = millis();
          }

          while (digitalRead(downPin) == LOW && (millis() - starttime2) < 1000 && tempVal > 0)
          {
            if (count == 0)
            {
              tempVal = tempVal - 0.5;
            }
            else
            {
              tempVal = tempVal - 1;
            }
            updateLCD();
            delay(250);
          }

          if (digitalRead(downPin) == LOW && (millis() - starttime2) >= 1000 && tempVal > 0)
          {
            if (tempVal < 10)
            {
              tempVal = 0;
              updateLCD();
            }
            else
            {
              tempVal = tempVal - 10;
              updateLCD();
              delay(100);
            }
          }
          if (digitalRead(downPin) == HIGH)
          {
            starttime2 = millis();
          }
        }
        if (count == 1)
        {
          while (digitalRead(upPin) == LOW && (millis() - starttime) < 1000 && tempRamp <= 1299)
          {

            tempRamp = tempRamp + 1;
            updateLCD();
            delay(250);
          }

          if (digitalRead(upPin) == LOW && (millis() - starttime) >= 1000 && tempRamp <= 1290)
          {

            tempRamp = tempRamp + 10;
            updateLCD();
            delay(100);
          }

          if (digitalRead(upPin) == LOW && (millis() - starttime) >= 1000 && tempRamp > 1290)
          {

            tempRamp = 1300;
            updateLCD();
            delay(100);
          }
          if (digitalRead(upPin) == HIGH)
          {
            starttime = millis();
          }

          while (digitalRead(downPin) == LOW && (millis() - starttime2) < 1000 && tempRamp > 0)
          {
            tempRamp = tempRamp - 1;
            updateLCD();
            delay(250);
          }

          if (digitalRead(downPin) == LOW && (millis() - starttime2) >= 1000 && tempRamp > 0)
          {
            if (tempRamp < 10)
            {
              tempRamp = 0;
              updateLCD();
            }
            else
            {
              tempRamp = tempRamp - 10;
              updateLCD();
              delay(100);
            }
          }
          if (digitalRead(downPin) == HIGH)
          {
            starttime2 = millis();
          }
        }

        if (digitalRead(selectPin) == LOW && count == 1)
        {
          btnBounce(selectPin);
          myfile = SPIFFS.open(fileName, FILE_APPEND);
          myfile.print(tempRamp);
          myfile.print(",");

          starttime = millis();
          starttime2 = millis();
          count = count + 1;
          updateLCD();
          delay(300);
          myfile.close();
        }

        if (digitalRead(selectPin) == LOW && count == 0)
        {
          btnBounce(selectPin);
          myfile = SPIFFS.open(fileName, FILE_APPEND);
          myfile.print(tempVal * 60);
          myfile.print(",");
          count = count + 1;
          tempVal = 0;
          updateLCD();
          starttime = millis();
          starttime2 = millis();
          delay(300);
          myfile.close();
        }

        if (digitalRead(selectPin) == LOW && count == 2)
        {
          btnBounce(selectPin);
          myfile = SPIFFS.open(fileName, FILE_APPEND);
          myfile.print(tempVal);
          starttime = millis();
          starttime2 = millis();
          count = 0;
          tempVal = 0;
          segments = segments - 1;
          if (segments > 0)
          {
            myfile.print("\r\n");
            segmentNum = segmentNum + 1;
            updateLCD();
          }
          delay(300);
          myfile.close();
        }
      }
      if (segments == 0)
      {
        delay(300);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(F("  Programa creado"));
        lcd.setCursor(0, 2);
        lcd.print(F("   Existosamente!"));
        delay(2500);
        interface = 0;
        screenNum = 9;
        segmentNum = 1;
        updateLCD();
        yes = false;
        tempRamp = 0;
        fileNumber = 13;
        segments = 1;
      }
    }
  }

  //******************************
  // Open firing shedule # 1

  if (interface == 2)
  {

    //******************************
    // Shut down if too hot

    if (pidInput >= maxTemp && t1 == false)
    {
      t1 = true;
      overheatTimer = millis();
      delay(500);
    }
    if (t1 && millis() - overheatTimer > 60000)
    {
      lcd.clear();
      lcd.print(F("       ERROR:"));
      lcd.setCursor(1, 2);
      lcd.print(F("Sobrecalentamiento"));
      lcd.setCursor(5, 3);
      lcd.print(F("Reiniciar"));
      shutDown();
    }
    if (pidInput < maxTemp && t1)
    {
      t0 = false;
      overheatTimer = 4000000000;
      delay(500);
    }

    //******************************
    // Select a firing schedule
    if (segNum == 0)
    {

      // Up arrow button
      if (digitalRead(upPin) == LOW && schedNum > 1)
      {
        schedNum = schedNum - 1;
        openSched();
        btnBounce(upPin);
      }

      // Down arrow button
      if (digitalRead(downPin) == LOW)
      {
        schedNum = schedNum + 1;
        openSched();
        btnBounce(downPin);
      }

      // Select / Start button
      if (digitalRead(selectPin) == LOW && schedOK == true)
      {
        setupPIDs();
        segNum = 1;
        lcdStart = millis();
        pidStart = millis();
        rampStart = millis();
        schedStart = millis();
        updateLCD();
        btnBounce(selectPin);
      }
    }

    //******************************
    // Running the firing schedule
    if (segNum >= 1)
    {

      // Up arrow button
      if (digitalRead(upPin) == LOW)
      {
        if (screenNum == 2 || (screenNum == 3 && optionNum == 1))
        {
          screenNum = screenNum - 1;
        }
        else if (screenNum == 3 && optionNum >= 2)
        {
          optionNum = optionNum - 1;
        }
        updateLCD();
        btnBounce(upPin);
      }

      // Down arrow button
      if (digitalRead(downPin) == LOW)
      {
        if (screenNum <= 2)
        {
          screenNum = screenNum + 1;
        }
        else if (screenNum == 3 && optionNum <= 2)
        {
          optionNum = optionNum + 1;
        }
        updateLCD();
        btnBounce(downPin);
      }
      Serial.println(pidInput);
      // Select / Start button
      if (digitalRead(selectPin) == LOW && screenNum == 3)
      {
        if (optionNum == 1)
        { // Add 5 min
          segHold[segNum - 1] = segHold[segNum - 1] + 5;
          optionNum = 1;
          screenNum = 2;
        }

        if (optionNum == 2)
        { // Add 5 deg
          segTemp[segNum - 1] = segTemp[segNum - 1] + 5;
          optionNum = 1;
          screenNum = 1;
        }

        if (optionNum == 3)
        { // Goto next segment
          segNum = segNum + 1;
          optionNum = 1;
          screenNum = 2;
        }

        updateLCD();
        btnBounce(selectPin);
      }

      // Update PID's / turn on heaters / update segment info
      if (screenNum < 4)
      {
        if (millis() - pidStart >= pidCycle)
        {
          pidStart = millis();
          updatePIDs();
        }
        htrControl();
        updateSeg();
      }

      // Refresh the LCD
      if (millis() - lcdStart >= lcdRefresh && !isnan(pidInput))
      {
        updateLCD();
        WIFII();
        lcdStart = millis();
      }
      if (millis() - lcdStart >= lcdRefresh && isnan(pidInput))
      {
        readTemps();
        lcdStart = millis();
      }
    }
  }

  if (interface == 3)
  {
    // Up arrow button
    if (digitalRead(upPin) == LOW)
    {
      if (optionNum >= 2)
      {
        optionNum = optionNum - 1;
      }
      updateLCD();
      btnBounce(upPin);
    }

    // Down arrow button
    if (digitalRead(downPin) == LOW)
    {
      if (optionNum <= 2)
      {
        optionNum = optionNum + 1;
      }
      updateLCD();
      btnBounce(downPin);
    }

    // Select button
    if (digitalRead(selectPin) == LOW)
    {
      if (optionNum == 1)
      {
        screenNum = 11;
        interface = 4;
        updateLCD();
        delay(300);
      }
      if (optionNum == 2)
      {
        screenNum = 10;
        updateLCD();
        delay(300);
        if (FirmwareVersionCheck())
        {
          firmwareUpdate();
        }
      }
      if (optionNum == 3)
      {
        interface = 0;
        screenNum = 9;
        updateLCD();
        delay(300);
      }


    }
  }


  if (interface == 4) {
    // Up arrow button
    if (digitalRead(upPin) == LOW)
    {
      if (optionNum >= 2)
      {
        optionNum = optionNum - 1;
      }
      updateLCD();
      btnBounce(upPin);
    }

    // Down arrow button
    if (digitalRead(downPin) == LOW)
    {
      if (optionNum <= 2)
      {
        optionNum = optionNum + 1;
      }
      updateLCD();
      btnBounce(downPin);
    }

    // Select button
    if (digitalRead(selectPin) == LOW)
    {
      if (optionNum == 1) {
        if (WiFi.status() != WL_CONNECTED && Provisioned == false) {
          lcd.clear();
          lcd.setCursor(4, 1);
          lcd.print(F("Buscando Red"));
          lcd.setCursor(2, 2);
          lcd.print(F("Continuar en App"));

          WiFi.onEvent(SysProvEvent);
          WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_0, "abcd1234", DeviceName, NULL, NULL);



          while (Provisioned == false) {
            delay(1);
          }
          delay(1000);


        }
        if (WiFi.status() == WL_CONNECTED && Provisioned == true) {
          lcd.clear();
          lcd.print(F(" "));
          lcd.setCursor(2, 1);
          lcd.print(F("Conectado a WIFI"));
          delay(1500);
          screenNum = 11;
          interface = 4;
          updateLCD();
          delay(300);
        }
        if (WiFi.status() != WL_CONNECTED && Provisioned == true) {
          lcd.clear();
          lcd.print(F(" "));
          lcd.setCursor(2, 1);
          lcd.print(F("Fallo la conexion"));
          lcd.setCursor(5, 2);
          lcd.print(F("Olvidar Red"));
          delay(1500);
          screenNum = 11;
          interface = 4;
          updateLCD();
          delay(300);
        }
      }

      if (optionNum == 2)
      {
        eraseCredentials();
        WiFi.disconnect(true);
        interface = 4;
        screenNum = 11;
        updateLCD();
        delay(300);
      }



      if (optionNum == 3)
      {
        interface = 3;
        screenNum = 10;
        updateLCD();
        delay(300);
      }
    }

  }

}