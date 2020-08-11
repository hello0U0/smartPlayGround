int prevPersonUp=0; //전에 사람이 지나갔다
int prevPersonDown=0; // 전에 아래 사람이 지나갔다
int cds1=A0;
int cds2=A2;
int cds3=A1;
int precds1=0; // 전 조도 센서값들..........수우우우우상
int precds2=0;
int precds3=0;
void setup ()
{
  Serial.begin(9600);
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
}
  int count=0;
void loop (){;
  cds1 = analogRead(A0);  // 위쪽
  cds2 = analogRead(A2);  // 아래쪽
 // Serial.print(cds1);
 // Serial.print("   ");
 // Serial.println(cds2);

  
  if(precds1-cds1>40){
    Serial.println("윗쪽사람이 지나감");
    prevPersonUp=1;
    if(prevPersonDown==1){
      Serial.println("움직이지 마시오");
    }
  }
  else{
    count++;
    if(count>20){
      prevPersonUp=0;
      count=0;
    }
  }
  precds1=cds1;

  
  if(precds2-cds2<-80){ 
    
    Serial.println("아래사람이 지나감");
    if(prevPersonUp==1){
      Serial.println("정상적으로 지나감");
      prevPersonUp=0;
    }
    else{
      Serial.println("역주행 의심");
      prevPersonDown=1;
    }
   
  }
  else{
    prevPersonDown=0;
  }
  
  precds2=cds2;
 
}
