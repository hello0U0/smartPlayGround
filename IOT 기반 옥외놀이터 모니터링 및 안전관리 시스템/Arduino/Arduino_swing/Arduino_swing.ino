#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}
#define DEBOUNCE_MS       20000         //micros
#define LONGCLICK_MS    1200000
#define DOUBLECLICK_MS  1400000 
#define upthreshold 30000               //이것보다 시간이 적으면 높이 올라간 것
#define pin D8

/////////////////////write playid///////////////////
#define playid 0
/////////////////////write WiFi//////////////////////
static const char* ssid = "";//와이파이
static const char* password = "";//비번
////////////////////write ip////////////////////////////////
const char* host = "0,0,0,0";
byte ip[]={0,0,0,0};
int port=8200;
int port2=8400;

////////////////////////////////////////////////////////////////////////////
unsigned long sensortime=0;
unsigned long usetime=0;
int swingcase=0;
////////////////////////////////////////////////////////////////////////////////////
WiFiUDP UDP;
WiFiUDP UDP2;
WiFiClient client;

/////////////////////////////////////////////////////////////////////////////////
void connectTCPHost();
void sendmessage();
void sendstatemessage();
void connectWiFiWPA();
//////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////
void caltime();
void settime();
int month_to_digit(char* str);
int day_to_digit(char* str);
String getTime();
//////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void buttonReset();
void buttonLoop();
void blongpressed();
void bpressed();
void breleased();

int prev_state=0;
int state=0;
int upThreshold=0;
int pressed=1;
int released=0;
int isstop=1;
int ispressed=0;
int ishigh=0;
unsigned long click_ms;
unsigned long down_ms;
unsigned long up_ms;
unsigned int debounce_time_ms;
unsigned int down_time_ms = 0;
unsigned long nowtime;
bool pressed_triggered = false;
bool longclick_detected = false;


int counter=0;
/////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println("Waiting for connection and IP Address from DHCP");
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  UDP.begin(port);
  UDP2.begin(port2);
  buttonReset();
  settime();
}
void loop() {
  buttonLoop();
}
/////////////////////////////////////////////
void blongpressed(){
  sendmessage();
}
void bpressed(){
}
void breleased(){
}
void buttontest(){
  prev_state = state;
  state = digitalRead(pin);
  Serial.println(state);
}
////////////////////////////////////////////////////////////////////////////
void buttonLoop(){
  prev_state = state;
  state = digitalRead(pin);
  //upThreshold=digitalRead(up_pin);
  nowtime=micros();
  //센서 안으로
  if (prev_state == pressed && state == released) {         
    down_ms = nowtime;
    pressed_triggered = false;
    bpressed();
  } //센서 밖으로
  else if (prev_state == released && state == pressed&&ispressed) {
    down_time_ms = nowtime - down_ms;
    if(10000<down_time_ms&&down_time_ms<upthreshold){
      swingcase=2;
      ishigh=1;
    }
    if (down_time_ms >= debounce_time_ms) {
      breleased();
      ispressed=0;
    }
  }
  //제대로 센서 밖으로 ------ if()시작함
  else if (state == released && !pressed_triggered && (micros() - down_ms >= debounce_time_ms)) {
    bpressed();
    ispressed=1;
    pressed_triggered = true;
    if(isstop==1){
      isstop=0;
      click_ms = down_ms;
    }
  }//멈춤
  else if (state == released &&pressed_triggered&& (micros() - down_ms >= LONGCLICK_MS)&&!isstop) {
    isstop=1;
    caltime();
    usetime=nowtime-click_ms;
    if(swingcase==0)swingcase=1;
    blongpressed();
    swingcase=0;
    
  }//센서 밖에서 높이 올라가면
  /*else if(state == released &&pressed_triggered&&upThreshold==released){
    Serial.println("너무 높이 올라감");
    swingcase=2;
  }*/
  sendstatemessage();
  yield();
}
///////////////////////////////////////////////////////////////////////////////////////////
void buttonReset(){
  down_time_ms = 0;
  pressed_triggered = true;
  longclick_detected = false;
  prev_state=0;
}
//////////////////////////////////////////////////
void sendmessage(){
  String sendmessage="";
  UDP.beginPacket(ip,port);
  String starttime=realtime;
  sendmessage+=String(playid);
  sendmessage+="/";
  sendmessage+=String(swingcase);
  sendmessage+="/";
  sendmessage+=starttime;
  sendmessage+="/";
  sendmessage+=String(day);
  sendmessage+="/";
  sendmessage+=String(usetime);
  sendmessage+="/";
  char packetBuffer[255]={0};
  std::vector<char> writable(sendmessage.begin(), sendmessage.end());
  writable.push_back('\0');
  char* ptr = &writable[0];
  strcpy(packetBuffer,sendmessage);
  UDP.write(ptr);
  UDP.endPacket();
}
void sendstatemessage(){
  String sendmessage="";
  UDP.beginPacket(ip,port2);
  sendmessage+=String(state);
  sendmessage+="/";
  sendmessage+=String(ishigh);
  sendmessage+="/";
  char packetBuffer[255]={0};
  std::vector<char> writable(sendmessage.begin(), sendmessage.end());
  writable.push_back('\0');
  char* ptr = &writable[0];
  strcpy(packetBuffer,sendmessage);
  UDP.write(ptr);
  UDP.endPacket();
}
void connectWiFiWPA(){
  
}
void connectTCPHost(){
  Serial.print("connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  const int httpPort = port;
  while(!client.connect(ip, port))
  {
    Serial.println("connection failed");
    delay(1000);
  }
  Serial.println("connection successed");
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
