#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "btn.h"
#include "set_code.h"
#include "connect.h"
#include "web_blue.h"
#include "battery.h"

#define LGFX_M5STACK
#include <LovyanGFX.hpp>

extern LGFX lcd;


//ここから 座標などの定義
#define CONNECTED     true //接続状態を表す
#define NOT_CONNECTED false

#define TITLE_TEXT_SIZE 2.5 //タイトルのサイズと座標
#define TITLE_X         10
#define TITLE_Y          5

#define TEXT_SIZE 2 //文字サイズ

#define BTN_Y   195 //仮想ボタンの座標や文字の座標
#define BTN_W   110
#define BTN_H    40
#define BTN_RAD  10
#define STR_Y   202

#define CON_BTN_X 40 //接続開始ボタン
#define CON_STR_X 48

#define RET_BTN_X 170 //戻るボタン
#define RET_STR_X 200

#define CON_TEXT_SIZE 2.5 //接続状態を表示する際の文字サイズ
#define CON_TEXT_X    30
#define CON_TEXT_Y    90

#define CONNECTING_TEXT_SIZE   3 //接続中に表示する文字サイズ
#define CONNECTING_TEXT_X     50
#define CONNECTING_TEXT_Y    100

#define MACADDRESS_TEXT_SIZE  2 //接続中に表示するmacアドレス
#define MACADDRESS_TEXT_X    10 
#define MACADDRESS_TEXT_Y    10


//ここまで

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

static BLEServer* pServer = NULL;
static BLECharacteristic* pCharacteristic = NULL;
static bool DeviceConnected = false;

static void Web_State_Disp(boolean);

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      //lcd.println("connect");
      DeviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      //lcd.println("disconnect");
      DeviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    //lcd.println("read");
    pCharacteristic->setValue("Hello");
  }

  void onWrite(BLECharacteristic *pCharacteristic) {
    //lcd.println("write");
    std::string value = pCharacteristic->getValue();
    Web_Code_Rec(value.c_str());
    //lcd.println(value.c_str());
  }
};

void Web_Menu_Disp( int select ) {
  lcd.fillScreen(WHITE); //追加
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.setTextColor(MAGENTA);
  lcd.drawString( "Web接続", TITLE_X, TITLE_Y );

  Display_Battery_Level();

  Web_State_Disp(DeviceConnected);
  
  lcd.setTextSize(TEXT_SIZE);
  lcd.setTextColor(BLACK);
  switch (select) {
    case WEB_CONNECT_START:
      lcd.fillRoundRect( CON_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "接続開始", CON_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
      
    case WEB_CONNECT_RETURN:
      lcd.fillRoundRect( CON_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "接続開始", CON_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
  }
  
  return;
}

static void Web_State_Disp( boolean connection ) {
  lcd.setTextSize(CON_TEXT_SIZE);
  lcd.setTextColor(BLACK);

  if ( connection == NOT_CONNECTED ) {
    lcd.drawString( "接続状態: 未接続", CON_TEXT_X, CON_TEXT_Y );
  }
  else if ( connection == CONNECTED ) {
    lcd.drawString( "接続状態: 接続済み", CON_TEXT_X, CON_TEXT_Y );
  }
  
  return;
}

void Web_Connect( String MAC_ADRESS ) {
  lcd.fillScreen(WHITE);
  lcd.setTextSize(CONNECTING_TEXT_SIZE);
  
  if ( DeviceConnected == CONNECTED ) {
    return;
  }



  //lcd.drawString( "接続中・・・", CONNECTING_TEXT_X, CONNECTING_TEXT_Y );

  /* 実機側との接続を切っておく */
  BLEDevice::deinit();
  Connect_Reset();
  
  //lcd.println("BLE start.");
  lcd.drawString( "接続中・・・", CONNECTING_TEXT_X, CONNECTING_TEXT_Y );

  lcd.setTextSize(MACADDRESS_TEXT_SIZE);
  lcd.drawString( "address:" + MAC_ADRESS, MACADDRESS_TEXT_X, MACADDRESS_TEXT_Y );

  BLEDevice::init("m5-stack");
  
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  while ( DeviceConnected != true ) {
    M5.update();
  }

  return;
}

boolean Get_Web_State() {
  return DeviceConnected;
}

void Send_Web_Order( int num ) {

  switch (num) {
    case 0:
      pCharacteristic->setValue("a");
      //lcd.println("send:a");
      break;
    case 1:
      pCharacteristic->setValue("b");
      //lcd.println("send:b");
      break;
    case 2:
      pCharacteristic->setValue("c");
      //lcd.println("send:c");
      break;
    case 3:
      pCharacteristic->setValue("d");
      //lcd.println("send:d");
      break;
    case 4:
      pCharacteristic->setValue("e");
      //lcd.println("send:e");
      break;
    case 5:
      pCharacteristic->setValue("f");
      //lcd.println("send:f");
      break;
    case 6:
      pCharacteristic->setValue("g");
      //lcd.println("send:g");
      break;
    case 7:
      pCharacteristic->setValue("h");
      //lcd.println("send:h");
      break;
  }
  pCharacteristic->notify();

  return;
}
