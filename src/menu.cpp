#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>


#include "menu.h"
#include "battery.h"

#define LGFX_M5STACK
#include <LovyanGFX.hpp>

extern LGFX lcd;


//ここから 座標などの定義

#define TITLE_TEXT_SIZE 2.5 //タイトルの文字サイズ
#define TITLE_X_POS     10  //タイトルのX座標
#define TITLE_Y_POS      5  //タイトルのY座標

#define TEXT_SIZE 2 //メニューの文字サイズ

#define BTN_X_POS   80 //ボタンのX座標
#define BTN_Y_POS   50 //ボタンのY座標
#define BTN_SPACE   45 //ボタンの間隔_縦方向
#define BTN_WIDTH  160 //ボタンの横幅
#define BTN_HEIGHT  40 //ボタンの縦幅
#define BTN_RADIUS  10 //ボタンの角丸の半径

#define REG_X_POS   100 //"コード登録"のX,Y座標
#define REG_Y_POS    57
#define TEMPO_X_POS 100 //"テンポ登録"のX,Y座標
#define TEMPO_Y_POS 102
#define SEND_X_POS  100 //"コード送信"のX,Y座標
#define SEND_Y_POS  147
#define CON_X_POS   115 //"実機接続"のX,Y座標
#define CON_Y_POS   192

#define WEB_X_POS   140  //接続先選択画面でのX,Y座標
#define WEB_Y_POS    57
#define ROBO_X_POS  138
#define ROBO_Y_POS  102
#define AR_X_POS    145
#define AR_Y_POS    147
#define RET_X_POS   138
#define RET_Y_POS   192

#define ROOM_X_POS  130
#define ROOM1_X_POS 130  //ROOM選択先選択画面でのX,Y座標
#define ROOM1_Y_POS  57
#define ROOM2_X_POS 130
#define ROOM2_Y_POS 102
#define ROOM3_X_POS 130
#define ROOM3_Y_POS 147

//ここまで


void Menu_Text() {
  lcd.setTextColor(MAGENTA);
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.drawString("メニュー", TITLE_X_POS, TITLE_Y_POS);

  Display_Battery_Level();

  return;
}


void Select_Menu(int select) {
  lcd.setTextColor(BLACK);
  lcd.setTextSize(TEXT_SIZE);

  switch(select) {
    case SET_CODE:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  RED);
      lcd.drawString("コード登録", REG_X_POS, REG_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("テンポ登録", TEMPO_X_POS, TEMPO_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("コード送信", SEND_X_POS, SEND_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("外部接続", CON_X_POS, CON_Y_POS);
      
      break;
      
    case SET_TEMPO:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("コード登録", REG_X_POS, REG_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("テンポ登録", TEMPO_X_POS, TEMPO_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("コード送信", SEND_X_POS, SEND_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("外部接続", CON_X_POS, CON_Y_POS);
      break;
      
    case SEND_CODE:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("コード登録", REG_X_POS, REG_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("テンポ登録", TEMPO_X_POS, TEMPO_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("コード送信", SEND_X_POS, SEND_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("外部接続", CON_X_POS, CON_Y_POS);
      break;

    case CONNECT:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("コード登録", REG_X_POS, REG_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("テンポ登録", TEMPO_X_POS, TEMPO_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("コード送信", SEND_X_POS, SEND_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("外部接続", CON_X_POS, CON_Y_POS);
      break;
  }
  
  return;
}

void Outside_Text() {
  lcd.setTextColor(MAGENTA);
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.drawString("外部接続", TITLE_X_POS, TITLE_Y_POS);

  Display_Battery_Level();

  return;
}

void Outside_Disp( int select ) {
  lcd.setTextColor(BLACK);
  lcd.setTextSize(TEXT_SIZE);
  
  switch(select) {
    case WEB:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  RED);
      lcd.drawString("WEB", WEB_X_POS, WEB_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("実機", ROBO_X_POS, ROBO_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("AR", AR_X_POS, AR_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
      
    case ROBOT:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("WEB", WEB_X_POS, WEB_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("実機", ROBO_X_POS, ROBO_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("AR", AR_X_POS, AR_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;

    case AR:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("WEB", WEB_X_POS, WEB_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("実機", ROBO_X_POS, ROBO_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("AR", AR_X_POS, AR_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
      
    case OUT_RETURN:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("WEB", WEB_X_POS, WEB_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("実機", ROBO_X_POS, ROBO_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("AR", AR_X_POS, AR_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
  
  }
  return;
}


void Ar_Room_Text() {
  lcd.setTextColor(MAGENTA);
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.drawString("Room選択", TITLE_X_POS, TITLE_Y_POS);

  Display_Battery_Level();

  return;
}


void Ar_Room_Disp( int select ) {
  lcd.setTextColor(BLACK);
  lcd.setTextSize(TEXT_SIZE);
  
  switch(select) {
    case ROOM1:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  RED);
      lcd.drawString("Room1", ROOM1_X_POS, ROOM1_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("Room2", ROOM2_X_POS, ROOM2_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("Room3", ROOM3_X_POS, ROOM3_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
      
    case ROOM2:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("Room1", ROOM1_X_POS, ROOM1_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("Room2", ROOM2_X_POS, ROOM2_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("Room3", ROOM3_X_POS, ROOM3_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;

    case ROOM3:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("Room1", ROOM1_X_POS, ROOM1_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("Room2", ROOM2_X_POS, ROOM2_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("Room3", ROOM3_X_POS, ROOM3_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
      
    case ROOM_RETURN:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("Room1", ROOM1_X_POS, ROOM1_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("Room2", ROOM2_X_POS, ROOM2_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 2 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("Room3", ROOM3_X_POS, ROOM3_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
  
  }
  return;
}


