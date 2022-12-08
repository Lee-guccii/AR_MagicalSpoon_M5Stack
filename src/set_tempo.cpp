#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "us.h"
#include "set_tempo.h"
#include "battery.h"


#define LGFX_M5STACK
#include <LovyanGFX.hpp>

extern LGFX lcd;

//ここから 座標などの定義

#define TITLE_TEXT_SIZE 2.5 //タイトルのサイズと座標
#define TITLE_X         10
#define TITLE_Y          5

#define TEXT_SIZE 2 //文字サイズ

#define BTN_Y   195 //仮想ボタンの座標や文字の座標
#define BTN_W   110
#define BTN_H    40
#define BTN_RAD  10
#define STR_Y   202

#define REG_BTN_X  40 //登録開始ボタン
#define REG_STR_X  48

#define RET_BTN_X 170 //戻るボタン
#define RET_STR_X 200

#define TEMPO_TEXT_SIZE 2.5 //現在のテンポ表示
#define TEMPO_X  50
#define TEMPO_Y 140

#define MIN_NOTES 0 //画面に表示する音符の数の下限、上限
#define MAX_NOTES 4

#define NOTES_INIT true      //音符の表示が初めてかを判断するフラグ
#define NOTES_NOT_INIT false 

#define USCHECK_SIZE 6 //超音波を認識した際の赤丸の大きさ
#define USCHECK_MIN  0
#define USCHECK_MAX  4

#define USMARK_X     20 //赤丸の座標と間隔
#define USMARK_Y     50
#define USMARK_SPACE 50

#define WAIT_TIME    50

#define LOWER_LIMIT  250 //登録テンポの下限と上限
#define UPPER_LIMIT 5000

#define NOTES_SQU_X      59  //音符の四角形部分の座標
#define NOTES_SQU_Y      70
#define NOTES_SQU_W       2
#define NOTES_SQU_H      45
#define NOTES_LINE_X     59  //音符の縦線部分の座標
#define NOTES_LINE_Y     70
#define NOTES_LINE_W      5
#define NOTES_LINE_H     10
#define NOTES_TRI_X1     64  //音符の三角形部分の座標
#define NOTES_TRI_Y1     70
#define NOTES_TRI_X2     64
#define NOTES_TRI_Y2     80
#define NOTES_TRI_X3     84
#define NOTES_TRI_Y3     80
#define NOTES_ROUND_X    38  //音符の楕円部分の座標
#define NOTES_ROUND_Y   105
#define NOTES_ROUND_W    22
#define NOTES_ROUND_H    16
#define NOTES_ROUND_RAD   8
#define NOTES_SPACE      65  //音符同士の間隔

//ここまで

static void Tempo_Notes(int);  //音符を表示する

static unsigned long Interval; //設定されているテンポ
static unsigned long Nowtime;  //時間記録用
static int Notes_Num;          //表示しているノーツの個数
static boolean Notes_Init;     //ノーツ初期化のフラグ

void Interval_Init() {
  Interval = INIT_INTERVAL;
  return;
}

void Tempo_Disp_Init() {
  Nowtime = millis();
  Notes_Num = MIN_NOTES;
  Notes_Init = NOTES_INIT;
  
  lcd.fillScreen(WHITE);
  char Tempo_Str[20];
  lcd.setTextColor(MAGENTA);
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.drawString( "テンポ登録", TITLE_X, TITLE_Y );
  lcd.setTextColor(BLACK);
  lcd.setTextSize(TEXT_SIZE);
  lcd.fillRoundRect( REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
  lcd.drawString( "登録開始", REG_STR_X, STR_Y );
  lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
  lcd.drawString( "戻る", RET_STR_X, STR_Y );

  lcd.setTextSize(TEMPO_TEXT_SIZE); //追記
  sprintf(Tempo_Str, "テンポ : %lums", Interval);
  lcd.drawString( Tempo_Str, TEMPO_X, TEMPO_Y );

  Display_Battery_Level();

  return;
}

static void Tempo_Notes(int num) {
  lcd.fillRect( NOTES_SQU_X + ( num * NOTES_SPACE ), NOTES_SQU_Y, NOTES_SQU_W, NOTES_SQU_H, BLACK );
  lcd.fillRect( NOTES_LINE_X + ( num * NOTES_SPACE ), NOTES_LINE_Y, NOTES_LINE_W, NOTES_LINE_H, BLACK );
  lcd.fillTriangle( NOTES_TRI_X1 + ( num * NOTES_SPACE ), NOTES_TRI_Y1, NOTES_TRI_X2 + ( num * NOTES_SPACE ), NOTES_TRI_Y2, NOTES_TRI_X3 + ( num * NOTES_SPACE ), NOTES_TRI_Y3, BLACK );
  lcd.fillRoundRect( NOTES_ROUND_X + ( num * NOTES_SPACE ), NOTES_ROUND_Y, NOTES_ROUND_W, NOTES_ROUND_H, NOTES_ROUND_RAD, BLACK );

  return;
}

void Now_Tempo() {
  if ( ( millis() - Nowtime ) >= Interval ) {
    Nowtime = millis();
    if ( Notes_Num == MAX_NOTES ) {
      lcd.fillRect( 0, 60, 280, 80, WHITE );
      Notes_Num = MIN_NOTES;
      Tempo_Notes(Notes_Num);
      Notes_Num++;
    }
    else {
      Tempo_Notes(Notes_Num);
      Notes_Num++;
    }
  }
  else if ( Notes_Init == NOTES_INIT ) {
    Tempo_Notes(Notes_Num);
    Nowtime = millis();
    Notes_Num++;
    Notes_Init = NOTES_NOT_INIT; 
  }

  return;
}

void Tempo_Btn_Disp(int select) {
  lcd.setTextSize(TEXT_SIZE);
  switch (select) {
    case REGIST_START:
      lcd.fillRoundRect( REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "登録開始", REG_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
    
    case TEMPO_RETURN:
      lcd.fillRoundRect( REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "登録開始", REG_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
  }
  return;
}

void Tempo_Regist() {
  int Settime;

  lcd.fillScreen(WHITE);
  lcd.setTextColor(MAGENTA);
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.drawString( "テンポ登録", TITLE_X, TITLE_Y );

  lcd.setTextSize(USCHECK_SIZE);
  lcd.setTextColor(RED);
  
  for ( int num = USCHECK_MIN; num < USCHECK_MAX; num++ ) {
    while ( Check_Hit() != HITSOUND ) ;
    
    if ( num == USCHECK_MIN ) {
      Settime = millis();
    }
    else if ( num == ( USCHECK_MAX - 1 ) ) {
      Settime = millis() - Settime;
    }

    lcd.drawString( "o", USMARK_X + ( num * USMARK_SPACE ), USMARK_Y );

    if ( num < USCHECK_MAX ) {
      delay(WAIT_TIME); //連続入力阻止用
      while ( Check_Hit() == HITSOUND ) ;
    }

  }
  Interval = Settime / 3; //平均値

  if ( Interval < LOWER_LIMIT ) {
    Interval = LOWER_LIMIT;
  }
  else if ( Interval > UPPER_LIMIT ) {
    Interval = UPPER_LIMIT;
  }
  
}

unsigned long Check_Tempo() {
  return Interval;
}
