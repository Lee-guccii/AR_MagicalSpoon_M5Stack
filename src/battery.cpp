#include <M5Stack.h>

#include "battery.h"

#include <LovyanGFX.hpp>

extern LGFX lcd;



static int giBattery = 0;


//バッテリー表示
#define BATTERY_HESO_X    255 //電池へそ
#define BATTERY_HESO_Y     10
#define BATTERY_HESO_W      7 //幅
#define BATTERY_HESO_H     10 //高さ
#define BATTERY_HESO_R      1 //半径

#define BATTERY_WAKU_X    260 //大枠
#define BATTERY_WAKU_Y      5
#define BATTERY_WAKU_W     50 //幅
#define BATTERY_WAKU_H     20 //高さ
#define BATTERY_WAKU_R      2 //半径

#define BATTERY_10_X      298 // 10%以上
#define BATTERY_10_Y        7
#define BATTERY_10_W       10 //幅
#define BATTERY_10_H       16 //高さ

#define BATTERY_50_X      286 // 50%以上
#define BATTERY_50_Y        7
#define BATTERY_50_W       10 //幅
#define BATTERY_50_H       16 //高さ

#define BATTERY_75_X      274 // 75%以上
#define BATTERY_75_Y        7
#define BATTERY_75_W       10 //幅
#define BATTERY_75_H       16 //高さ

#define BATTERY_90_X      262 // 90%以上
#define BATTERY_90_Y        7
#define BATTERY_90_W       10 //幅
#define BATTERY_90_H       16 //高さ


void Display_Battery_Level(){
    giBattery = M5.Power.getBatteryLevel();
    lcd.setCursor(0, 0);


    // 電池へそ
    lcd.drawRoundRect(BATTERY_HESO_X, BATTERY_HESO_Y, BATTERY_HESO_W, BATTERY_HESO_H, BATTERY_HESO_R, BLACK);
    lcd.fillRoundRect(BATTERY_HESO_X, BATTERY_HESO_Y, BATTERY_HESO_W, BATTERY_HESO_H, BATTERY_HESO_R, BLACK);
    // 大外枠
    lcd.drawRoundRect(BATTERY_WAKU_X, BATTERY_WAKU_Y, BATTERY_WAKU_W, BATTERY_WAKU_H, BATTERY_WAKU_R, BLACK);
    lcd.fillRoundRect(BATTERY_WAKU_X, BATTERY_WAKU_Y, BATTERY_WAKU_W, BATTERY_WAKU_H, BATTERY_WAKU_R, BLACK);
    // 10%以下
    if( giBattery <= 10 ){
        lcd.drawRect(BATTERY_10_X, BATTERY_10_Y, BATTERY_10_W, BATTERY_10_H, RED);
        lcd.fillRect(BATTERY_10_X, BATTERY_10_Y, BATTERY_10_W, BATTERY_10_H, RED);
    }else{
        lcd.drawRect(BATTERY_10_X, BATTERY_10_Y, BATTERY_10_W, BATTERY_10_H, GREEN);
        lcd.fillRect(BATTERY_10_X, BATTERY_10_Y, BATTERY_10_W, BATTERY_10_H, GREEN);
    }
    // 50%以上
    if( giBattery >= 50 ){
        lcd.drawRect(BATTERY_50_X, BATTERY_50_Y, BATTERY_50_W, BATTERY_50_H, GREEN);
        lcd.fillRect(BATTERY_50_X, BATTERY_50_Y, BATTERY_50_W, BATTERY_50_H, GREEN);
    }
    // 75%以上
    if( giBattery >= 75 ){
        lcd.drawRect(BATTERY_75_X, BATTERY_75_Y, BATTERY_75_W, BATTERY_75_H, GREEN);
        lcd.fillRect(BATTERY_75_X, BATTERY_75_Y, BATTERY_75_W, BATTERY_75_H, GREEN);
    }
    // 90%以上
    if( giBattery >= 90 ){
        lcd.drawRect(BATTERY_90_X, BATTERY_90_Y, BATTERY_90_W, BATTERY_90_H, GREEN);
        lcd.fillRect(BATTERY_90_X, BATTERY_90_Y, BATTERY_90_W, BATTERY_90_H, GREEN);
    }

    
}

