#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include "variables.h"
#include "updateLCD.h"
#include "FirmwareVersionCheck.h"


int httpCode;
String FirmwareVer = {"2.1"};
#define URL_fw_Version "http://www.polygonhornos.com/update/bin_version.txt"
int FirmwareVersionCheck(void)
{
  String payload;
  
  String fwurl = "";
  fwurl += URL_fw_Version;
  Serial.println(fwurl);
  WiFiClient *client = new WiFiClient;

  if (client)
  {
    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;

    if (https.begin(*client, fwurl))
    { // HTTPS
      lcd.clear();
      lcd.print(F(" "));
      lcd.setCursor(2, 1);
      lcd.print(F("Buscando Version"));
      delay(1500);
      // start connection and send HTTP header
      httpCode = https.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
      }
      else
      {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
        lcd.clear();
        lcd.setCursor(1, 1);
        lcd.print(F("Error Actualizando"));
        lcd.setCursor(1, 2);
        lcd.print(F("Verificar Conexion"));
        delay(2000);
        updateLCD();
      }
      https.end();
    }
    delete client;
  }

  if (httpCode == HTTP_CODE_OK) // if version received
  {
    payload.trim();
    if (payload.equals(FirmwareVer))
    {

      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print(F("Ultima version"));
      lcd.setCursor(4, 2);
      lcd.print(F("Versión: "));
      lcd.print(FirmwareVer);
      delay(2000);
      updateLCD();
      return 0;
    }
    else
    {
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print(F("Nueva version!"));
      lcd.setCursor(4, 2);
      lcd.print(F("Version: "));
      lcd.print(payload);
      delay(1500);
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print(F("Instalando..."));
      delay(1500);
      return 1;
    }
  }
  return 0;
}