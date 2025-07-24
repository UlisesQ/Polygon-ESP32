// LCDUpdater.cpp
#include "updateLCD.h"
#include "variables.h"
#include <LiquidCrystal_I2C.h>
#include "readTemps.h"
void updateLCD()
{

  // Clear screen and set cursor to top left
  lcd.clear();

  // Temperatures
  if (screenNum == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print(("Temperatura:"));
    lcd.setCursor(13, 0);
    lcd.print((int)pidInput);
    lcd.print((" "));
    lcd.setCursor(17, 0);
    lcd.print((char)223);
    lcd.print(tempScale);

    lcd.setCursor(0, 1);
    lcd.print(("SetPoint:"));
    lcd.setCursor(13, 1);
    lcd.print((int)pidSetPoint);
    lcd.print((" "));
    lcd.setCursor(17, 1);
    lcd.print((char)223);
    lcd.print(tempScale);

    lcd.setCursor(0, 2);
    lcd.print(("Objetivo:"));
    lcd.setCursor(13, 2);
    lcd.print(int(segTemp[segNum - 1]));
    lcd.print((" "));
    lcd.setCursor(17, 2);
    lcd.print((char)223);
    lcd.print(tempScale);
  }

  // Schedule / segment info
  if (screenNum == 2)
  {
    lcd.print(schedDesc1);
    lcd.setCursor(0, 1);
    lcd.print(F("ETAPA: "));
    lcd.print(segNum);
    lcd.print(F(" / "));
    lcd.print(lastSeg);

    if (segPhase == 0)
    {
      lcd.setCursor(0, 2);
      lcd.print(F("Rampa a "));
      lcd.print(int(segTemp[segNum - 1]));
      lcd.print(F(" "));
      lcd.print((char)223);
      lcd.print(tempScale);

      lcd.setCursor(0, 3);
      lcd.print(F("Velocidad: "));
      lcd.print((segRamp[segNum - 1] / 60), 1);
      lcd.print((char)223);
      lcd.print(tempScale);
      lcd.print(F("/min"));
    }
    else
    {
      lcd.setCursor(0, 2);
      lcd.print(F("Meseta: "));
      lcd.print(int(segTemp[segNum - 1]));
      lcd.print(F(" "));
      lcd.print((char)223);
      lcd.print(tempScale);

      lcd.setCursor(0, 3);
      lcd.print(F("Restante: "));
      lcd.print(int(segHold[segNum - 1] - (millis() - holdStart) / 60000));
      lcd.print(F(" min"));
    }
  }

  // Tools
  if (screenNum == 3)
  {
    lcd.print(F("    HERRAMIENTAS:"));
    lcd.setCursor(2, 1);
    lcd.print(F("Sumar 5 minutos"));
    lcd.setCursor(2, 2);
    lcd.print(F("Sumar 5 grados"));
    lcd.setCursor(2, 3);
    lcd.print(F("Omitir tramo"));
    lcd.setCursor(0, optionNum);
    lcd.print(F(">"));
    lcd.setCursor(19, optionNum);
    lcd.print(F("<"));
  }

  // Schedule completed
  if (screenNum == 4 && !isnan(pidInput))
  {
    readTemps();

    lcd.print(F(" PROGRAMA TERMINADO"));
    lcd.setCursor(0, 1);
    lcd.print(F("Esperar enfriamiento"));
    lcd.setCursor(3, 2);
    lcd.print(F("antes de abrir"));
    lcd.setCursor(1, 3);
    lcd.print(F("Temperatura: "));
    lcd.print((int)pidInput);
    lcd.print((char)223);
    lcd.print(tempScale);
  }

  if (screenNum == 4 && isnan(pidInput))
  {
    readTemps();
    lcd.clear();
    lcd.print(F(" PROGRAMA TERMINADO"));
    lcd.setCursor(0, 1);
    lcd.print(F("Esperar enfriamiento"));
    lcd.setCursor(3, 2);
    lcd.print(F("antes de abrir"));
    lcd.setCursor(1, 3);
    lcd.print(F("Temperatura: "));
    lcd.print("");
    lcd.print((char)223);
    lcd.print(tempScale);
  }

  // PROGRAM CREATOR

  if (screenNum == 8)
  {
    if (count == 0)
    {
      lcd.print(F("     Programa "));
      lcd.setCursor(14, 0);
      lcd.print(fileNumber - 12);
      lcd.setCursor(0, 1);
      lcd.print(F("Etapa:"));
      lcd.setCursor(8, 1);
      lcd.print(segmentNum);
      lcd.print(F("/"));
      lcd.print(totalSegments);
      lcd.setCursor(0, 2);
      lcd.print(F("Rampa:"));
      lcd.setCursor(7, 2);
      lcd.print(tempVal, 1);
      lcd.print(F(" "));
      lcd.print((char)223);
      lcd.print(F("C"));
      lcd.print(F("/min"));
    }

    if (count == 1)
    {
      lcd.print(F("     Programa "));
      lcd.setCursor(14, 0);
      lcd.print(fileNumber - 12);
      lcd.setCursor(0, 1);
      lcd.print(F("Etapa:"));
      lcd.setCursor(8, 1);
      lcd.print(segmentNum);
      lcd.print(F("/"));
      lcd.print(totalSegments);
      lcd.setCursor(0, 2);
      lcd.print(F("Temperatura:"));
      lcd.setCursor(13, 2);
      lcd.print(tempRamp);
      lcd.print(F(" "));
      lcd.print((char)223);
      lcd.print(F("C"));
    }

    if (count == 2)
    {
      lcd.print(F("     Programa "));
      lcd.setCursor(14, 0);
      lcd.print(fileNumber - 12);
      lcd.setCursor(0, 1);
      lcd.print(F("Etapa:"));
      lcd.setCursor(8, 1);
      lcd.print(segmentNum);
      lcd.print(F("/"));
      lcd.print(totalSegments);
      lcd.setCursor(0, 2);
      lcd.print(F("Meseta:"));
      lcd.setCursor(8, 2);
      lcd.print(int(tempVal));
      lcd.print(F(" min"));
    }
  }

  if (screenNum == 6)
  {
    lcd.print(F("Elegir programa"));
    lcd.setCursor(0, 2);
    lcd.print(F("Programa:"));
    lcd.setCursor(10, 2);
    lcd.print(fileNumber - 12);
  }

  if (screenNum == 7)
  {
    if (yes == true)
    {
      lcd.print(F(" Programa existente"));
      lcd.setCursor(0, 1);
      lcd.print(F("   Sobreescribir?"));
      lcd.setCursor(4, 3);
      lcd.print(F(">Si"));
      lcd.setCursor(13, 3);
      lcd.print(F("No"));
    }
    if (yes == false)
    {
      lcd.print(F(" Programa existente"));
      lcd.setCursor(0, 1);
      lcd.print(F("   Sobreescribir?"));
      lcd.setCursor(5, 3);
      lcd.print(F("Si"));
      lcd.setCursor(12, 3);
      lcd.print(F(">No"));
    }
  }

  if (screenNum == 5)
  {
    lcd.print(F("  Numero de Etapas"));
    lcd.setCursor(0, 2);
    lcd.print(F("Etapas:"));
    lcd.setCursor(8, 2);
    lcd.print(segments);
  }

  if (screenNum == 9)
  {
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
  }

  if (screenNum == 10)
  {
    lcd.print(F("   CONFIGURACION"));
    lcd.setCursor(8, 1);
    lcd.print(F("WiFi"));
    lcd.setCursor(5, 2);
    lcd.print(F("Actualizar"));
    lcd.setCursor(7, 3);
    lcd.print(F("Atras"));
    lcd.setCursor(0, optionNum);
    lcd.print(F(">"));
    lcd.setCursor(19, optionNum);
    lcd.print(F("<"));
  }

  if (screenNum == 11)
  {
    lcd.setCursor(8, 0);
    lcd.print(F("WIFI"));
    lcd.setCursor(6, 1);
    lcd.print(F("Conectar"));
    lcd.setCursor(5, 2);
    lcd.print(F("Olvidar Red"));
    lcd.setCursor(8, 3);
    lcd.print(F("Atras"));
    lcd.setCursor(0, optionNum);
    lcd.print(F(">"));
    lcd.setCursor(19, optionNum);
    lcd.print(F("<"));
  }


}