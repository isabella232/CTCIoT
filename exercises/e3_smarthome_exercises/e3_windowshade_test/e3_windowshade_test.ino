


//Library for the servo
#include <Servo.h>

Servo shadesservo;  // create servo object to control a servo

int buttonPin = 2;    // the number of the button pin

int buttonState = 0;   // variable for reading the button status



void setup() {


  shadesservo.attach(4);  // attaches the servo on pin 2 to the servo object
  // configure the serial
  Serial.begin(9600);

  while (!Serial);// waits for the serial monitor to open
}

void loop() {

  // read the state of the button value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) { //button is pushed
    // servo off state

    Serial.println("shade off");
    shadesservo.write(30);

  } else {
    // servo on state
    Serial.println("shade on");
    shadesservo.write(180);
  }



  delay(250);  //small delay
}



