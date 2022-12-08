#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "us.h"


//超音波の有無が入力されるピン
#define USIN 21

//チャタリング対策の待機時間
#define WAIT_TIME 50

static unsigned long UsTime = 0;

int Check_Hit() {
  
  if ( ( millis() - UsTime ) >= WAIT_TIME ) {
    boolean hit = digitalRead(USIN);
    if ( hit == SPN_HIT ) {
      UsTime = millis();
      return HITSOUND;
    }
    else {
      return IGNORE;
    }
  }

  else {
    return IGNORE;
  }
  
}
