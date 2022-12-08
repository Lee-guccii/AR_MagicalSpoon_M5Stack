//コード登録方法選択画面用
#define WEB_CODE   0
#define M5_CODE    1
#define WAY_RETURN 2

//WEBからのコード登録画面用
#define WEB_REGIST 0 //登録
#define WEB_RETURN 1 //戻る

//M5でのコード登録画面用
#define BITS        0 //ビット
#define REGIST      1 //登録
#define RESET       2 //リセット
#define CODE_RETURN 3 //戻る

#define CODE_OK true
#define CODE_NG false


void Code_Disp(void);                   //コード表示
void Code_Btn_Disp(void);               //ボタン表示
void Code_Reset(void);                  //コードリセット
void Select_Code( int, int, int, int ); //コード登録画面での選択中箇所を強調
void Exit_Bit( int, int, int );         //ビットから移動した際の処理
void Bit_Flip( int, int, int );         //ビット反転
boolean Code_Check(void);               //指令コードのチェック
int Pass_CodeVal(int);                  //指令コードの値(10進数)を渡す

void Web_Code_Rec(String); //Webから受信したコードを文字列として保存
void Web_Code_Set(); //文字列として保存されているコードを配列に記録
void Web_Code_Btn(int);    //Webからのコードを表示する際のボタン

void Code_Way_Text(void);  //コード登録方法選択画面のタイトル
void Code_Way_Disp(int);   //コード登録方法選択のメニュー
