#include <EasyButton.h>

// Arduino pin where the buttons are connected to
#define BUTTON_A_PIN 2
#define BUTTON_B_PIN 3
#define BUTTON_C_PIN 4
#define BUTTON_D_PIN 5

#define BAUDRATE 115200

// ButtonA
EasyButton buttonA(BUTTON_A_PIN);
// ButtonB
EasyButton buttonB(BUTTON_B_PIN);
// ButtonC
EasyButton buttonC(BUTTON_C_PIN);
// ButtonD
EasyButton buttonD(BUTTON_D_PIN);

long t1;
long t2;
long t3;
long t4;


// Callback function to be called when button1 is pressed
void onButton1Pressed()
{
  Serial.println("Button1 pressed");
}

// Callback function to be called when button is pressed
void onButtonAPressed()
{
  t1= micros();
  //Serial.println("ButtonA pressed");
  //Serial.println(t1);
 
}
void onButtonBPressed()
{
  t2= micros();
  //Serial.println("ButtonB pressed");
 // Serial.println(t2);
}
void onButtonCPressed()
{
   t3= micros();
  //Serial.println("ButtonC pressed");
   //Serial.println(t3);
   //Serial.println(t2);
   float timeinterval1 = (float)(t3-t2)/(float)1000000;
   double timeinterval2 = (double)(t3-t2)/(double)1000000;
   Serial.println(timeinterval1,7);
   Serial.println(timeinterval2,7);
}
void onButtonDPressed()
{ 
  t4= micros();
  //Serial.println("ButtonD pressed");
  //Serial.println(t4);
}

void setup()
{
  // Initialize Serial for debuging purposes
  Serial.begin(BAUDRATE);

  Serial.println();
  Serial.println(">>> EasyButton multiple buttons example <<<");

  // Initialize the button1
  buttonA.begin();
  // Initialize the button2
  buttonB.begin();
   // Initialize the button1
  buttonC.begin();
  // Initialize the button2
  buttonD.begin();
  // Add the callback function to be called when the buttonA is pressed.
  buttonA.onPressed(onButtonAPressed);
  // Add the callback function to be called when the buttonB is pressed.
  buttonB.onPressed(onButtonBPressed);
  // Add the callback function to be called when the buttonC is pressed.
  buttonC.onPressed(onButtonCPressed);
  // Add the callback function to be called when the buttonD is pressed.
  buttonD.onPressed(onButtonDPressed);
}

void loop()
{
  // Continuously read the status of the buttons
  buttonA.read();
  buttonB.read();
  buttonC.read();
  buttonD.read();

  
}
