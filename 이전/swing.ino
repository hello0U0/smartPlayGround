#include <EasyButton.h>

// Arduino pin where the buttons are connected to
#define BUTTON_A_PIN 25

#define BAUDRATE 115200

// ButtonA
EasyButton buttonA(BUTTON_A_PIN);

long t1;


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
  Serial.println(t1);
 
}
void setup()
{
  // Initialize Serial for debuging purposes
  Serial.begin(BAUDRATE);

  Serial.println();
  Serial.println(">>> EasyButton multiple buttons example <<<");

  // Initialize the button1
  buttonA.begin();
  // Add the callback function to be called when the buttonA is pressed.
  buttonA.onPressed(onButtonAPressed);
}

void loop()
{
  // Continuously read the status of the buttons
  buttonA.read();

  
}
