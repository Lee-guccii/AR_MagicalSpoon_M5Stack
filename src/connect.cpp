#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "connect.h"
#include "battery.h"

#define LGFX_M5STACK
#include <LovyanGFX.hpp>

extern LGFX lcd;



//指令コマンド一覧_偶数パリティ
#define UP       (uint8_t)( 'a' + 0x80 ) //0x61 + 0x80 -> 01100001 + 10000000
#define DOWN     (uint8_t)( 'b' + 0x80 ) //0x62 + 0x80 -> 01100010 + 10000000
#define RIGHT    (uint8_t)( 'c' + 0x00 ) //0x63 + 0x00 -> 01100011 + 00000000
#define LEFT     (uint8_t)( 'd' + 0x80 ) //0x64 + 0x80 -> 01100100 + 10000000
#define RIGHT_UP (uint8_t)( 'e' + 0x00 ) //0x65 + 0x00 -> 01100101 + 00000000
#define LEFT_UP  (uint8_t)( 'f' + 0x00 ) //0x66 + 0x00 -> 01100110 + 00000000
#define STOP     (uint8_t)( 'g' + 0x80 ) //0x67 + 0x80 -> 01100111 + 10000000
#define LED      (uint8_t)( 'h' + 0x80 ) //0x68 + 0x80 -> 01101000 + 10000000

//肯定応答と否定応答
#define ACK (uint8_t)(0x06) //00000110
#define NAK (uint8_t)(0x15) //00010101


//ここから 座標などの定義

#define WAIT_TIME 5000 //5秒

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

#define FAILED_TEXT_SIZE 2 //接続失敗時の文字サイズ

#define RECEIVE_ACK  0 //ACKを受け取った
#define RECEIVE_NAK  1 //NAKを受け取った
#define TEN_SECONDS 10 //10秒経過した

//ここまで

static boolean Connection;     //接続状態の判別用

//static void Connect_State_Disp(boolean); //接続状態を表示


//microbitのアドレス
#define MYSERVER_ADDRESS "ca:8f:a0:f1:1c:42" //No.1 V1
//#define MYSERVER_ADDRESS "c8:8a:f9:f9:fa:94" //No.2 V1
//#define MYSERVER_ADDRESS "f0:bf:1b:cb:2f:4b" //No.3 V1
//#define MYSERVER_ADDRESS "f3:e3:71:58:b3:d4" //No.4 V1

//#define MYSERVER_ADDRESS "ec:5e:8d:dd:e9:b1" //No.1 V2

static BLEUUID svcUUID("19b10010-e8f2-537e-4f6c-d104768a1214");
static BLEUUID chUUID("19b10011-e8f2-537e-4f6c-d104768a1214");

static BLEAddress *ServerAddress;
static BLERemoteCharacteristic *RemoteCharacteristic;
static BLEClient *Client;

static uint8_t Response; //受け取った応答を記録する
static unsigned long NowTime; //指令を送信した時間

void Connection_Init() {
  if ( Connection == CONNECTED ) {
    return;
  }
  BLEDevice::deinit();
  
  Connection = NOT_CONNECTED;
  BLEDevice::init("");
  ServerAddress = new BLEAddress(MYSERVER_ADDRESS);
  
  return;
}

void Connect_Menu_Disp( int select ) {
  lcd.fillScreen(WHITE); //追加
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.setTextColor(MAGENTA);
  lcd.drawString( "実機接続", TITLE_X, TITLE_Y );

  Display_Battery_Level();

  Connect_State_Disp(Connection);
  
  lcd.setTextSize(TEXT_SIZE);
  lcd.setTextColor(BLACK);
  switch (select) {
    case CONNECT_START:
      lcd.fillRoundRect( CON_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "接続開始", CON_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
      
    case CONNECT_RETURN:
      lcd.fillRoundRect( CON_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "接続開始", CON_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
  }

  return;
}

void Connect_State_Disp( boolean connection ) {
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

void Connect_Reset() {
  Connection = NOT_CONNECTED;
}

boolean Connect_Start() {
  lcd.fillScreen(WHITE);
  lcd.setTextSize(CONNECTING_TEXT_SIZE);
  if ( Connection == CONNECTED ) {
    return CONNECT_FAILED;
  }

  lcd.drawString( "接続中・・・", CONNECTING_TEXT_X, CONNECTING_TEXT_Y );

  lcd.setTextSize(FAILED_TEXT_SIZE);

  /* microbitと接続 */
  //BLEClient* Client = BLEDevice::createClient();
  Client = BLEDevice::createClient();
  Client->connect(*ServerAddress, BLE_ADDR_TYPE_RANDOM);
  if ( Client->isConnected() == false ) {
    lcd.fillScreen(WHITE);
    lcd.print("サーバーとの接続に失敗しました...");//print毎に位置がずれるので後で修正, drawString()にする
    delay(WAIT_TIME);
    return CONNECT_FAILED;
  }

  BLERemoteService* RemoteService = Client->getService(svcUUID);
  if( RemoteService == nullptr ){
    lcd.fillScreen(WHITE);
    lcd.print("service UUID の入手に失敗しました...");
    delay(WAIT_TIME);
    Client->disconnect();
    return CONNECT_FAILED;
  }

  RemoteCharacteristic = RemoteService->getCharacteristic(chUUID);
  if( RemoteCharacteristic == nullptr ){
    lcd.fillScreen(WHITE);
    lcd.print("characteristic UUID の入手に失敗しました...");
    Client->disconnect();
    delay(WAIT_TIME);
    return CONNECT_FAILED;
  }
  
  Connection = CONNECTED;
  return CONNECT_SUCCESS;
  
}


int Send_Order_Num(int num) {
  switch (num) {
    case 0:
      if ( RemoteCharacteristic->canWrite()) {
        RemoteCharacteristic->writeValue( UP, false );
        //RemoteCharacteristic->writeValue( (uint8_t)('a'), false );
      }
      break;
    case 1:
      if ( RemoteCharacteristic->canWrite()) {
        RemoteCharacteristic->writeValue( DOWN, false );
      }
      break;
    case 2:
      if ( RemoteCharacteristic->canWrite()) {
        RemoteCharacteristic->writeValue( RIGHT, false );
      }
      break;
    case 3:
      if ( RemoteCharacteristic->canWrite()) {
        RemoteCharacteristic->writeValue( LEFT, false );
      }
      break;
    case 4:
      if ( RemoteCharacteristic->canWrite()) {
        RemoteCharacteristic->writeValue( RIGHT_UP, false );
      }
      break;
    case 5:
      if ( RemoteCharacteristic->canWrite()) {
        RemoteCharacteristic->writeValue( LEFT_UP, false );
      }
      break;
    case 6:
      if ( RemoteCharacteristic->canWrite()) {
        RemoteCharacteristic->writeValue( STOP, false );
      }
      break;
    case 7:
      if ( RemoteCharacteristic->canWrite()) {
        RemoteCharacteristic->writeValue( LED, false );
      }
      break;
  }

  NowTime = millis();
  delay(2500);

  //応答を読み取る
  if ( RemoteCharacteristic->canRead() ) {
    Response = RemoteCharacteristic->readUInt8();
  }

  //NowTime = millis();

  while ( ( Response != ACK ) && ( Response != NAK ) ) {
    if ( ( millis() - NowTime ) >= 10000 ) {
      return TEN_SECONDS;
    }
    
    if ( RemoteCharacteristic->canRead() ) {
      Response = RemoteCharacteristic->readUInt8();
    }
  }

  if ( Response == ACK ) {
    return RECEIVE_ACK;
  }
  else if ( Response == NAK ) {
    return RECEIVE_NAK;
  }

}

boolean Get_Connect_State() {
  return Connection;
}
