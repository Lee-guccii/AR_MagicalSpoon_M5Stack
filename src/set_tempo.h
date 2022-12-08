//テンポ登録画面用
#define REGIST_START 0    //テンポ登録開始
#define TEMPO_RETURN 1    //戻る

#define INIT_INTERVAL 500 //テンポ初期値

void Interval_Init(void);        //テンポの初期化
void Tempo_Disp_Init(void);      //遷移時の処理と画面表示
void Now_Tempo(void);            //登録されているテンポに従って音符を表示
void Tempo_Btn_Disp(int);        //選択中の項目を強調
void Tempo_Regist(void);         //テンポ登録
unsigned long Check_Tempo(void); //テンポ確認
