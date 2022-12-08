#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>


#include "btn.h"


//不具合対策版
int Btn_A() {
  if ( M5.BtnA.pressedFor(10) ) {
    return BTN_PUSHED;
  }
  else {
    return BTN_USUAL;
  }
}

int Btn_B() {
  if ( M5.BtnB.pressedFor(10) ) {
    return BTN_PUSHED;
  }
  else {
    return BTN_USUAL;
  }
}

int Btn_C() {
  if ( M5.BtnC.pressedFor(10) ) {
    return BTN_PUSHED;
  }
  else {
    return BTN_USUAL;
  }
}

void BtnA_Wait() {
  do {
    M5.update();
  } while ( M5.BtnA.isPressed() );
  return;
}

void BtnB_Wait() {
  do {
    M5.update();
  } while ( M5.BtnB.isPressed() );
  return;
}

void BtnC_Wait() {
  do {
    M5.update();
  } while ( M5.BtnC.isPressed() );
  return;
}

/*
int Btn_A() {
  if ( M5.BtnA.wasPressed() ) {
    return BTN_PUSHED;
  }
  else {
    return BTN_USUAL;
  }
}

int Btn_B() {
  if ( M5.BtnB.wasPressed() ) {
    return BTN_PUSHED;
  }
  else {
    return BTN_USUAL;
  }
}

int Btn_C() {
  if ( M5.BtnC.wasPressed() ) {
    return BTN_PUSHED;
  }
  else {
    return BTN_USUAL;
  }
}
*/
