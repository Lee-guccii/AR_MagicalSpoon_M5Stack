//メニュー画面用
#define SET_CODE  1
#define SET_TEMPO 2
#define SEND_CODE 3
#define CONNECT   4

//外部接続画面用
#define WEB        0
#define ROBOT      1
#define AR         2
#define OUT_RETURN 3

//roomの選択
#define ROOM1      1
#define ROOM2      2
#define ROOM3      3
#define ROOM_RETURN 4

void Menu_Text(void);  //メニューのタイトル
void Select_Menu(int); //選択中項目強調

void Outside_Text(void);  //外部接続のタイトル
void Outside_Disp(int);   //外部接続のメニュー

void Ar_Room_Text(void);   //部屋決めのタイトル
void Ar_Room_Disp(int);   //部屋決めのメニュー
