int DigitalPin = 4;  // Digital input
//int cds = A0;
void setup ()

{

  Serial.begin(9600);
  pinMode (DigitalPin, OUTPUT);

}

void loop ()

{
  digitalWrite (DigitalPin, HIGH);
  int cds=A0;
  cds = analogRead(A0); 
  
  Serial.println(cds);

 delay(1000);





 if(cds >500){
   Serial.println("사람이 있다");
 }
 else if(cds <500){
   Serial.println("사람이 없음");
 }
}
