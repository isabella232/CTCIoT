

//Libraries for the Bluetooth connection
#include <SoftwareSerial.h>
//pins for the bluetooth 10/RX to the bluetooths TX, and 11/TX to the bluetooths RX
SoftwareSerial mySerial(10, 11); // RX, TX

// your group number
int group = 1;
//  number of steps detected
int stepCount = 0;
// threshold for the sensitivity for the sensor
int threshold = 600;

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
  int sensor = analogRead(0);// from analog sensor 0


  // if the sensor value is bigger then the threshold, add to the step count
  if (sensor > threshold) {
    stepCount++;
  }

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {

    // if any new steps have been added, call the url, else do nothing
    if (stepCount  != 0) {

      //check if the APIvalue needs to be retived
      if (checkState == 1) {
        //get the current value form the API
        //Setup for the content of the URL/web address
        String url =  "http://verkstad.cc/iot/mmx/";
        url += group;
        url += "/get_value.php";


        //send the url to the API
        mySerial.println(url); // sends the url to the bluetooth
        while (mySerial.available()) {

          //get the value of the return form the url
          char c = mySerial.read();
          APIvalue = APIvalue  * 10 + int(c) - 48;

        }
        Serial.println("");
        Serial.println("Api data retived");
        Serial.println("");

        //clear the serials

        mySerial.flush();
        Serial.flush();//Waits for the transmission of outgoing serial data to complete
        checkState = 0; // change the state
      } else {


        //Setup for the content of the URL/web address
        String url =  "http://verkstad.cc/iot/mmx/";
        url += group;
        url += "/index.php?stepCount=";
        APIvalue = APIvalue + stepCount; // add the local stepcount value to the APIvalue

        url += APIvalue; // add the updated value to the url

        // Sends the step count to the phone
        mySerial.println(url); // sends the url to the bluetooth

        Serial.println("");
        Serial.println("Api updated");
        Serial.println("");

        //clear the serials

        mySerial.flush();
        Serial.flush();//Waits for the transmission of outgoing serial data to complete

        checkState = 1; // change the state

        stepCount = 0; //reset the counter
      }

    }

    callurlTimer = millis();// sets callurlTimer to be millis()/the time the arduino been on

  }

  // prints the current step count in the serial montitor
  Serial.print("stepCount: ");
  Serial.println(stepCount);

  Serial.print("Sensor value: ");
  Serial.println(sensor);


  delay(500);  // small delay

}


