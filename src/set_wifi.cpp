// WiFi
#include <WiFi.h> // Wi-Fi接続用ライブラリ　なくても行けるはず

//#include "firebase.h"
#include "set_wifi.h"
#include "header.h"
#include "microSD.h"

//wifi接続
String Wifi_Connect() {
  GetWifi_info();
  String StrSsid = Get_Ssid(String(1));
  String StrPass = Get_Pass(String(1));

  char Ssid[32];
  char Pass[32];

  StrSsid.toCharArray(Ssid, 32);
  StrPass.toCharArray(Pass, 32);

  Serial.print(Ssid);
  Serial.print("\n");
  Serial.print(Pass);
  Serial.print("\n");

  
  WiFi.begin(Ssid, Pass);
  // WiFi接続が完了するまで待つ。
  while (WiFi.status() != WL_CONNECTED){
    delay(500); // 0.5秒間隔
  }

   return WiFi.macAddress();

}