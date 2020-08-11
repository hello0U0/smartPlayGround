int sensor = D3;  // 홀자기센서를 디지털 10번 핀에 연결

void setup(){
  pinMode(sensor,INPUT); // 홀 자기센서를 입력으로 설정
  Serial.begin(9600);
}

int pre=0,last=0; //그네한 주기의 시작과 끝 시간을 보여준다(단위 ms)
int between=0;
int count=0; //그네의 주기 횟수(주기변화 없으면 그네를 이용하지 않는 것으로 판단
int noncount=0; // 그네 주기변화가 없는 횟수
void loop(){
  int val = digitalRead(sensor);
  if(val == HIGH){    // 자기장이 감지 되었을 때
    last=millis();
    between=last-pre;//왔다갔다한 시간
    if(between<1000){
       noncount++;
       if(noncount>15&&count>0){
        Serial.print("그네 사용 끝, 그네 주기횟수 : ");
        Serial.println(count);
        count=0;
        noncount=0;
       }
    }
    else{
      Serial.println(between);
      pre=last;
      count++;
      noncount=0;
   } 
 }
}
