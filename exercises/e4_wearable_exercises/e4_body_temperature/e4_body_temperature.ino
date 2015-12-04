


//Libraries for the Bluetooth connection
#include <SoftwareSerial.h>

//pins for the bluetooth 10/RX to the bluetooths TX, and 11/TX to the bluetooths RX
SoftwareSerial mySerial(10, 11); // RX, TX



// your group number
int group = 1;
//  temperature
int temperatureCount = 0;
// saveing the old temperature value
int oldtemperatureCount = 0;

unsigned long int callurlTimer = 0; //  url timer
unsigned long int callurlDelay = 5000; // url delay. 1000=1 sec
int checkState = 1; // State for switching from getting the value from the api to sending the update the api

//the value from the Api
int APIvalue = 0;

void setup() {

  // configure the serial
  Serial.begin(9600);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

}

void loop() {

  // Take in the value from the sensor
  int sensorRaw = analogRead(0);// from analog sensor 0
  int sensor = (5.0 * sensorRaw * 100.0) / 1024;  // calculate the celsus from the raw value of the sensor
  // set the sensor value to the temperatureCount
  temperatureCount = sensor;


  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {

    //if the temp is the same, do nothing
    if (oldtemperatureCount == temperatureCount) {
      //temp is the same, no need to update it
    } else {

      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?temperature=";
      url += temperatureCount ;

      // Sends the temperature to the phone
      mySerial.println(url); // sends the url to the bluetooth


      Serial.println("");
      Serial.println("Api updated");
      Serial.println("");

      //clear the serials
      oldtemperatureCount = temperatureCount;// update the old temperature to the current temperature count

    }

    callurlTimer = millis();// sets callurlTimer to be millis()/the time the arduino been on
  }



  // prints the current temperature in the serial montitor
  Serial.print("temperature: ");
  Serial.println(temperatureCount);


  delay(250); // small delay

}


