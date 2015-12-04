
// motor pin
int motorPin = 6;

int buttonPin = 2;    // the number of the button pin

int buttonState = 0;   // variable for reading the button status
int kickStart = 0; // the motor is set to not run at full power, it needs a kick start to get going


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

    while (kickStart == 1) {
      analogWrite(motorPin, 255);
      delay(250);
      kickStart = 0;
    }
    analogWrite(motorPin, 50);

  } else {
    // motor/fan  off
    Serial.println("fan off");
    digitalWrite(motorPin, LOW);
    kickStart = 1;
  }


  delay(250);  //small delay

}




