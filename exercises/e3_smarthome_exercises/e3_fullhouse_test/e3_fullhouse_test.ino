
//Libraries for the servo
#include <Servo.h>

//clothesline servo
Servo clotheslineservo;  // create continuous servo object to control a servo
//shades servo
Servo shadesservo;  // create standard servo object to control a servo

// fanmotor pin
int motorPin = 6; int kickStart = 0; // the motor is set to not run at full power, it needs a kick start to get going


// watersolenoid pin
int watersolenoid = 9;
// vent solenoid pin
int ventsolenoidPin = 10;


// the group numbers
int groupOne = 1; //clothesline servo
int groupTwo = 2; // vent the air solenoid
int groupThree = 3; // shades servo
int groupFour = 4;  // water plant solenoid
int groupFive = 5; //fan dc motor

//  temperature
int temperatureCount = 0;
// saveing the old temperature value
int oldtemperatureCount = 0;

int tempControl = 0; // state switcher for testing each device

void setup() {



  clotheslineservo.attach(3);  // attaches the servo on pin 2 to the servo object

  shadesservo.attach(4);  // attaches the servo on pin 3 to the servo object

  // configure the solenoidPin as a OUTPUT
  pinMode(watersolenoid, OUTPUT);

  // configure the solenoidPin as a OUTPUT
  pinMode(ventsolenoidPin, OUTPUT);

  // configure the motorPin as a OUTPUT
  pinMode(motorPin, OUTPUT);

  // configure the serial
  Serial.begin(9600);


  while (!Serial);// waits for the serial monitor to open
}

void loop() {
// changes the states of each motor/servo/solenoid
  if (tempControl == 0) {
    tempControl=1;
  }  else if (tempControl == 1) {
    tempControl=0;
  }


  //--------------------------------------------
  //temperature sensor and updater
  //--------------------------------------------

  // Take in the value from the sensor
  int sensorRaw = analogRead(0);// from analog sensor 0
  int sensor = (5.0 * sensorRaw * 100.0) / 1024;  // calculate the celsus from the raw value of the sensor
  // set the sensor value to the temperatureCount
  temperatureCount = sensor;

  Serial.print("temp: ");
  Serial.println(temperatureCount)


  //--------------------------------------------
  //clothesline servo
  //--------------------------------------------


  //checks if the temp control is off/0 or anything else is on
  if (tempControl == 0) {
    // servo off state
    clotheslineservo.write(90);//90 is no rotation
  } else {
    // servo on state
    clotheslineservo.write(180);

  }

  //--------------------------------------------
  //vent the air solenoid
  //--------------------------------------------



  //checks if the temp control is on/1 or anything else is off
  if (tempControl == 1) {
    // solenoid on
    digitalWrite(ventsolenoidPin, LOW);
  } else {
    // solenoid  off
    digitalWrite(ventsolenoidPin, HIGH);
  }


  //--------------------------------------------
  //shades servo
  //--------------------------------------------



  //checks if the temp control is off/0 or anything else is on
  if (tempControl == 0) {

    // servo off state
    shadesservo.write(30);

  } else {
    // servo on state
    shadesservo.write(180);
  }

  //--------------------------------------------
  //water plant solenoid
  //--------------------------------------------


  //checks if the temp control is on/1 or anything else is off
  if (tempControl == 1) {
    // solenoid on
    digitalWrite(watersolenoid, LOW);
  } else {
    // solenoid  off
    digitalWrite(watersolenoid, HIGH);
  }


  //--------------------------------------------
  //fan dc motor
  //--------------------------------------------


  //checks if the temp control is off/0 or anything else is on
  if (tempControl == 0) {
    // motor/fan on
    Serial.println("fan on");
    digitalWrite(motorPin, LOW);
    kickStart = 1;
  } else {
    // motor/fan  off
    Serial.println("fan off");
    while (kickStart == 1) {
      analogWrite(motorPin, 255);
      delay(250);
      kickStart = 0;
    }
    analogWrite(motorPin, 50);
    // prints and the Temp control states in the serial montitor



    delay(5000); //wait 5 sec
    

  }

}

