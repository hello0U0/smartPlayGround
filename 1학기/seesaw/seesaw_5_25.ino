int SPin1=D3;
int state1=0;


void setup(){
  pinMode(SPin1, INPUT);
  Serial.begin(9600);
}


void loop(){
  state1= digitalRead(SPin1);

  if(state1==HIGH){
    Serial.println("기울어짐");
  }
   else if(state1==LOW){
    Serial.println("기울어지지 않음");
  }

  delay(1000);
}
