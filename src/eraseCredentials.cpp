//******************************************************************************************************************************
//  ERASE WIFI CREDENTIALS
//******************************************************************************************************************************

#include "nvs_flash.h"
#include "nvs.h"
#include "variables.h"
#include <LiquidCrystal_I2C.h>
#include <SPI.h>   
#include "eraseCredentials.h"
void eraseCredentials() {
  nvs_flash_erase();
  nvs_flash_init();
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print(F("Red Olvidada"));
  lcd.setCursor(4, 2);
  lcd.print(F("Sin conexion"));
  delay(1500);
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print(F("Reiniciando..."));
  delay(1500);
  ESP.restart();
}
