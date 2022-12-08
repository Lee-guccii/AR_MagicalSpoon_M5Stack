#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include "firebase.h"
#include "set_wifi.h"
#include "battery.h"

#include <stdint.h>
#include <BLEDevice.h>

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

static void Ar_State_Disp(boolean);
static bool DeviceConnected = false;


static String MAC_ADDRESS;


// Wi-Fiに接続するために必要な情報　Wi-Fiは2.4GHzしか繋がらない
// 2階wifi
// #define WIFI_SSID "hko205G"    
// #define WIFI_PASSWORD "1qaz2wsx3edc4"
// 5階wifi
// #define WIFI_SSID "seclab509"    
// #define WIFI_PASSWORD "securitysecur"



// firebaseに接続するための情報 
#define FIREBASE_HOST "ar-magicalspoon-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "nW0lXpjt3v9bext0L8f9iLvDCP9L8EvftHL8NN02"

#define START_FIREBASE 100

FirebaseData firebaseData;
FirebaseJson json;


void Firebase_Connect(int ROOMNUMBER, String MAC_ADRESS) {

  // 初期化
  //M5.begin();
  lcd.fillScreen(WHITE);
  lcd.setTextSize(CONNECTING_TEXT_SIZE);
  lcd.drawString( "接続中・・・", CONNECTING_TEXT_X, CONNECTING_TEXT_Y );

  MAC_ADDRESS = Wifi_Connect();
  
  // connect firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Firebase.set(firebaseData, (String)("/room" + (String)ROOMNUMBER + "/" + MAC_ADDRESS + "/Cord"), START_FIREBASE);

  Serial.println("connect");
  lcd.setTextSize(MACADDRESS_TEXT_SIZE);
  lcd.drawString( "address:" + MAC_ADRESS, MACADDRESS_TEXT_X, MACADDRESS_TEXT_Y );

  do {


  }while((Firebase.get(firebaseData, (String)("/room" + (String)ROOMNUMBER + "/" + MAC_ADDRESS + "/Object/")) == 0));
  

    DeviceConnected = true;
  
  return;
}





// FirebaseにPUSHするための関数
void Send_Firebase(int SendData){
  // Firebaseに送信，".set"は書き換え
  Firebase.set(firebaseData, (String)("/room1/" + MAC_ADDRESS + "/Cord"), SendData); //コネクトした際にFireBaseに100を送る
  return;
}




//
void Ar_Menu_Disp( int select ) {
  lcd.fillScreen(WHITE); //追加
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.setTextColor(MAGENTA);
  lcd.drawString( "AR接続", TITLE_X, TITLE_Y );

  Display_Battery_Level();

  Ar_State_Disp(DeviceConnected);
  
  lcd.setTextSize(TEXT_SIZE);
  lcd.setTextColor(BLACK);
  switch (select) {
    case AR_CONNECT_START:
      lcd.fillRoundRect( CON_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "接続開始", CON_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
      
    case AR_CONNECT_RETURN:
      lcd.fillRoundRect( CON_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "接続開始", CON_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
  }
  
  return;
}


static void Ar_State_Disp( boolean connection ) {
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


boolean Get_Ar_State() {
  return DeviceConnected;
}



