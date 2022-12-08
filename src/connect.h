//接続画面用
#define CONNECT_START  0 //接続開始
#define CONNECT_RETURN 1 //戻る

#define CONNECT_SUCCESS true //接続処理の結果
#define CONNECT_FAILED  false

void Connection_Init(void);       //接続状態記録用変数を初期化
void Connect_Menu_Disp(int);      //実機接続画面を表示
void Connect_State_Disp(boolean); //接続状態を表示
boolean Connect_Start(void);      //接続を開始する
int Send_Order_Num(int);          //指令の番号を送信する
void Connect_Reset(void);         //micro:bitとの接続を切る
boolean Get_Connect_State(void);  //micro:bitとの接続状態を確認する
