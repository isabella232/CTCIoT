
// motor pin
int motorPin = 6;

int buttonPin = 2;    // the number of the button pin

int buttonState = 0;   // variable for reading the button status


void setup() {


  // configure the motorPin as a OUTPUT
  pinMode(motorPin, OUTPUT);

  // configure the serial
  Serial.begin(9600);

  while (!Serial);// waits for the serial monitor to open
}

void loop() {

  // read the state of the button value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) { //button is pushed

    // motor/fan on
    Serial.println("fan on");


    analogWrite(motorPin, 255);

  } else {
    // motor/fan  off
    Serial.println("fan off");
    digitalWrite(motorPin, LOW);

  }


  delay(250);  //small delay

}




