
//Libraries for the servo
#include <Servo.h>

//clothesline servo
Servo clotheslineservo;  // create continuous servo object to control a servo

int buttonPin = 2;    // the number of the button pin

int buttonState = 0;   // variable for reading the button status


void setup() {


  clotheslineservo.attach(3);  // attaches the servo on pin 2 to the servo object

  // configure the serial
  Serial.begin(9600);

  while (!Serial);// waits for the serial monitor to open
}

void loop() {

  // read the state of the button value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) { //button is pushed
    Serial.println("clothesline in");
    // servo on state
    clotheslineservo.write(0);//0 is full speed backwards
  } else {
    // servo off state
    Serial.println("clothesline stop");
    clotheslineservo.write(90);//90 is no rotation

  }

  delay(250);  //small delay
}



