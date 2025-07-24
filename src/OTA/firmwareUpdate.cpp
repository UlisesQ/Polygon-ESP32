#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include "variables.h"
#include "firmwareUpdate.h"


#define URL_fw_Bin "http://www.polygonhornos.com/update/fw.bin"

void firmwareUpdate(void)
{
  WiFiClient client;
  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret)
  {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
  
}