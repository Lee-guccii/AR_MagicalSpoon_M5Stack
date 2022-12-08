#include <stdio.h>

#include "header.h"


const char* fname = "/wifi.csv";
File fp;
String Ssid;
String Pass;


String Get_Ssid(String Judge){ // 1 : SSIDを返す
  if(Judge == "1"){
    return Ssid;
  }else{
    Ssid = Judge;
    return "";
  }
}

String Get_Pass(String Judge){ // 1 : Passを返す
  if(Judge == "1"){
    return Pass;
  }else{
    Pass = Judge;
    return "";
  }
} 


void GetWifi_info() {
  static char Ssid[32];
  static char Pass[32];

  
// <<<  SDカードからWi-FiのSSIDとPASSWORDを取得する
  unsigned int Cnt = 0;
  char Data[64];
  char *Key;
  // char *Key_Ssid;
  // char *Key_Pass;
  char *Str;
  char Str_Copy[32];
  String Period_Check;

  

  fp = SD.open(fname, FILE_READ);
  while(fp.available()){
    Data[Cnt++] = fp.read();
  }

  Key = strtok(Data,",");
  if(String(Key) == "SSID"){
    Str = strtok(NULL,"\r"); 
    // 
    //全角スペースがあった場合，横のセルの空欄以降を入力してもらう
    if(strstr(Str, ",") != NULL){ //全角スペースがあるかの確認
      Period_Check = Str;
      if(strlen(strstr(Str, ",")) == 1){
        Period_Check.replace(",", "");
      }else{
        Period_Check.replace(",", "　");
      }
      strcpy(Str_Copy, Period_Check.c_str());
      strncpy(&Ssid[0], Str_Copy, strlen(Str_Copy)); 
    }else{
      strncpy(&Ssid[0], Str, strlen(Str)); 
    }
    //

    
  }

  //strtok(NULL,"\n");
  Key = strtok(NULL,",");
  if(String(Key) == "\nPASS"){
    Str = strtok(NULL,"\r");    // CR
    // 
    //全角スペースがあった場合，横のセルの空欄以降を入力してもらう
    if(strstr(Str, ",") != NULL){ //全角スペースがあるかの確認
      Period_Check = Str;
      if(strlen(strstr(Str, ",")) == 1){
        Period_Check.replace(",", "");
      }else{
        Period_Check.replace(",", "　");
      }
      strcpy(Str_Copy, Period_Check.c_str());
      strncpy(&Pass[0], Str_Copy, strlen(Str_Copy)); 
    }else{
      strncpy(&Pass[0], Str, strlen(Str)); 
    }
    
    strncpy(&Pass[0], Str, strlen(Str)); 
    
  }
  

  Get_Ssid(String(Ssid));
  Get_Pass(String(Pass));
  

  fp.close();
}

