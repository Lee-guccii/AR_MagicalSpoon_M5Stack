//超音波確認用
#define HIT_ON  1
#define HIT_OFF 0

//ビットの番号確認用
#define BIT_1ST 0
#define BIT_2ND 1
#define BIT_3RD 2
#define BIT_4TH 3



void Set_Tempo(unsigned long); //登録されているテンポを記録する
void Send_Disp_Init(void);     //コード送信の基本画面
void Notes_Disp(void);         //基本画面での音符表示
void Send_Disp(void);          //入力中の画面
void Reset_Disp(void);         //画面切り替えの際のリセット


