
int sensor = 8;  // 홀자기센서를 디지털 10번 핀에 연결
int timePre=0,timeLast=0; //자기장이 읽혔을때 이전에 읽혔을때의 시간과 지금 읽힌 시간(단위 ms)
int minTime=400;
int peopleStart=0, peopleEnd=0;
int timeBetween=0;
int count=0; //그네의 주기 횟수(주기변화 없으면 그네를 이용하지 않는 것으로 판단
int noncount=0; // 그네 주기변화가 없는 횟수
void setup(){
  pinMode(sensor,INPUT); // 홀 자기센서를 입력으로 설정
  Serial.begin(9600);
  timePre=millis();
}


void loop(){
  int val = digitalRead(sensor);

  if(val == HIGH){    // 자기장이 감지 되었을 때
    timeLast=millis();
    timeBetween=timeLast-timePre;
    if(timeBetween>minTime){
      if(peopleStart==0&&count>0){
        peopleStart=millis();
        Serial.println("사용시작");
      }
      Serial.print("/");
      Serial.print(timeBetween);
      count++;
    }//사용한다고 판단하는 시간보다 크게 움직이면
    else{
      if(count<3&&peopleStart!=0){
        count=0;
        peopleStart=0;
      }
      else if(peopleStart!=0){
        peopleEnd=millis();
        Serial.println("");
        Serial.print("사용 끝! 사용시간 : ");
        Serial.println(peopleEnd-peopleStart);
        count=0;
        peopleStart=0;
        peopleEnd=0;
      }
    }//사용한다고 판단하는 시간보다 작게 움직이면

    if(peopleStart!=0){}
    timePre=timeLast;
    delay(300);
 }
}
