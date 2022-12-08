//ar接続画面用
#define AR_CONNECT_START  0 //接続開始
#define AR_CONNECT_RETURN 1 //戻る

#define AR_CONNECT_SUCCESS true //接続処理の結果
#define AR_CONNECT_FAILED  false

void Ar_Menu_Disp(int);     //AR接続画面

boolean Get_Ar_State(void); //ARとの接続状態を確認する


void Firebase_Connect(int, String); //firebaseに接続
void Color_Choice_Disp(); //色を決める画面
void Send_Firebase(int); //firebaseにコードを送信

