//Web接続画面用
#define WEB_CONNECT_START  0 //接続開始
#define WEB_CONNECT_RETURN 1 //戻る

#define WEB_CONNECT_SUCCESS true //接続処理の結果
#define WEB_CONNECT_FAILED  false

void Web_Menu_Disp(int);     //Web接続画面
void Web_Connect(String MAC_ADDRESS);      //Web接続
boolean Get_Web_State(void); //Webとの接続状態を確認する
void Send_Web_Order(int);    //Webに指令を送信
