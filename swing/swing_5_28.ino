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
    Serial.println("감지됨");
    Serial.println(between);
    pre=millis();//1/1000초
    delay(500);
 }
}
