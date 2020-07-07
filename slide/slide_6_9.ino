int prevPersonUp=0;
int prevPersonDown=0;
int cds1=A0;
int cds2=A1;
int cds3=A2;
int precds1=0;
int precds2=0;
int precds3=0;
void setup ()
{
  Serial.begin(9600);
  //pinMode (DigitalPin1, OUTPUT);
  //pinMode (DigitalPin2, OUTPUT);

}
  
void loop (){
  //digitalWrite (DigitalPin1, HIGH);
  //digitalWrite (DigitalPin2, HIGH);
  cds1 = analogRead(A0);  // 위쪽
  cds2 = analogRead(A1);  // 아래쪽
  cds3 = analogRead(A2);  // 아래쪽2
  //if(cds1-precds1>100){//윗쪽 사람있음}
  else if(cds1-precds1<-100){//윗쪽 사람 사라짐
    prevPersonUp=millis();
  }
  //if(cds2-precds2>100){//아래쪽(위) 사람 있음}
  else if(cds2-precds2<-100){//아래쪽(위) 사람 사라짐
    if(prevPersonDown>0){
      Serial.print("사람이 역주행 하려함, 사용시간 : ");
      Serial.println(millis()-prevPersonDown);
      prevPersonDown=0;
    }
  }
  //if(cds3-precds3>100){//아래쪽(아래) 사람 있음}
  else if(cds3-precds3<-100){//아래쪽(아래) 사람 사라짐
    if(prevPersonUp>0){
      Serial.print("사람이 정상적으로 내려옴, 사용시간 : ");
      Serial.println(millis()-prevPersonUp);
      prevPersonUp=0;
    }
    else{
      prevPersonDown=millis();
    }
  }
  if(prevPersonUp>0&&millis()-prevPersonUp>10000) prevPersonUp=0;
  if(prevPersonDown>0&&millis()-prevPersonDown>10000) prevPersonUp=0;

  
  //윗쪽 사람있음->윗쪽 사람없음->아래쪽 사람있음->아래쪽 사람없음 : 정상루트 : 파란불(이용가능)
  //아래쪽(아래)사람있음->아래쪽(위)사람있음 : 역주행 시도, 알람 &신호등 : 빨간불
  //윗쪽 사람있음->윗쪽 사람 없음->윗쪽 사람있음 : 사용중인 사람 있음 :노란불
  //사람 있다 없음 -> 다른쪽 나타나지 않음 10초 : 초기화
  //한쪽 사람 있다 없음 있다 없음 -> 사람 바뀜 : 초기화
}
