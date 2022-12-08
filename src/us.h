//ピンへの入力
#define SPN_HIT   HIGH   //スプーンが叩かれた
#define SPN_UNHIT LOW    //スプーンが叩かれていない

#define HITSOUND  1      //超音波が認識された判定
#define IGNORE    0      //超音波を認識しない

int Check_Hit(void); //超音波の有無を判定する
