#include <M5Stack.h> // M5Stackライブラリ
#include <FirebaseESP32.h> // FirebaseにM5Stackを接続させるためのライブラリ

#include <stdint.h>
#include <BLEDevice.h>

//Web Bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "btn.h"
#include "us.h"
#include "menu.h"
#include "set_code.h"
#include "set_tempo.h"
#include "send_code.h"
#include "connect.h"
#include "web_blue.h"
#include "firebase.h"
#include "battery.h"

#define LGFX_M5STACK
#include <LovyanGFX.hpp>



LGFX lcd;

//状態判別用
#define STATE_MENU    0
#define STATE_CODE    1
#define STATE_TEMPO   2
#define STATE_SEND    3
#define STATE_CONNECT 4


/* 全体 */
static int State; //状態判別用

/* メニュー画面用 */
static int MenuSelectMode; //選択項目

/* コード登録画面用 */
static int WaySelectMode;
static boolean WayFlag;

/* WEBコード登録用 */
static int WebCodeSelectMode;

/* M5コード登録用 */
static int CodeSelectMode; //選択項目
static int Horizon;        //配列の横
static int Vertical;       //配列の縦
static int BitNum;         //ビット番号

/*
指令の配列の形(2次元)

y/x  0         1
0   1番目     5番目
1   2番目     6番目
2   3番目     7番目
3   4番目     8番目

*/

/* テンポ登録画面用 */
static int TempoSelectMode;
static unsigned long Tempo;

/* 外部接続画面用 */
static int OutSelectMode;
static boolean OutFlag;

/* Web接続画面用 */
static int WebSelectMode;

/* 実機接続画面用 */
static int ConSelectMode;
static boolean Connect_Result; //接続処理結果

/* AR接続画面用 */
static int ArSelectMode;

/* ROOM選択画面用 */
static int RoomSelectMode;
static boolean RoomFlag;


uint8_t MacAddress[6];
static char MacAddressChr[18] = {0};
static String MacAddressStr;

void setup() {
  M5.begin(true, true, true, false);

  M5.Power.begin();

  Serial.print(115200);


  lcd.init();
  lcd.setFont( &fonts::efontJA_12 );
  lcd.fillScreen(WHITE);
  lcd.setTextColor(BLACK);
  lcd.setTextSize(2);

  State = STATE_MENU;
  Interval_Init();
  Tempo = Check_Tempo();
  //Connection_Init();

  esp_read_mac(MacAddress, ESP_MAC_WIFI_STA);  // Wi-FiのMACアドレスを取得する
  sprintf(MacAddressChr, "%02X:%02X:%02X:%02X:%02X:%02X", MacAddress[0], MacAddress[1], MacAddress[2], MacAddress[3], MacAddress[4], MacAddress[5]);
  MacAddressStr = String(MacAddressChr);
}

void loop() {
  switch (State) {
    
    /* メニュー */
    case STATE_MENU:

      //entry
      Menu_Text();
      MenuSelectMode = SET_CODE;
      Select_Menu(MenuSelectMode);


      do {
        
        M5.update();

        
        
        if ( Btn_A() == BTN_PUSHED ) {
          if ( MenuSelectMode == CONNECT ) {
            MenuSelectMode = SET_CODE;
          }
          else {
            MenuSelectMode++;
          }
          Select_Menu(MenuSelectMode);
          BtnA_Wait();//誤動作対策
        }
        
        if ( Btn_B() == BTN_PUSHED ) {
          State = MenuSelectMode;
          /*テスト ここから
          if ( MenuSelectMode == SET_CODE ) {
            State = SET_CODE;
          }
          else if ( MenuSelectMode == SET_TEMPO ) {
            State = SET_TEMPO; 
          }
          else if ( MenuSelectMode == SEND_CODE ) {
            State = SEND_CODE;
          }
          else if ( MenuSelectMode == CONNECT ) {
            State = CONNECT;
          }
          ここまで */
          BtnB_Wait();//誤動作対策
        }
        
      } while ( State == STATE_MENU );


      //exit
      lcd.fillScreen(WHITE);
      
      break;

    
    /* コード登録 */
    case STATE_CODE:

      //entry
      WayFlag = false;
      Code_Way_Text();
      WaySelectMode = WEB_CODE;
      Code_Way_Disp(WaySelectMode);

      do {

        M5.update();

        if ( Btn_A() == BTN_PUSHED ) {
          BtnA_Wait();
          if ( WaySelectMode == WAY_RETURN ) {
            WaySelectMode = WEB_CODE;
          }
          else {
            WaySelectMode++;
          }
          Code_Way_Disp(WaySelectMode);         
        }

        if ( Btn_B() == BTN_PUSHED ) {
          BtnB_Wait();
          lcd.fillScreen(WHITE);

          switch (WaySelectMode) {
            case WEB_CODE:

              Web_Code_Set();
              Code_Disp();
              WebCodeSelectMode = WEB_REGIST;
              Web_Code_Btn(WebCodeSelectMode);
            
              do {
                M5.update();

                if ( Btn_B() == BTN_PUSHED ) {
                  BtnB_Wait();//誤動作対策
                  if ( WebCodeSelectMode == WEB_REGIST ) {
                    if ( Code_Check() == CODE_OK ) {
                      State = STATE_MENU;
                      WayFlag = true;
                    }
                  }
                  else if ( WebCodeSelectMode == WEB_RETURN ) {
                    WayFlag = true;
                  }
                }
                
                if ( Btn_C() == BTN_PUSHED ) {
                  BtnC_Wait();//誤動作対策
                  if ( WebCodeSelectMode == WEB_RETURN ) {
                    WebCodeSelectMode = WEB_REGIST; 
                  }
                  else {
                    WebCodeSelectMode++;
                  }
                  Web_Code_Btn(WebCodeSelectMode);
                }
                
              } while ( WayFlag == false );

              WayFlag = false;
              break;

            case M5_CODE:

              //entry
              Code_Disp();
              Code_Btn_Disp();
              CodeSelectMode = BITS;
              Vertical = 0;
              Horizon = 0;
              BitNum = 1;
              Select_Code( CodeSelectMode, Vertical, Horizon, BitNum );

              do {

                M5.update();

                if ( Btn_A() == BTN_PUSHED ) {
                  if ( CodeSelectMode != BITS ) {
                    CodeSelectMode = BITS;
                    Vertical = 0;
                    Horizon = 0;
                    BitNum = 1;
                    Code_Btn_Disp();
                  }
                  else if ( Vertical == 3 ) {
                    Exit_Bit( Vertical, Horizon, BitNum );
                    CodeSelectMode = REGIST;
                  }
                  else {
                    Exit_Bit( Vertical, Horizon, BitNum );
                    Vertical++;
                  }
                  Select_Code( CodeSelectMode, Vertical, Horizon, BitNum );
                  BtnA_Wait();//誤動作対策
                }

                if ( Btn_B() == BTN_PUSHED ) {
                  if ( CodeSelectMode == BITS ) {
                    Bit_Flip( Vertical, Horizon, BitNum );
                  }
                  else if ( CodeSelectMode == REGIST ) {
                    if ( Code_Check() == CODE_OK ) {
                      State = STATE_MENU;
                      WayFlag = true;
                    }
                    Select_Code( CodeSelectMode, Vertical, Horizon, BitNum );
                  }
                  else if ( CodeSelectMode == RESET )  {
                    Code_Reset();
                  }
                  else if ( CodeSelectMode == CODE_RETURN ) {
                    //State = STATE_MENU;
                    WayFlag = true;
                  }
                  BtnB_Wait();//誤動作対策
                }

                if ( Btn_C() == BTN_PUSHED ) {
                  if ( CodeSelectMode == BITS ) {
                    Exit_Bit( Vertical, Horizon, BitNum );
                    if ( ( Horizon == 1 ) && ( BitNum == 3 ) ) {
                      Horizon = 0;
                      BitNum = 1;
                    }
                    else if ( ( Horizon == 0 ) && ( BitNum == 3 ) ) {
                      Horizon = 1;
                      BitNum = 1;
                    }
                    else {
                      BitNum++;
                    }
                  }
                  else {
                    if ( CodeSelectMode == CODE_RETURN ) {
                      CodeSelectMode = REGIST;
                    }
                    else {
                      CodeSelectMode++;
                    }
                  }
                  Select_Code( CodeSelectMode, Vertical, Horizon, BitNum );
                  BtnC_Wait();//誤動作対策
                }        
        
              } while ( WayFlag == false );

              WayFlag = false;
              break;

            case WAY_RETURN:
              State = STATE_MENU;
              break;
          }
          lcd.fillScreen(WHITE);
          Code_Way_Text();
          WaySelectMode = WEB_CODE;
          Code_Way_Disp(WaySelectMode);
        }

      } while ( State == STATE_CODE );
      
      //exit
      lcd.fillScreen(WHITE);
      
      break;

    
    /* テンポ登録 */
    case STATE_TEMPO:

      //entry
      TempoSelectMode = REGIST_START;
      Tempo_Disp_Init();
      Tempo = Check_Tempo();

      do {

        M5.update();

        Now_Tempo();
        
        if ( Btn_B() == BTN_PUSHED ) {
          if ( TempoSelectMode == REGIST_START ) {
            Tempo_Regist();
            //Tempo = Check_Tempo();
            //テスト動作
            //State = STATE_MENU;
            break; //登録テンポ確認のために、再度テンポ登録画面を表示する
          }
          else if ( TempoSelectMode == TEMPO_RETURN ) {
            State = STATE_MENU;
          }
          BtnB_Wait();//誤動作対策
        }

        if ( Btn_C() == BTN_PUSHED ) {
          if ( TempoSelectMode == TEMPO_RETURN ) {
            TempoSelectMode = REGIST_START;
          }
          else {
            TempoSelectMode++;
          }
          Tempo_Btn_Disp(TempoSelectMode);
          BtnC_Wait();//誤動作対策
        }
        
      } while ( State == STATE_TEMPO );


      //exit
      lcd.fillScreen(WHITE);
      
      break;

    
    /* コード送信 */
    case STATE_SEND:

      //entry
      Set_Tempo(Tempo);
      Send_Disp_Init();

      do {

        M5.update();

        Notes_Disp();

        if ( Btn_B() == BTN_PUSHED ) {
          State = STATE_MENU;
          BtnB_Wait();//誤動作対策
        }

        if ( Check_Hit() == HITSOUND ) {
          Reset_Disp();
          Send_Disp();
          delay(1000);
          Reset_Disp();
          Send_Disp_Init();
          BtnC_Wait();//誤動作対策
        }
        
      } while ( State == STATE_SEND );


      //exit
      lcd.fillScreen(WHITE);
      
      break;
    
    /* 外部接続 */
    case STATE_CONNECT:

      //entry
      OutFlag = false;
      Outside_Text();
      OutSelectMode = WEB;
      Outside_Disp(OutSelectMode);

      do {

        M5.update();

        if ( Btn_A() == BTN_PUSHED ) {
          if ( OutSelectMode == OUT_RETURN ) {
            OutSelectMode = WEB;
          }
          else {
            OutSelectMode++;
          }
          Outside_Disp(OutSelectMode);
          BtnA_Wait();//誤動作対策
        }

        if ( Btn_B() == BTN_PUSHED ) {
          BtnB_Wait();//誤動作対策
          switch (OutSelectMode) {
            
            case WEB:
              //entry
              WebSelectMode = WEB_CONNECT_START;
              Web_Menu_Disp(WebSelectMode);


              do {
                M5.update();

                if ( Btn_B() == BTN_PUSHED ) {
                  BtnB_Wait();//誤動作対策
                  if ( WebSelectMode == WEB_CONNECT_START ) {
                    Web_Connect( MacAddressStr );
                    OutFlag = true;
                  }
                  
                  else if ( WebSelectMode == WEB_CONNECT_RETURN ) {
                    OutFlag = true;
                  }
                }

                if ( Btn_C() == BTN_PUSHED ) {
                  if ( WebSelectMode == WEB_CONNECT_RETURN ) {
                    WebSelectMode = WEB_CONNECT_START;
                  }
                  else {
                    WebSelectMode++;
                  }
                  Web_Menu_Disp(WebSelectMode);
                  BtnC_Wait();//誤動作対策
                }
        
              } while ( OutFlag == false );

              //exit
              OutFlag = false;
              break;

            case ROBOT:
              //entry
              Connect_Result = CONNECT_FAILED;
              ConSelectMode = CONNECT_START;
              Connect_Menu_Disp(ConSelectMode);


              do {
                M5.update();

                if ( Btn_B() == BTN_PUSHED ) {
                  if ( ConSelectMode == CONNECT_START ) {
                    Connection_Init();
                    Connect_Result = Connect_Start();
                    OutFlag = true;
                  }
                  else if ( ConSelectMode == CONNECT_RETURN ) {
                    OutFlag = true;
                  }
                  BtnB_Wait();//誤動作対策
                }

                if ( Btn_C() == BTN_PUSHED ) {
                  if ( ConSelectMode == CONNECT_RETURN ) {
                    ConSelectMode = CONNECT_START;
                  }
                  else {
                    ConSelectMode++;
                  }
                  Connect_Menu_Disp(ConSelectMode);
                  BtnC_Wait();//誤動作対策
                }
        
              } while ( OutFlag == false );

              //exit
              OutFlag = false;
              break;

//
            case AR:

//////////////////////////////////////////////////////////////
              //entry
              lcd.fillScreen(WHITE);
              
              RoomFlag = false;
              Ar_Room_Text();
              RoomSelectMode = ROOM1;
              Ar_Room_Disp(RoomSelectMode);

              do {

                M5.update();

                if ( Btn_A() == BTN_PUSHED ) {
                  if ( RoomSelectMode == ROOM_RETURN ) {
                    RoomSelectMode = ROOM1;
                  }
                  else {
                    RoomSelectMode++;
                  }
                  Ar_Room_Disp(RoomSelectMode);
                  BtnA_Wait();//誤動作対策
                }

                if ( Btn_B() == BTN_PUSHED ) {
                  BtnB_Wait();//誤動作対策

                  RoomFlag = true;
                }
                
              } while ( RoomFlag == false );

              //exit
              lcd.fillScreen(WHITE);

              if(RoomSelectMode != ROOM_RETURN){
                //Connect_Result = AR_CONNECT_FAILED;
                  ArSelectMode = AR_CONNECT_START;
                  Ar_Menu_Disp(ArSelectMode);

                  do {
                    M5.update();

                    if ( Btn_B() == BTN_PUSHED ) {
                      BtnB_Wait();//誤動作対策
                      if ( ArSelectMode == AR_CONNECT_START ) {
                        Firebase_Connect(RoomSelectMode, MacAddressStr);
                        OutFlag = true;
                      }
                      
                      else if ( ArSelectMode == AR_CONNECT_RETURN ) {
                        OutFlag = true;
                      }
                    }

                    if ( Btn_C() == BTN_PUSHED ) {
                      if ( ArSelectMode == AR_CONNECT_RETURN ) {
                        ArSelectMode = AR_CONNECT_START;
                      }
                      else {
                        ArSelectMode++;
                      }
                      Ar_Menu_Disp(ArSelectMode);
                      BtnC_Wait();//誤動作対策
                    }
            
                  } while ( OutFlag == false );

              }


///////////////////////////////////////////////////////







              // //Connect_Result = AR_CONNECT_FAILED;
              // ArSelectMode = AR_CONNECT_START;
              // Ar_Menu_Disp(ArSelectMode);

              // do {
              //   M5.update();

              //   if ( Btn_B() == BTN_PUSHED ) {
              //     BtnB_Wait();//誤動作対策
              //     if ( ArSelectMode == AR_CONNECT_START ) {
              //       Firebase_Connect();
              //       OutFlag = true;
              //     }
                  
              //     else if ( ArSelectMode == AR_CONNECT_RETURN ) {
              //       OutFlag = true;
              //     }
              //   }

              //   if ( Btn_C() == BTN_PUSHED ) {
              //     if ( ArSelectMode == AR_CONNECT_RETURN ) {
              //       ArSelectMode = AR_CONNECT_START;
              //     }
              //     else {
              //       ArSelectMode++;
              //     }
              //     Ar_Menu_Disp(ArSelectMode);
              //     BtnC_Wait();//誤動作対策
              //   }
        
              // } while ( OutFlag == false );

              //exit
              OutFlag = false;
              break;


            case OUT_RETURN:
              State = STATE_MENU;
              break;
          }
          lcd.fillScreen(WHITE);
          Outside_Text();
          OutSelectMode = WEB;
          Outside_Disp(OutSelectMode);
        }
        
      } while ( State == STATE_CONNECT );

      //exit
      lcd.fillScreen(WHITE);
    
      break;
  }
}
