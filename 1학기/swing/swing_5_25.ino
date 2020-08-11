  // led를 디지털 5핀에 연결
int sensor = D3;  // 홀자기센서를 디지털 10번 핀에 연결

void setup(){
  pinMode(sensor,INPUT); // 홀 자기센서를 입력으로 설정
  Serial.begin(9600);
}

void loop(){
  int val = digitalRead(sensor);
  Serial.println(val);
  if(val == HIGH){    // 자기장이 감지 되었을 때
   
    Serial.println("감지됨");
 }
 else    {      
   Serial.println("감지되지 않음");
  }

  delay(500);
}
