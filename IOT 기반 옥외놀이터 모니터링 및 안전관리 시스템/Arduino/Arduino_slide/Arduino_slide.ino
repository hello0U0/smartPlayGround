#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}
#define middleTh 10000000
#define threshold 1000
int pin_A=D4;
int pin_B=D5;
int pin_C=D6;
int pin_D=D7;
/////////////////////write playid///////////////////
#define playid 0
/////////////////////write WiFi//////////////////////
static const char* ssid = "EWHA-IOT";//와이파이
static const char* password = "dscho007";//비번
////////////////////write ip////////////////////////////////
const char* host = "192,168,0,35";
byte ip[]={192,168,0,35};
int port=8100;
int port2=8300;

WiFiUDP UDP;
WiFiUDP UDP2;
WiFiClient client;
/////////////////////////////////////////////////////////////////////////////////
void sendmessage();
void caltime();
void settime();
int month_to_digit(char* str);
int day_to_digit(char* str);
String getTime();
//////////////////////////////////////////////////////////////////////////////////////--세팅 같음
const char *month_arr[13] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const char *day_arr[7] = {"Sun", "Mon", "Tue", "Wen", "Thu", "Fri", "Sat"};
char buf[40];
char *time_cut=0;
char *time_val[8]={0};
char DaysOfMonth[]={31,28,31,30,31,30,31,31,30,31,30,31};
int year;
int month;
int date;
int day;
int hour;
int minute;
int second;
int ms;
String realtime;

unsigned long lasttime;
//////////////////////////////////////////////////////////////////

unsigned long starttime, usetime;
unsigned long nowtime;
unsigned long down_ms[4];
unsigned long up_ms[4];
bool tryOut[4]={0};
bool doing[4]={0};
int prev_A, prev_B, prev_C, prev_D;
int A, B, C, D;
int person[5][2]={0};//state
int upstate=0;
int downstate=8;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
  }
  UDP.begin(port);
  UDP2.begin(port2);
  settime();
  A=digitalRead(pin_A);
  B=digitalRead(pin_B);
  C=digitalRead(pin_C);
  D=digitalRead(pin_D);
}
void loop() {
  nowtime=micros();
  //이전상태 및 현재상태 저장
  prev_A=A;
  prev_B=B;
  prev_C=C;
  prev_D=D;
  A=digitalRead(pin_A);
  B=digitalRead(pin_B);
  C=digitalRead(pin_C);
  D=digitalRead(pin_D);
  
  //A,B,C,D의 현재 , 이전상태들로 현재 사람이 어디에 위치하는지 판단
  if(prev_A==1 && A==0){///////A인식
    if(upstate==0||upstate==4) upstate=1;
    else if(upstate==3) upstate=2;
  }else if(prev_A==0 && A==1){
    if(upstate==1) upstate=0;
    else if(upstate==2) upstate=3;
  }
  if(prev_B==1 && B==0){//////////////////////////////B인식
    if(upstate==1) upstate=2;
    else if(upstate==4||upstate==0) upstate=3;
  }else if(prev_B==0 && B==1){
    if(upstate==3) upstate=4;
    else if(upstate==2) upstate=1;
  }
  if(prev_C==1 && C==0){///////////////////////////////////////////C인식
    if(downstate==8||downstate==4) downstate=5;
    else if(downstate==7) downstate=6;
  }else if(prev_C==0 && C==1){
    if(downstate==5)downstate=4;
    else if(downstate==6) downstate=7;
  }
  if(prev_D==1 && D==0){////////////////////////////////D인식
    if(downstate==5) downstate=6;
    else if(downstate==8||downstate==4) downstate=7;
  }else if(prev_D==0 && D==1){
    if(downstate==7) downstate=8;
    else if(downstate==6) downstate=5;
  }
    //현재 상태와 이전상태를 이용하여 사람이 어떤 case의 이용중인지 판단
   if(person[0][0]==0&&upstate==1){////////////////////////////위에서 아래 시작
      person[0][0]=1;
      person[0][1]=1;
      starttime=nowtime;
    }
    else if(person[0][0]==1&&upstate==0){////////////////////////////위에서 아래 끝
      usetime=nowtime-starttime;
      sendmessage();
      person[0][0]=0;
      person[0][1]=0;
    }
    else if(person[0][0]==3&&upstate==4){
      person[0][0]=4;
      upstate=0;
    }
    else if(person[0][0]==upstate-1){
      person[0][0]=upstate;
      if(person[0][1]==1||person[0][1]==4||person[0][1]==5){}
      else{
        if(person[0][1]==2) person[0][1]=1;
        else if(person[0][1]==3) person[0][1]=4;
        else if(person[0][1]==6) person[0][1]=4;
      }
    }
    else if(person[0][0]==upstate+1){
      person[0][0]=upstate;
      if(person[0][1]==2||person[0][1]==3||person[0][1]==6){
        }
      else{
        if(person[0][1]==1) person[0][1]=2;
        else if(person[0][1]==4) person[0][1]=3;
        else if(person[0][1]==5) {}//person[0][1]=2;
      }
    }
    else if(person[0][0]==0&&downstate==7){////////////////////////////////////////아래에서 위 시작
      person[0][0]=7;
      person[0][1]=3;
      starttime=nowtime;
    }
    else if(person[0][0]==7&&downstate==8){////////////////////////////////////////아래에서 위
      usetime=nowtime-starttime;
      sendmessage();
      person[0][0]=0;
      person[0][1]=0;
    }
    
    else if(person[0][0]==5&&downstate==4){
      person[0][0]=4;
      downstate=8;
    }
    else if(person[0][0]==downstate-1){
      person[0][0]=downstate;
      if(person[0][1]==1||person[0][1]==4||person[0][1]==5){}
      else{
        if(person[0][1]==2) person[0][1]=1;
        else if(person[0][1]==3) person[0][1]=4;
        else if(person[0][1]==6) person[0][1]=4;
      }
    }
    else if(person[0][0]==downstate+1){
      person[0][0]=downstate;
      if(person[0][1]==2||person[0][1]==3||person[0][1]==6){}
      else{
        if(person[0][1]==1) person[0][1]=2;
        else if(person[0][1]==4) person[0][1]=3;
        else if(person[0][1]==5) person[0][1]=2;
      }
    }
    
    if(person[0][0]==4&&nowtime-starttime>middleTh){
      if(person[0][1]==1) person[0][1]=5;
      else if(person[0][1]==3) person[0][1]=6;
    }

    //현재 상태 전송
     sendstatemessage();
}
/////////////////////////////////////////////////
void sendmessage(){
  String sendmessage="";
  UDP.beginPacket(ip,port);
  String starttime=realtime;
  sendmessage+=String(playid);
  sendmessage+="/";
  sendmessage+=String(person[0][1]);
  sendmessage+="/";
  sendmessage+=starttime;
  sendmessage+="/";
  sendmessage+=String(day);
  sendmessage+="/";
  sendmessage+=String(usetime);
  sendmessage+="/";
  //char packetBuffer[255]={0};
  std::vector<char> writable(sendmessage.begin(), sendmessage.end());
  writable.push_back('\0');
  char* ptr = &writable[0];
  //strcpy(packetBuffer,sendmessage);
  UDP.write(ptr);
  UDP.endPacket();
  Serial.println(ptr);
}
void sendstatemessage(){
  String sendmessage="";
  UDP2.beginPacket(ip,port2);
  sendmessage+=String(A);
  sendmessage+="/";
  sendmessage+=String(B);
  sendmessage+="/";
  sendmessage+=String(C);
  sendmessage+="/";
  sendmessage+=String(D);
  sendmessage+="/";
  //char packetBuffer[255]={0};
  std::vector<char> writable(sendmessage.begin(), sendmessage.end());
  writable.push_back('\0');
  char* ptr = &writable[0];
  //strcpy(packetBuffer,sendmessage);
  UDP2.write(ptr);
  UDP2.endPacket();
  //Serial.println(ptr);
}
void settime(){
  String str_time = getTime();
  lasttime=micros();
  str_time.toCharArray(buf, 30);
  time_cut = strtok(buf, ", :");  
  for(int i=0; time_cut; i++){ 
    time_val[i] = time_cut; 
    time_cut = strtok(0, ", :");
  }
  year   = atoi(time_val[3]);
  month  = month_to_digit((char *)time_val[2]);
  date   = atoi(time_val[1]);
  day    = day_to_digit((char *)time_val[0]);
  hour   = atoi(time_val[4]) + 9;
  minute = atoi(time_val[5]);
  second = atoi(time_val[6]);
  if(hour>23) {
    hour %=24;
    if(++day>6) day=0;
    if     (!(year%400)) DaysOfMonth[1]=29;  // 윤년/윤달 계산
    else if(!(year%100)) DaysOfMonth[1]=28; 
    else if(!(year%  4)) DaysOfMonth[1]=29;  
    if(date<DaysOfMonth[month-1]) date++;
    else {
      date=1;
      if(++month>12) {
        month=1;
        year++;
      }  
    }
  }
  ms=0;
  realtime=String(year)+"-"+String(month)+"-"+String(date)+"-"+String(hour)+"-"+String(minute)+"-"+String(second)+".";
}

void caltime(){
  unsigned long timeDist=nowtime-lasttime;
  timeDist+=ms;
  lasttime=nowtime;
  int plustime=timeDist/1000000;
  second +=plustime;
  if(second>59) {
    second %=60;
    if(++minute>59){
      minute%=60;
      if(++hour>23){
        hour%=24;
        if(++day>6) day=0;
        if     (!(year%400)) DaysOfMonth[1]=29;  // 윤년/윤달 계산
        else if(!(year%100)) DaysOfMonth[1]=28; 
        else if(!(year%  4)) DaysOfMonth[1]=29;  
        if(date<DaysOfMonth[month-1]) date++;
        else {
         date=1;
          if(++month>12) {
            month=1;
            year++;
          }  
        }
      }
    }
  }
  timeDist=timeDist%1000000;
  ms=timeDist;
  String temp_ms=String(timeDist/100000);
  timeDist%=100000;
  temp_ms+=String(timeDist/10000);
  timeDist%=10000;
  temp_ms+=String(timeDist/1000);
  timeDist%=1000;
  temp_ms+=String(timeDist/100);
  timeDist%=100;
  temp_ms+=String(timeDist/10);
  temp_ms+=String(timeDist%10);
  String temp_month=String(month/10)+String(month%10);
  String temp_date=String(date/10)+String(date%10);
  String temp_hour=String(hour/10)+String(hour%10);
  String temp_minute=String(minute/10)+String(minute%10);
  String temp_second=String(second/10)+String(second%10);
  realtime=String(year)+"-"+temp_month+"-"+temp_date+"-"+temp_hour+"-"+temp_minute+"-"+temp_second+"."+temp_ms;
}
int month_to_digit(char* str) {
  for(int i=0; i<12; i++) {
      if(!strncmp(str, (char *)month_arr[i], 3)) return i+1;
  }
}
int day_to_digit(char* str) {
  for(int i=0; i<7; i++) {
      if(!strncmp(str, (char *)day_arr[i], 3)) return i;
  }
}
 
String getTime() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {
      Serial.println("connection failed, retrying...");
  }
  client.print("HEAD / HTTP/1.1\r\n\r\n");
 
  while(!!!client.available()) {
     yield();
  }
 
  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
}
