#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>


#include "set_code.h"
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

#define WEB_X_POS  140 //"WEB"のX,Y座標
#define WEB_Y_POS   57
#define M5_X_POS   146   //"M5"のX,Y座標
#define M5_Y_POS   102
#define RET_X_POS  138 //"戻る"のX,Y座標
#define RET_Y_POS  192


#define X_SPACE 160 //指令毎の表示間隔_X方向
#define Y_SPACE  48 //指令毎の表示間隔_Y方向

#define UP_TRI_X1 23 //上矢印の三角部分座標
#define UP_TRI_Y1  8
#define UP_TRI_X2 11
#define UP_TRI_Y2 20
#define UP_TRI_X3 35
#define UP_TRI_Y3 20
#define UP_QUAD_X 17 //上矢印の四角部分座標
#define UP_QUAD_Y 20
#define UP_QUAD_W 13
#define UP_QUAD_H 20

#define DW_TRI_X1 23 //下矢印の三角部分座標
#define DW_TRI_Y1 42
#define DW_TRI_X2 11
#define DW_TRI_Y2 30
#define DW_TRI_X3 35
#define DW_TRI_Y3 30
#define DW_QUAD_X 17 //下矢印の四角部分座標
#define DW_QUAD_Y 11
#define DW_QUAD_W 13
#define DW_QUAD_H 20

#define RG_TRI_X1 40 //右矢印の三角部分座標
#define RG_TRI_Y1 25
#define RG_TRI_X2 28
#define RG_TRI_Y2 13
#define RG_TRI_X3 28
#define RG_TRI_Y3 37
#define RG_QUAD_X  9 //右矢印の四角部分座標
#define RG_QUAD_Y 19
#define RG_QUAD_W 20
#define RG_QUAD_H 13

#define LF_TRI_X1  6 //左矢印の三角部分座標
#define LF_TRI_Y1 25
#define LF_TRI_X2 18
#define LF_TRI_Y2 13
#define LF_TRI_X3 18
#define LF_TRI_Y3 37
#define LF_QUAD_X 18 //左矢印の四角部分座標
#define LF_QUAD_Y 19
#define LF_QUAD_W 20
#define LF_QUAD_H 13

#define RU_TRI_X1   35 //右上矢印の三角部分座標
#define RU_TRI_Y1   13
#define RU_TRI_X2   18
#define RU_TRI_Y2   13
#define RU_TRI_X3   35
#define RU_TRI_Y3   30
#define RU_QUAD1_X1 30 //右上矢印の四角部分半分の座標
#define RU_QUAD1_Y1 26
#define RU_QUAD1_X2 16
#define RU_QUAD1_Y2 40
#define RU_QUAD1_X3  8
#define RU_QUAD1_Y3 32
#define RU_QUAD2_X1 30 //右上矢印の四角部分半分の座標
#define RU_QUAD2_Y1 26
#define RU_QUAD2_X2 22
#define RU_QUAD2_Y2 18
#define RU_QUAD2_X3  8
#define RU_QUAD2_Y3 32

#define LU_TRI_X1   11 //左上矢印の三角部分座標
#define LU_TRI_Y1   13
#define LU_TRI_X2   11
#define LU_TRI_Y2   30
#define LU_TRI_X3   28
#define LU_TRI_Y3   13
#define LU_QUAD1_X1 16 //左上矢印の四角部分半分の座標
#define LU_QUAD1_Y1 26
#define LU_QUAD1_X2 30
#define LU_QUAD1_Y2 40
#define LU_QUAD1_X3 38
#define LU_QUAD1_Y3 32
#define LU_QUAD2_X1 16 //左上矢印の四角部分半分の座標
#define LU_QUAD2_Y1 26
#define LU_QUAD2_X2 24
#define LU_QUAD2_Y2 18
#define LU_QUAD2_X3 38
#define LU_QUAD2_Y3 32

#define ST_QUAD_X 13 //停止マークの座標
#define ST_QUAD_Y 15
#define ST_QUAD_W 21
#define ST_QUAD_H 21

#define LED_QUAD1_X 15 //LEDマーク
#define LED_QUAD1_Y 13
#define LED_QUAD1_W  2
#define LED_QUAD1_H 23
#define LED_QUAD2_X 16
#define LED_QUAD2_Y 34
#define LED_QUAD2_W 17
#define LED_QUAD2_H  2

#define ORDER_FRAME_X   1 //各指令を囲む四角の座標など
#define ORDER_FRAME_Y   1
#define ORDER_FRAME_W 158
#define ORDER_FRAME_H  47

#define CIRCLE_X   23 //指令マークを囲む円の座標
#define CIRCLE_Y   25
#define CIRCLE_RAD 20

#define BIT_BOX_X    48 //各ビットを表す四角の座標
#define BIT_BOX_Y    11
#define BIT_BOX_SIZE 25

#define BIT_BOX_SPACE 27 //ビットを表す四角の間隔

#define BTN_Y   195 //仮想ボタン共通の座標及び数値
#define BTN_W   100
#define BTN_H    40
#define BTN_RAD  10
#define STR_Y   202

//Webからのコード登録
#define W_REG_BTN_X 40  //登録ボタン
#define W_REG_STR_X 60

#define W_RET_BTN_X 170 //戻るボタン
#define W_RET_STR_X 200

//M5でのコード登録
#define REG_BTN_X   5 //"コード登録"のボタンと文字座標
#define REG_STR_X  30

#define RES_BTN_X 110 //"リセット"ボタンと文字座標
#define RES_STR_X 113

#define RET_BTN_X 215 //"戻る"ボタンと文字座標
#define RET_STR_X 240

//ここまで

#define MIN_HOR 0 //指令の縦と横の大きさ
#define MAX_HOR 1
#define MIN_VER 0
#define MAX_VER 3
#define MIN_BIT 1 //一つの指令内のビット番号
#define MAX_BIT 3

#define MIN_ORDER 0 //指令の数
#define MAX_ORDER 7

#define BIT_ON  1 //ビットのオンとオフ
#define BIT_OFF 0


//配列[横][縦][ビット]
static int BinCode[2][4][3] = { { { 0, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 }, { 0, 1, 1 } },
                                { { 1, 0, 0 }, { 1, 0, 1 }, { 1, 1, 0 }, { 1, 1, 1 } } };

static int DecCode[2][4] = { { 0, 1, 2, 3 },
                             { 4, 5, 6, 7 } };
/*
指令の番号(2次元)

    0         1
0  1番目     5番目
1  2番目     6番目
2  3番目     7番目
3  4番目     8番目

*/

static int CodeCount = 0;     //受信した指令の数を記録
static String ReceiveNum;     //受信した指令を記録
static String ReceiveBin[8];  //受信した指令を2進数で記録
static int ReceiveFlag = 0;   //指令の受信状況
static String Codes = "null"; //Webから受信したコードを一時保存

static void Icon_Disp();
static void Error_Box( int, int, uint16_t ); //コード重複箇所の色を変更
static String Web_Read(char);                //Web上のアプリからコードを読み取る


void Code_Way_Text() {
  lcd.setTextColor(MAGENTA);
  lcd.setTextSize(TITLE_TEXT_SIZE);
  lcd.drawString("コード登録", TITLE_X_POS, TITLE_Y_POS);

  Display_Battery_Level();
  
  return;
}

void Code_Way_Disp( int select ) {
  lcd.setTextColor(BLACK);
  lcd.setTextSize(TEXT_SIZE);
  
  switch(select) {
    case WEB_CODE:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  RED);
      lcd.drawString("WEB", WEB_X_POS, WEB_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("M5", M5_X_POS, M5_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
      
    case M5_CODE:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("WEB", WEB_X_POS, WEB_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("M5", M5_X_POS, M5_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
      
    case WAY_RETURN:
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS,  CYAN);
      lcd.drawString("WEB", WEB_X_POS, WEB_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + BTN_SPACE, BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, CYAN);
      lcd.drawString("M5", M5_X_POS, M5_Y_POS);
      lcd.fillRoundRect(BTN_X_POS, BTN_Y_POS + ( BTN_SPACE * 3 ), BTN_WIDTH, BTN_HEIGHT, BTN_RADIUS, RED);
      lcd.drawString("戻る", RET_X_POS, RET_Y_POS);
      break;
  }

  return;
}

static void Icon_Disp() {
  //上矢印
  lcd.fillTriangle( UP_TRI_X1, UP_TRI_Y1, UP_TRI_X2, UP_TRI_Y2, UP_TRI_X3, UP_TRI_Y3, BLUE );
  lcd.fillRect( UP_QUAD_X, UP_QUAD_Y, UP_QUAD_W, UP_QUAD_H, BLUE );
  
  //下矢印
  lcd.fillTriangle( DW_TRI_X1, DW_TRI_Y1 + ( Y_SPACE * 1 ), DW_TRI_X2, DW_TRI_Y2 + ( Y_SPACE * 1 ), DW_TRI_X3, DW_TRI_Y3 + ( Y_SPACE * 1 ), BLUE );
  lcd.fillRect( DW_QUAD_X, DW_QUAD_Y + ( Y_SPACE * 1 ), DW_QUAD_W, DW_QUAD_H, BLUE );
  
  //右矢印
  lcd.fillTriangle( RG_TRI_X1, RG_TRI_Y1 + ( Y_SPACE * 2 ), RG_TRI_X2, RG_TRI_Y2 + ( Y_SPACE * 2 ), RG_TRI_X3, RG_TRI_Y3 + ( Y_SPACE * 2 ), BLUE );
  lcd.fillRect( RG_QUAD_X, RG_QUAD_Y + ( Y_SPACE * 2 ), RG_QUAD_W, RG_QUAD_H, BLUE );
  
  //左矢印
  lcd.fillTriangle( LF_TRI_X1, LF_TRI_Y1 + ( Y_SPACE * 3 ), LF_TRI_X2, LF_TRI_Y2 + ( Y_SPACE * 3 ), LF_TRI_X3, LF_TRI_Y3 + ( Y_SPACE * 3 ), BLUE );
  lcd.fillRect( LF_QUAD_X, LF_QUAD_Y + ( Y_SPACE * 3 ), LF_QUAD_W, LF_QUAD_H, BLUE );
  
  //右上矢印
  lcd.fillTriangle( RU_TRI_X1 + X_SPACE, RU_TRI_Y1, RU_TRI_X2 + X_SPACE, RU_TRI_Y2, RU_TRI_X3 + X_SPACE, RU_TRI_Y3, BLUE );
  lcd.fillTriangle( RU_QUAD1_X1 + X_SPACE, RU_QUAD1_Y1, RU_QUAD1_X2 + X_SPACE, RU_QUAD1_Y2, RU_QUAD1_X3 + X_SPACE, RU_QUAD1_Y3, BLUE );
  lcd.fillTriangle( RU_QUAD2_X1 + X_SPACE, RU_QUAD2_Y1, RU_QUAD2_X2 + X_SPACE, RU_QUAD2_Y2, RU_QUAD2_X3 + X_SPACE, RU_QUAD2_Y3, BLUE );
  
  //左上矢印
  lcd.fillTriangle( LU_TRI_X1 + X_SPACE, LU_TRI_Y1 + ( Y_SPACE * 1 ), LU_TRI_X2 + X_SPACE, LU_TRI_Y2 + ( Y_SPACE * 1 ), LU_TRI_X3 + X_SPACE, LU_TRI_Y3 + ( Y_SPACE * 1 ), BLUE );
  lcd.fillTriangle( LU_QUAD1_X1 + X_SPACE, LU_QUAD1_Y1 + ( Y_SPACE * 1 ), LU_QUAD1_X2 + X_SPACE, LU_QUAD1_Y2 + ( Y_SPACE * 1 ), LU_QUAD1_X3 + X_SPACE, LU_QUAD1_Y3 + ( Y_SPACE * 1 ), BLUE );
  lcd.fillTriangle( LU_QUAD2_X1 + X_SPACE, LU_QUAD2_Y1 + ( Y_SPACE * 1 ), LU_QUAD2_X2 + X_SPACE, LU_QUAD2_Y2 + ( Y_SPACE * 1 ), LU_QUAD2_X3 + X_SPACE, LU_QUAD2_Y3 + ( Y_SPACE * 1 ), BLUE );
  
  //停止マーク
  lcd.fillRect( ST_QUAD_X + X_SPACE, ST_QUAD_Y + ( Y_SPACE * 2 ), ST_QUAD_W, ST_QUAD_H, BLUE );
  
  //LED表示
  lcd.drawRect( LED_QUAD1_X + X_SPACE, LED_QUAD1_Y + ( Y_SPACE * 3 ), LED_QUAD1_W, LED_QUAD1_H, BLUE );
  lcd.drawRect( LED_QUAD2_X + X_SPACE, LED_QUAD2_Y + ( Y_SPACE * 3 ), LED_QUAD2_W, LED_QUAD2_H, BLUE );
  
  return;
}


void Code_Disp() {
  for ( int i = MIN_HOR; i <= MAX_HOR; i++ ) { //横
    for ( int j = MIN_VER; j <= MAX_VER; j++ ) { //縦
      lcd.drawRect( ORDER_FRAME_X + ( i * 160 ), ORDER_FRAME_Y + ( j * 48 ), ORDER_FRAME_W, ORDER_FRAME_H, BLACK );
      lcd.fillCircle( CIRCLE_X + ( i * X_SPACE ), CIRCLE_X + ( j * Y_SPACE ), CIRCLE_RAD, YELLOW );
      lcd.drawRect( BIT_BOX_X + ( i * X_SPACE ), BIT_BOX_Y + ( j * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, BLACK );
      for ( int k = MIN_BIT; k <= MAX_BIT; k++ ) { //ビット
        if ( BinCode[i][j][k-1] == BIT_ON ) {
          lcd.drawRect( BIT_BOX_X + ( k * BIT_BOX_SPACE ) + ( i * X_SPACE ), BIT_BOX_Y + ( j * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, BLACK );
        }
        else {
          lcd.fillRect( BIT_BOX_X + ( k * BIT_BOX_SPACE ) + ( i * X_SPACE ), BIT_BOX_Y + ( j * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, BLACK );
        }
      }
    }
  }
  Icon_Disp();
  return;
}

void Code_Btn_Disp() {
  lcd.fillRoundRect( REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
  lcd.drawString( "登録", REG_STR_X, STR_Y );
  lcd.fillRoundRect( RES_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
  lcd.drawString( "リセット", RES_STR_X, STR_Y );
  lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
  lcd.drawString( "戻る", RET_STR_X, STR_Y );
  return;
}

void Code_Reset() {
  for ( int l = MIN_HOR; l <= MAX_HOR; l++ ) {
    for ( int m = MIN_VER; m <= MAX_VER; m++ ) {
      for ( int n = MIN_BIT; n <= MAX_BIT; n++ ) {
        BinCode[l][m][n-1] = BIT_OFF;
      }
    }
  }
  Code_Disp();
  return;
}

void Select_Code( int select, int vertical, int horizon, int bits ) {
  switch (select) {
    case BITS:
      if ( BinCode[horizon][vertical][bits-1] == BIT_ON ) {
        lcd.drawRect( BIT_BOX_X + ( bits * BIT_BOX_SPACE ) + ( horizon * X_SPACE ), BIT_BOX_Y + ( vertical * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, RED);
      }
      else {
        lcd.fillRect( BIT_BOX_X + ( bits * BIT_BOX_SPACE ) + ( horizon * X_SPACE ), BIT_BOX_Y + ( vertical * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, RED);
      }
      break;
    
    case REGIST:
      lcd.fillRoundRect( REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "登録", REG_STR_X, STR_Y );
      lcd.fillRoundRect( RES_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "リセット", RES_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
    
    case RESET:
      lcd.fillRoundRect( REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "登録", REG_STR_X, STR_Y );
      lcd.fillRoundRect( RES_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "リセット", RES_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
    
    case CODE_RETURN:
      lcd.fillRoundRect( REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "登録", REG_STR_X, STR_Y );
      lcd.fillRoundRect( RES_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "リセット", RES_STR_X, STR_Y );
      lcd.fillRoundRect( RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "戻る", RET_STR_X, STR_Y );
      break;
  }
  return;
}

void Exit_Bit( int vertical, int horizon, int bits ) {
  if ( BinCode[horizon][vertical][bits-1] == BIT_ON ) {
    lcd.drawRect( BIT_BOX_X + ( bits * BIT_BOX_SPACE ) + ( horizon * X_SPACE ), BIT_BOX_Y + ( vertical * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, BLACK);
  }
  else {
    lcd.fillRect( BIT_BOX_X + ( bits * BIT_BOX_SPACE ) + ( horizon * X_SPACE ), BIT_BOX_Y + ( vertical * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, BLACK);
  }
  return;
}

void Bit_Flip( int vertical, int horizon, int bits ) {
  switch ( BinCode[horizon][vertical][bits-1] ) {
    case BIT_OFF:
      lcd.fillRect( BIT_BOX_X + ( bits * BIT_BOX_SPACE ) + ( horizon * X_SPACE ), BIT_BOX_Y + ( vertical * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, WHITE);
      lcd.drawRect( BIT_BOX_X + ( bits * BIT_BOX_SPACE ) + ( horizon * X_SPACE ), BIT_BOX_Y + ( vertical * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, RED);
      BinCode[horizon][vertical][bits-1] = BIT_ON;
      break;
    
    case BIT_ON:
      lcd.fillRect( BIT_BOX_X + ( bits * BIT_BOX_SPACE ) + ( horizon * X_SPACE ), BIT_BOX_Y + ( vertical * Y_SPACE ), BIT_BOX_SIZE, BIT_BOX_SIZE, RED);
      BinCode[horizon][vertical][bits-1] = BIT_OFF;
      break;
  }
  return;
}

boolean Code_Check() {
  lcd.fillScreen(WHITE);
  int Comp[2][4];
  boolean CheckFlag;
  for ( int i = MIN_HOR; i <= MAX_HOR; i++ ) {
    for ( int j = MIN_VER; j <= MAX_VER; j++ ) {
      Comp[i][j] = ( 4*BinCode[i][j][0] ) + ( 2*BinCode[i][j][1] ) + BinCode[i][j][2];
    }
  }
  CheckFlag = CODE_OK;
  for ( int i = MIN_ORDER; i <= MAX_ORDER; i++ ) {
    for ( int j = i + 1; j <= MAX_ORDER; j++ ) {
      //商と余りを算出してインデックスを決定
      if ( Comp[i/4][i%4] == Comp[j/4][j%4] ) {
        Error_Box( i/4, i%4, CYAN );
        Error_Box( j/4, j%4, CYAN );
        CheckFlag = CODE_NG;
      }
    }
  }
  if ( CheckFlag == CODE_OK ) {
    for ( int i = MIN_HOR; i <= MAX_HOR; i++ ) {
      for ( int j = MIN_VER; j <= MAX_VER; j++ ) {
        DecCode[i][j] = Comp[i][j];
      }
    }
  }
  Code_Disp();
  Code_Btn_Disp();
  return CheckFlag;
}

static void Error_Box( int horizon, int vertical, uint16_t color ) {
  lcd.fillRect( ORDER_FRAME_X + ( horizon * X_SPACE ) ,  ORDER_FRAME_Y + ( vertical * Y_SPACE ) , ORDER_FRAME_W, ORDER_FRAME_H, color);
  return;
}

int Pass_CodeVal(int index) {
  int codeval;
  codeval = DecCode[index/4][index%4];
  return codeval;
}

void Web_Code_Rec( String webcode ) {
  Codes = webcode;
  return;
}

void Web_Code_Set() {
  if ( Codes == "null" ) { //Webからコードを受信していない場合は何もしない
    return;
  }
  
  CodeCount = 0;
  int index = 0; //前から何番目の文字かを記録
  String code;   //区切ったコードを記録
  String order;  //指令をStringからintに変換する際に使用する
  String num;    //
  while ( CodeCount <= MAX_ORDER ) {
    do {
      if ( Codes.charAt(index) == ',' ) {
        ReceiveNum = String( code.toInt(), BIN );
        while ( ReceiveNum.length() < 3 ) {
          ReceiveNum = '0' + ReceiveNum;
        }
        ReceiveBin[CodeCount] = ReceiveNum;
        ReceiveFlag = 0;
      }
      else {
        if ( ReceiveFlag == 0 ) {
          ReceiveBin[CodeCount] = "null";
          code = Codes.charAt(index);
          ReceiveFlag = 1;
        }
        else {
          code += Codes.charAt(index);
        }
      }
      index++;
    } while ( ReceiveBin[CodeCount] == "null" );
    Serial.println(ReceiveBin[CodeCount]);
    CodeCount++;
  }

  for (int i = MIN_ORDER; i <= MAX_ORDER; i++) {
    order = ReceiveBin[i];
    for (int j = ( MIN_BIT - 1 ); j <= ( MAX_BIT - 1 ); j++) {
      num = order.charAt(j);
      BinCode[i/4][i%4][j] = num.toInt();
    }
  }
  
  return;
}

void Web_Code_Btn( int select ) {
  switch (select) {
    case WEB_REGIST:
      lcd.fillRoundRect( W_REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "登録", W_REG_STR_X, STR_Y );
      lcd.fillRoundRect( W_RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "戻る", W_RET_STR_X, STR_Y );
      break;
      
    case WEB_RETURN:
      lcd.fillRoundRect( W_REG_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD, CYAN );
      lcd.drawString( "登録", W_REG_STR_X, STR_Y );
      lcd.fillRoundRect( W_RET_BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RAD,  RED );
      lcd.drawString( "戻る", W_RET_STR_X, STR_Y );
      break;
  }

  return;
}
