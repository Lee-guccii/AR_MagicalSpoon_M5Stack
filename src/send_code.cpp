#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "us.h"
#include "set_code.h"
#include "send_code.h"
#include "connect.h"
#include "web_blue.h"
#include "firebase.h"
#include "microSD.h"
#include "set_wifi.h"

#define LGFX_M5STACK
#include <LovyanGFX.hpp>

extern LGFX lcd;


//ここから 座標などの定義

#define TITLE_TEXT_SIZE 2.5 //タイトルの座標など
#define TITLE_X         10
#define TITLE_Y          5

#define BTN_TEXT_SIZE   2 //ボタンの座標など
#define BTN_X          70
#define BTN_Y         195
#define BTN_W         110
#define BTN_H          40
#define BTN_RAD        10
#define RET_TEXT_X    100
#define RET_TEXT_Y    202

#define TEXT_SIZE  1.9 //その他テキスト座標など
#define TEXT_X     10
#define SPOON_Y    50
#define TEMPO_Y   160

#define GAUGE_BOX_X     35 //ゲージの座標と大きさ
#define GAUGE_BOX_Y     90
#define GAUGE_BOX_SIZE  40
#define GAUGE_BOX_SPACE 40

#define CODE_TEXT_SIZE 1.7 //コード表の文字サイズ

#define BORDERLINE_X1 235 //縦線の座標
#define BORDERLINE_X2 235
#define BORDERLINE_Y1   0
#define BORDERLINE_Y2 240

#define CODE_TEXT_X 250 //"コード"の座標
#define CODE_TEXT_Y  10

#define COLON_X 265 //コロンとコードの座標
#define COLON_Y  35
#define CODE_X  275
#define CODE_Y   35

#define UP_LINE1_X1 250 //上矢印の座標
#define UP_LINE1_Y1  38
#define UP_LINE1_X2 250
#define UP_LINE1_Y2  54
#define UP_LINE2_X1 250
#define UP_LINE2_Y1  38
#define UP_LINE2_X2 245
#define UP_LINE2_Y2  45
#define UP_LINE3_X1 250
#define UP_LINE3_Y1  38
#define UP_LINE3_X2 255
#define UP_LINE3_Y2  45

#define DW_LINE1_X1 250 //下矢印の座標
#define DW_LINE1_Y1  38
#define DW_LINE1_X2 250
#define DW_LINE1_Y2  54
#define DW_LINE2_X1 245
#define DW_LINE2_Y1  47
#define DW_LINE2_X2 250
#define DW_LINE2_Y2  54
#define DW_LINE3_X1 255
#define DW_LINE3_Y1  47
#define DW_LINE3_X2 250
#define DW_LINE3_Y2  54

#define RT_LINE1_X1 242 //右矢印の座標
#define RT_LINE1_Y1  46
#define RT_LINE1_X2 258
#define RT_LINE1_Y2  46
#define RT_LINE2_X1 251
#define RT_LINE2_Y1  41
#define RT_LINE2_X2 258
#define RT_LINE2_Y2  46
#define RT_LINE3_X1 251
#define RT_LINE3_Y1  51
#define RT_LINE3_X2 258
#define RT_LINE3_Y2  46

#define LF_LINE1_X1 242 //左矢印の座標
#define LF_LINE1_Y1  46
#define LF_LINE1_X2 258
#define LF_LINE1_Y2  46
#define LF_LINE2_X1 242
#define LF_LINE2_Y1  46
#define LF_LINE2_X2 249
#define LF_LINE2_Y2  41
#define LF_LINE3_X1 242
#define LF_LINE3_Y1  46
#define LF_LINE3_X2 249
#define LF_LINE3_Y2  51

#define RU_LINE1_X1 256 //右上矢印の座標
#define RU_LINE1_Y1  40
#define RU_LINE1_X2 244
#define RU_LINE1_Y2  52
#define RU_LINE2_X1 256
#define RU_LINE2_Y1  40
#define RU_LINE2_X2 247
#define RU_LINE2_Y2  41
#define RU_LINE3_X1 256
#define RU_LINE3_Y1  40
#define RU_LINE3_X2 255
#define RU_LINE3_Y2  49

#define LU_LINE1_X1 244 //左上矢印の座標
#define LU_LINE1_Y1  40
#define LU_LINE1_X2 256
#define LU_LINE1_Y2  52
#define LU_LINE2_X1 244
#define LU_LINE2_Y1  40
#define LU_LINE2_X2 253
#define LU_LINE2_Y2  41
#define LU_LINE3_X1 244
#define LU_LINE3_Y1  40
#define LU_LINE3_X2 245
#define LU_LINE3_Y2  49

#define STOP_X    244 //停止マークの座標
#define STOP_Y     40
#define STOP_SIZE  12

#define LED_LINE1_X1 245 //LEDマークの座標
#define LED_LINE1_Y1  38
#define LED_LINE1_X2 245
#define LED_LINE1_Y2  54
#define LED_LINE2_X1 245
#define LED_LINE2_Y1  54
#define LED_LINE2_X2 255
#define LED_LINE2_Y2  54

#define Y_SPACE 25 //コード同士の間隔

#define INPUT_TEXT_SIZE 3 //入力のON, OFFを表す数字のサイズ

#define STARTBIT_X 35 //スタートビットの表示座標
#define STARTBIT_Y 55

#define INPUT_TEXT_X     35 //入力(0 or 1)を表示する際の座標
#define INPUT_TEXT_Y     55
#define INPUT_TEXT_SPACE 40

#define INPUT_ON  1 //入力がある時は1, ない時は0
#define INPUT_OFF 0

#define RESET_DISP_X  10
#define RESET_DISP_Y  50
#define RESET_DISP_W 220
#define RESET_DISP_H 230

#define NOTES_SQU_X     200  //音符の四角形部分の座標
#define NOTES_SQU_Y     145
#define NOTES_SQU_W       2
#define NOTES_SQU_H      35
#define NOTES_LINE_X    200  //音符の縦線部分の座標
#define NOTES_LINE_Y    145
#define NOTES_LINE_W      4
#define NOTES_LINE_H      8
#define NOTES_TRI_X1    204  //音符の三角形部分の座標
#define NOTES_TRI_Y1    145
#define NOTES_TRI_X2    204
#define NOTES_TRI_Y2    153
#define NOTES_TRI_X3    216
#define NOTES_TRI_Y3    153
#define NOTES_ROUND_X   183  //音符の楕円部分の座標
#define NOTES_ROUND_Y   172
#define NOTES_ROUND_W    18
#define NOTES_ROUND_H    13
#define NOTES_ROUND_RAD   7


//指令送信時に表示する記号の座標
#define SEND_UP_LINE1_X1 160 //上矢印の座標
#define SEND_UP_LINE1_Y1 160
#define SEND_UP_LINE1_X2 160
#define SEND_UP_LINE1_Y2 200
#define SEND_UP_LINE2_X1 160
#define SEND_UP_LINE2_Y1 160
#define SEND_UP_LINE2_X2 148
#define SEND_UP_LINE2_Y2 177
#define SEND_UP_LINE3_X1 160
#define SEND_UP_LINE3_Y1 160
#define SEND_UP_LINE3_X2 172
#define SEND_UP_LINE3_Y2 177

#define SEND_DW_LINE1_X1 160 //下矢印の座標
#define SEND_DW_LINE1_Y1 160
#define SEND_DW_LINE1_X2 160
#define SEND_DW_LINE1_Y2 200
#define SEND_DW_LINE2_X1 160
#define SEND_DW_LINE2_Y1 200
#define SEND_DW_LINE2_X2 148
#define SEND_DW_LINE2_Y2 183
#define SEND_DW_LINE3_X1 160
#define SEND_DW_LINE3_Y1 200
#define SEND_DW_LINE3_X2 172
#define SEND_DW_LINE3_Y2 183

#define SEND_RT_LINE1_X1 140 //右矢印の座標
#define SEND_RT_LINE1_Y1 180
#define SEND_RT_LINE1_X2 180
#define SEND_RT_LINE1_Y2 180
#define SEND_RT_LINE2_X1 180
#define SEND_RT_LINE2_Y1 180
#define SEND_RT_LINE2_X2 163
#define SEND_RT_LINE2_Y2 168
#define SEND_RT_LINE3_X1 180
#define SEND_RT_LINE3_Y1 180
#define SEND_RT_LINE3_X2 163
#define SEND_RT_LINE3_Y2 192

#define SEND_LF_LINE1_X1 140 //左矢印の座標
#define SEND_LF_LINE1_Y1 180
#define SEND_LF_LINE1_X2 180
#define SEND_LF_LINE1_Y2 180
#define SEND_LF_LINE2_X1 140
#define SEND_LF_LINE2_Y1 180
#define SEND_LF_LINE2_X2 157
#define SEND_LF_LINE2_Y2 168
#define SEND_LF_LINE3_X1 140
#define SEND_LF_LINE3_Y1 180
#define SEND_LF_LINE3_X2 157
#define SEND_LF_LINE3_Y2 192

#define SEND_RU_LINE1_X1 180 //右上矢印の座標
#define SEND_RU_LINE1_Y1 160
#define SEND_RU_LINE1_X2 140
#define SEND_RU_LINE1_Y2 200
#define SEND_RU_LINE2_X1 180
#define SEND_RU_LINE2_Y1 160
#define SEND_RU_LINE2_X2 150
#define SEND_RU_LINE2_Y2 163
#define SEND_RU_LINE3_X1 180
#define SEND_RU_LINE3_Y1 160
#define SEND_RU_LINE3_X2 177
#define SEND_RU_LINE3_Y2 190

#define SEND_LU_LINE1_X1 140 //左上矢印の座標
#define SEND_LU_LINE1_Y1 160
#define SEND_LU_LINE1_X2 180
#define SEND_LU_LINE1_Y2 200
#define SEND_LU_LINE2_X1 140
#define SEND_LU_LINE2_Y1 160
#define SEND_LU_LINE2_X2 170
#define SEND_LU_LINE2_Y2 163
#define SEND_LU_LINE3_X1 140
#define SEND_LU_LINE3_Y1 160
#define SEND_LU_LINE3_X2 143
#define SEND_LU_LINE3_Y2 190

#define SEND_STOP_X    150 //停止マークの座標
#define SEND_STOP_Y    160
#define SEND_STOP_SIZE  40

#define SEND_LED_LINE1_X1 150 //LEDマークの座標
#define SEND_LED_LINE1_Y1 160
#define SEND_LED_LINE1_X2 150
#define SEND_LED_LINE1_Y2 200
#define SEND_LED_LINE2_X1 150
#define SEND_LED_LINE2_Y1 200
#define SEND_LED_LINE2_X2 170
#define SEND_LED_LINE2_Y2 200

#define SELECT_OBJECT_COLOR_X 30 //AR用　オブジェクトの色を表示
#define SELECT_OBJECT_COLOR_Y 215
#define SELECT_OBJECT_COLOR_R 15

//ここまで

#define RECEIVE_ACK  0
#define RECEIVE_NAK  1
#define TEN_SECONDS 10

#define ORDER_MIN 0 //指令の番号
#define ORDER_MAX 7

//
#define MATCH     true  //入力と登録指令の比較用
#define UNMATCH   false
#define NOT_FOUND 10

static unsigned long SettingTime;
static unsigned long PrevTime;
static int Hit;
static boolean NotesFlag;
//

static int CodeVal;              //入力を10進数で記録
static int MatchNum = NOT_FOUND; //送信処理で用いる指令番号の初期化

static int SendResult;
static int ErrorCount; //パリティエラーの回数


static void Send_Disp_Notes(void); //送信画面で音符を表示
static void Send_Order(void);  //指令送信
static void Disp_Order(int);   //送信した指令を表示


void Set_Tempo(unsigned long tempo) {
  SettingTime = tempo;
  return;
}

void Send_Disp_Init() {
  PrevTime = millis();
  NotesFlag = true;
  String StrCode;
  char Tempo_Str[20];
  lcd.setTextColor(MAGENTA);
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.drawString( "コード送信", TITLE_X, TITLE_Y );
  
  lcd.setTextColor(BLACK);
  lcd.setTextSize(BTN_TEXT_SIZE);
  lcd.fillRoundRect( BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, RED );
  lcd.drawString( "戻る", RET_TEXT_X, RET_TEXT_Y );

  // if( Get_Ar_State() == true ){ //ARの時はオブジェクトの色を表示
  //   int ObjectColor = Get_Select_Object_Color();

  //   lcd.fillCircle(SELECT_OBJECT_COLOR_X, SELECT_OBJECT_COLOR_Y, SELECT_OBJECT_COLOR_R, ObjectColor);
  //   lcd.drawCircle(SELECT_OBJECT_COLOR_X, SELECT_OBJECT_COLOR_Y, SELECT_OBJECT_COLOR_R, BLACK);

  // }
  
  lcd.setTextColor(BLACK);
  lcd.setTextSize(TEXT_SIZE);
  lcd.drawString( "スプーンを打ってね", TEXT_X, SPOON_Y );
  sprintf(Tempo_Str, "テンポ : %lums", SettingTime);
  lcd.drawString( Tempo_Str, TEXT_X, TEMPO_Y );

  lcd.drawRect( GAUGE_BOX_X, GAUGE_BOX_Y, GAUGE_BOX_SIZE, GAUGE_BOX_SIZE, ORANGE );
  lcd.drawRect( GAUGE_BOX_X + ( GAUGE_BOX_SPACE * 1 ), GAUGE_BOX_Y, GAUGE_BOX_SIZE, GAUGE_BOX_SIZE, YELLOW );
  lcd.drawRect( GAUGE_BOX_X + ( GAUGE_BOX_SPACE * 2 ), GAUGE_BOX_Y, GAUGE_BOX_SIZE, GAUGE_BOX_SIZE, YELLOW );
  lcd.drawRect( GAUGE_BOX_X + ( GAUGE_BOX_SPACE * 3 ), GAUGE_BOX_Y, GAUGE_BOX_SIZE, GAUGE_BOX_SIZE, YELLOW );
  
  lcd.setTextSize(CODE_TEXT_SIZE);
  lcd.drawLine( BORDERLINE_X1, BORDERLINE_Y1, BORDERLINE_X2, BORDERLINE_Y2, BLACK );
  lcd.drawString( "コード", CODE_TEXT_X, CODE_TEXT_Y );
  
  for ( int i = ORDER_MIN; i <= ORDER_MAX; i++ ) {
    StrCode = String( Pass_CodeVal(i), BIN );
    while ( StrCode.length() < 3 ) {
      StrCode = 0 + StrCode;
    }
    StrCode = 1 + StrCode;
    lcd.drawString(     ":", COLON_X, COLON_Y + ( i * Y_SPACE ) );
    lcd.drawString( StrCode,  CODE_X,  CODE_Y + ( i * Y_SPACE ) );
  }

  //上矢印
  lcd.drawLine( UP_LINE1_X1, UP_LINE1_Y1, UP_LINE1_X2, UP_LINE1_Y2, BLUE );
  lcd.drawLine( UP_LINE2_X1, UP_LINE2_Y1, UP_LINE2_X2, UP_LINE2_Y2, BLUE );
  lcd.drawLine( UP_LINE3_X1, UP_LINE3_Y1, UP_LINE3_X2, UP_LINE3_Y2, BLUE );

  //下矢印
  lcd.drawLine( DW_LINE1_X1, DW_LINE1_Y1 + Y_SPACE, DW_LINE1_X2, DW_LINE1_Y2 + Y_SPACE, BLUE );
  lcd.drawLine( DW_LINE2_X1, DW_LINE2_Y1 + Y_SPACE, DW_LINE2_X2, DW_LINE2_Y2 + Y_SPACE, BLUE );
  lcd.drawLine( DW_LINE3_X1, DW_LINE3_Y1 + Y_SPACE, DW_LINE3_X2, DW_LINE3_Y2 + Y_SPACE, BLUE );

  //右矢印
  lcd.drawLine( RT_LINE1_X1, RT_LINE1_Y1 + ( Y_SPACE * 2 ), RT_LINE1_X2, RT_LINE1_Y2 + ( Y_SPACE * 2 ), BLUE );
  lcd.drawLine( RT_LINE2_X1, RT_LINE2_Y1 + ( Y_SPACE * 2 ), RT_LINE2_X2, RT_LINE2_Y2 + ( Y_SPACE * 2 ), BLUE );
  lcd.drawLine( RT_LINE3_X1, RT_LINE3_Y1 + ( Y_SPACE * 2 ), RT_LINE3_X2, RT_LINE3_Y2 + ( Y_SPACE * 2 ), BLUE );

  //左矢印
  lcd.drawLine( LF_LINE1_X1, LF_LINE1_Y1 + ( Y_SPACE * 3 ), LF_LINE1_X2, LF_LINE1_Y2 + ( Y_SPACE * 3 ), BLUE );
  lcd.drawLine( LF_LINE2_X1, LF_LINE2_Y1 + ( Y_SPACE * 3 ), LF_LINE2_X2, LF_LINE2_Y2 + ( Y_SPACE * 3 ), BLUE );
  lcd.drawLine( LF_LINE3_X1, LF_LINE3_Y1 + ( Y_SPACE * 3 ), LF_LINE3_X2, LF_LINE3_Y2 + ( Y_SPACE * 3 ), BLUE );

  //右上矢印
  lcd.drawLine( RU_LINE1_X1, RU_LINE1_Y1 + ( Y_SPACE * 4 ), RU_LINE1_X2, RU_LINE1_Y2 + ( Y_SPACE * 4 ), BLUE );
  lcd.drawLine( RU_LINE2_X1, RU_LINE2_Y1 + ( Y_SPACE * 4 ), RU_LINE2_X2, RU_LINE2_Y2 + ( Y_SPACE * 4 ), BLUE );
  lcd.drawLine( RU_LINE3_X1, RU_LINE3_Y1 + ( Y_SPACE * 4 ), RU_LINE3_X2, RU_LINE3_Y2 + ( Y_SPACE * 4 ), BLUE );

  //左上矢印
  lcd.drawLine( LU_LINE1_X1, LU_LINE1_Y1 + ( Y_SPACE * 5 ), LU_LINE1_X2, LU_LINE1_Y2 + ( Y_SPACE * 5 ), BLUE );
  lcd.drawLine( LU_LINE2_X1, LU_LINE2_Y1 + ( Y_SPACE * 5 ), LU_LINE2_X2, LU_LINE2_Y2 + ( Y_SPACE * 5 ), BLUE );
  lcd.drawLine( LU_LINE3_X1, LU_LINE3_Y1 + ( Y_SPACE * 5 ), LU_LINE3_X2, LU_LINE3_Y2 + ( Y_SPACE * 5 ), BLUE );

  //停止
  lcd.fillRect( STOP_X, STOP_Y + ( Y_SPACE * 6 ), STOP_SIZE, STOP_SIZE, BLUE );

  //LED
  lcd.drawLine( LED_LINE1_X1, LED_LINE1_Y1 + ( Y_SPACE * 7 ), LED_LINE1_X2, LED_LINE1_Y2 + ( Y_SPACE * 7 ), BLUE );
  lcd.drawLine( LED_LINE2_X1, LED_LINE2_Y1 + ( Y_SPACE * 7 ), LED_LINE2_X2, LED_LINE2_Y2 + ( Y_SPACE * 7 ), BLUE );
  
  return;
}

static void Send_Disp_Notes() {
  lcd.fillRect( NOTES_SQU_X, NOTES_SQU_Y, NOTES_SQU_W, NOTES_SQU_H, BLACK );
  lcd.fillRect( NOTES_LINE_X, NOTES_LINE_Y, NOTES_LINE_W, NOTES_LINE_H, BLACK );
  lcd.fillTriangle( NOTES_TRI_X1, NOTES_TRI_Y1, NOTES_TRI_X2, NOTES_TRI_Y2, NOTES_TRI_X3, NOTES_TRI_Y3, BLACK );
  lcd.fillRoundRect( NOTES_ROUND_X, NOTES_ROUND_Y, NOTES_ROUND_W, NOTES_ROUND_H, NOTES_ROUND_RAD, BLACK );

  return;
}

void Notes_Disp() {
  if ( ( millis() - PrevTime ) >= ( SettingTime / 2 ) ) {
    if ( NotesFlag == true ) {
      //lcd.fillRect( 190, 150, 30, 30, GREEN );
      Send_Disp_Notes();
      NotesFlag = false;
    }
    else if ( NotesFlag == false ) {
      //lcd.fillRect( 190, 150, 30, 30, WHITE );
      lcd.fillRect( 180, 140, 50, 50, WHITE );
      NotesFlag = true;
    }
    PrevTime = millis();
  }
}

void Send_Disp() {
  char Tempo_Str[20];
  lcd.setTextColor(BLACK);
  lcd.setTextSize(INPUT_TEXT_SIZE);
  lcd.fillRect( GAUGE_BOX_X, GAUGE_BOX_Y, GAUGE_BOX_SIZE, GAUGE_BOX_SIZE, RED );
  lcd.drawString( "1", STARTBIT_X, STARTBIT_Y );
  lcd.drawRect( GAUGE_BOX_X + ( GAUGE_BOX_SPACE * 1 ), GAUGE_BOX_Y, GAUGE_BOX_SIZE, GAUGE_BOX_SIZE, YELLOW );
  lcd.drawRect( GAUGE_BOX_X + ( GAUGE_BOX_SPACE * 2 ), GAUGE_BOX_Y, GAUGE_BOX_SIZE, GAUGE_BOX_SIZE, YELLOW );
  lcd.drawRect( GAUGE_BOX_X + ( GAUGE_BOX_SPACE * 3 ), GAUGE_BOX_Y, GAUGE_BOX_SIZE, GAUGE_BOX_SIZE, YELLOW );

  int code[3];
  /*
  lcd.setTextSize(TEXT_SIZE);
  sprintf(Tempo_Str, "テンポ : %lums", SettingTime);
  lcd.drawString( Tempo_Str, TEXT_X, TEMPO_Y );
  */
  lcd.setTextSize(2.5);
  lcd.drawString( "指令 : ", 30, 170 );
  
  lcd.setTextSize(INPUT_TEXT_SIZE);

  Hit = HIT_OFF;

  delay(SettingTime);

  for ( int i = BIT_2ND; i <= BIT_4TH; i++ ) {
    PrevTime = millis();
    code[i-1] = INPUT_OFF;
    
    while ( ( millis() - PrevTime ) < SettingTime ) {
      //M5.update();//仮動作用
      if ( Hit == HIT_ON ) {
        lcd.fillRect( GAUGE_BOX_X + ( GAUGE_BOX_SPACE * i ), GAUGE_BOX_Y, GAUGE_BOX_SIZE * ( millis() - PrevTime ) / SettingTime, GAUGE_BOX_SIZE,  RED );
      }
      else if ( Hit == HIT_OFF ) {
        lcd.fillRect( GAUGE_BOX_X + ( GAUGE_BOX_SPACE * i ), GAUGE_BOX_Y, GAUGE_BOX_SIZE * ( millis() - PrevTime ) / SettingTime, GAUGE_BOX_SIZE, PINK );
      }
      
      if ( Check_Hit() == HITSOUND ) {
        Hit = HIT_ON;
        lcd.drawString( "1", INPUT_TEXT_X + ( INPUT_TEXT_SPACE * i ), INPUT_TEXT_Y );
      }
    }
    if ( Hit == HIT_ON ) {
      code[i-1] = INPUT_ON;
    }
    else if ( Hit == HIT_OFF ) {
      lcd.drawString( "0", INPUT_TEXT_X + ( INPUT_TEXT_SPACE * i ), INPUT_TEXT_Y );
    }
    Hit = HIT_OFF;
  }
  //
  CodeVal = 4*code[0] + 2*code[1] + code[2];
  Send_Order();
  return;
}

void Reset_Disp() {
  lcd.fillRect( RESET_DISP_X, RESET_DISP_Y, RESET_DISP_W, RESET_DISP_H, WHITE );
  return;
}

static void Disp_Order(int num) {
  switch(num) {
    case 0:
      lcd.drawLine( SEND_UP_LINE1_X1, SEND_UP_LINE1_Y1, SEND_UP_LINE1_X2, SEND_UP_LINE1_Y2, BLACK );
      lcd.drawLine( SEND_UP_LINE2_X1, SEND_UP_LINE2_Y1, SEND_UP_LINE2_X2, SEND_UP_LINE2_Y2, BLACK );
      lcd.drawLine( SEND_UP_LINE3_X1, SEND_UP_LINE3_Y1, SEND_UP_LINE3_X2, SEND_UP_LINE3_Y2, BLACK );
      break;
    case 1:
      lcd.drawLine( SEND_DW_LINE1_X1, SEND_DW_LINE1_Y1, SEND_DW_LINE1_X2, SEND_DW_LINE1_Y2, BLACK );
      lcd.drawLine( SEND_DW_LINE2_X1, SEND_DW_LINE2_Y1, SEND_DW_LINE2_X2, SEND_DW_LINE2_Y2, BLACK );
      lcd.drawLine( SEND_DW_LINE3_X1, SEND_DW_LINE3_Y1, SEND_DW_LINE3_X2, SEND_DW_LINE3_Y2, BLACK );
      break;
    case 2:
      lcd.drawLine( SEND_RT_LINE1_X1, SEND_RT_LINE1_Y1, SEND_RT_LINE1_X2, SEND_RT_LINE1_Y2, BLACK );
      lcd.drawLine( SEND_RT_LINE2_X1, SEND_RT_LINE2_Y1, SEND_RT_LINE2_X2, SEND_RT_LINE2_Y2, BLACK );
      lcd.drawLine( SEND_RT_LINE3_X1, SEND_RT_LINE3_Y1, SEND_RT_LINE3_X2, SEND_RT_LINE3_Y2, BLACK );
      break;
    case 3:
      lcd.drawLine( SEND_LF_LINE1_X1, SEND_LF_LINE1_Y1, SEND_LF_LINE1_X2, SEND_LF_LINE1_Y2, BLACK );
      lcd.drawLine( SEND_LF_LINE2_X1, SEND_LF_LINE2_Y1, SEND_LF_LINE2_X2, SEND_LF_LINE2_Y2, BLACK );
      lcd.drawLine( SEND_LF_LINE3_X1, SEND_LF_LINE3_Y1, SEND_LF_LINE3_X2, SEND_LF_LINE3_Y2, BLACK );
      break;
    case 4:
      lcd.drawLine( SEND_RU_LINE1_X1, SEND_RU_LINE1_Y1, SEND_RU_LINE1_X2, SEND_RU_LINE1_Y2, BLACK );
      lcd.drawLine( SEND_RU_LINE2_X1, SEND_RU_LINE2_Y1, SEND_RU_LINE2_X2, SEND_RU_LINE2_Y2, BLACK );
      lcd.drawLine( SEND_RU_LINE3_X1, SEND_RU_LINE3_Y1, SEND_RU_LINE3_X2, SEND_RU_LINE3_Y2, BLACK );
      break;
    case 5:
      lcd.drawLine( SEND_LU_LINE1_X1, SEND_LU_LINE1_Y1, SEND_LU_LINE1_X2, SEND_LU_LINE1_Y2, BLACK );
      lcd.drawLine( SEND_LU_LINE2_X1, SEND_LU_LINE2_Y1, SEND_LU_LINE2_X2, SEND_LU_LINE2_Y2, BLACK );
      lcd.drawLine( SEND_LU_LINE3_X1, SEND_LU_LINE3_Y1, SEND_LU_LINE3_X2, SEND_LU_LINE3_Y2, BLACK );
      break;
    case 6:
      lcd.fillRect( SEND_STOP_X, SEND_STOP_Y, SEND_STOP_SIZE, SEND_STOP_SIZE, BLACK );
      break;
    case 7:
      lcd.drawLine( SEND_LED_LINE1_X1, SEND_LED_LINE1_Y1, SEND_LED_LINE1_X2, SEND_LED_LINE1_Y2, BLACK );
      lcd.drawLine( SEND_LED_LINE2_X1, SEND_LED_LINE2_Y1, SEND_LED_LINE2_X2, SEND_LED_LINE2_Y2, BLACK );
      break;
    default:
      break;
  }
}

//送信処理
static void Send_Order() {
  boolean result = UNMATCH;

  for ( int i = 0; i < 8; i++ ) {
    if ( Pass_CodeVal(i) == CodeVal ) {
      result = MATCH;
      MatchNum = i;

    }
  }
  
  lcd.setTextColor(BLACK);
  lcd.setTextSize(3);

  //Serial.println(MatchNum); // MatchNumが送信する数字

  if ( Get_Web_State() == true ) { //シミュレータと接続されていたら
    Send_Web_Order(MatchNum);
  }

  else if ( Get_Connect_State() == true ) { //実機と接続されていたら
    //データ送信とパリティチェックの結果確認
    ErrorCount = 0;
    SendResult = Send_Order_Num(MatchNum);

    while ( SendResult != RECEIVE_ACK ) { //応答がACKでない間
      if ( SendResult == RECEIVE_NAK ) { //応答がNAKの場合
        ErrorCount++;
        if ( ErrorCount >= 3 ) {
          lcd.drawString( "通信エラー", 10, 180 );
          delay(200);
          Connection_Init(); //microbit側もリセットする
          return;
        }
      }
      else if ( SendResult == TEN_SECONDS ) { //10秒経過した場合
        lcd.drawString( "通信エラー", 10, 180 );
        delay(200);
        BLEDevice::deinit(); //初期化のための追加処理
        Connection_Init(); //microbit側もリセットする
        return;
      }
      SendResult = Send_Order_Num(MatchNum);
     
    }

  }
  if( Get_Ar_State() == true ){ //firebase,arと接続していたら
    Send_Firebase(MatchNum);

  }
  
  //lcd.drawString( String(MatchNum), 10, 180 ); //指令自体を表示("上"など)
  Disp_Order(MatchNum); //送信した指令を表示
  delay(200);
  return;
}
