
//Libraries for the Bluetooth connection
#include <SoftwareSerial.h>

//pins for the bluetooth 10/RX to the bluetooths TX, and 11/TX to the bluetooths RX
SoftwareSerial mySerial(10, 11); // RX, TX


//Library for the CapacitiveSensor
#include <CapacitiveSensor.h>

// set the pins that are used, in this case 4 and 2
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2);
// 10k resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired


// your group number
int group = 1;
//  hand sensor
int hand = 0;
//  old hand sensor value
int oldHand = 0;
//the number of hand the have been shook
int handCount = 0;


// threshold for the sensitivity for the sensor
int thresholdLow = 5;
int thresholdHigh = 15;

// for checking if the hand shake value have been low once before its high
int low = 0;


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
  long sensor =  cs_4_2.capacitiveSensor(30);



  if (sensor < thresholdLow) {
    low = 1;
  } else if (sensor > thresholdHigh && low == 1) {
    handCount++;
    low = 0;
  }

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {

    // if any new hand shakes been made, call the url, else do nothing
    if (handCount  != 0) {
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
        url += "/index.php?HandCount=";

        APIvalue = APIvalue + handCount; // add the local handCount value to the APIvalue

        url += APIvalue; // add the updated value to the url

        // Sends the newspaper count to the api
        mySerial.println(url); // sends the url to the bluetooth


        Serial.println("");
        Serial.println("Api updated");
        Serial.println("");

        //clear the serials

        mySerial.flush();
        Serial.flush();//Waits for the transmission of outgoing serial data to complete


        handCount = 0; //reset the counter
        checkState = 1; // change the state
      }
    }
    callurlTimer = millis();// sets callurlTimer to be millis()/the time the arduino been on
  }


  // prints the current handCount in the serial montitor
  Serial.print("hand sensor value: ");
  Serial.println(sensor);

  Serial.print("handCount: ");
  Serial.println(handCount);

  delay(250); // small delay

}



