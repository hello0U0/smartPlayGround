int sensor = D3;  // 홀자기센서를 디지털 10번 핀에 연결

void setup(){
  pinMode(sensor,INPUT); // 홀 자기센서를 입력으로 설정
  Serial.begin(9600);
}

int pre=0,last=0; //그네 시작과 끝 시간을 보여준다(단위 ms)
int between=0;

void loop(){
  int val = digitalRead(sensor);
  if(val == HIGH){    // 자기장이 감지 되었을 때
    last=millis();
    between=last-pre;//왔다갔다한 시간
    //Serial.println("감지됨");
    Serial.println(between);
//시간을 이용해서 각속도 측정
  if(between>2000){
     Serial.print(“이용중”); 
       }


if(between)
    pre=millis();//1/1000초

   
    delay(500);
 }
}



이용중: 이용중인지 데이터를 받기
이용중이 얼마나 오래 쓰는지 체크해서 그네 사용기간을 세기
얼마나 높게 올라가냐 체크



미끄럼틀 완성
통신, MQTT
그네 공식 및 기술 설명 앞으로의 문제
미끄럼틀


int DigitalPin1 = 4;  
int DigitalPin2 = 3;

int prevPersonUp=0;
int prevPersonDown=0;
void setup ()
{

  Serial.begin(9600);
  pinMode (DigitalPin1, OUTPUT);
  pinMode (DigitalPin2, OUTPUT);
}
int cds1=A0;
int cds2=A1;
  
void loop ()

{
  //digitalWrite (DigitalPin1, HIGH);
  //digitalWrite (DigitalPin2, HIGH);
 

  cds1 = analogRead(A0);  // 위쪽
  cds2 = analogRead(A1);  // 아래쪽


  if(cds1 > 500 && cds2 > 500){

     Serial.println("둘 다 사람이 있는 것");
  }
  
  else if(cds1 > 500 && cds2 < 500){     //위에 사람이 있고 아래에 사람이 없음

    if(prevPersonDown==1){
      Serial.println(" 사람이 아래에서 위로 올라왔음");
      prevPersonDown=0;
    }
    else{
       Serial.println("위쪽만 사람이 있음");
       prevPersonUp=1;
    }    
  }
  
  else if(cds1< 500 && cds2 > 500){ //아래쪽에 사람이 있는거
  
    if(prevPersonUp==1){
       Serial.println(" 사람이 위에서 아래로 내려왔음");
       prevPersonUp=0;

    }
    else{
       Serial.println("아래쪽만 사람이 있음");
       prevPersonDown=1;
    }
   
  }
  else{
    Serial.println("둘 다 사람이 없음");
  }
}
