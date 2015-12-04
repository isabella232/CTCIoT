

//Libraries for the Bluetooth connection
#include <SoftwareSerial.h>

//pins for the bluetooth 10/RX to the bluetooths TX, and 11/TX to the bluetooths RX
SoftwareSerial mySerial(10, 11); // RX, TX



// your group number
int group = 1;
//  number of flexs
int flexCount = 0;
// thresholds for the flex
int thresholdLow = 180;
int thresholdHigh = 250;

// for checking if the flex value have been low once before its high
int low=0;

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

  // if the sensor value is first smaller then the low threshold then is higher then the high threshold
  //then a flex if done
  if (sensor < thresholdLow) {
    low = 1;
  } else if (sensor > thresholdHigh && low == 1) {
    flexCount++;
    low = 0;
  }

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {
    
    // if any new flexes been made, call the url, else do nothing
    if (flexCount  != 0) {
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
        url += "/index.php?flexCount=";
        APIvalue = APIvalue + flexCount; // add the local flexCount value to the APIvalue

        url += APIvalue ;

        // Sends the flex count to the phone
        mySerial.println(url); // sends the url to the bluetooth

        Serial.println("");
        Serial.println("Api updated");
        Serial.println("");

        //clear the serials

        mySerial.flush();
        Serial.flush();//Waits for the transmission of outgoing serial data to complete

        checkState = 1; // change the state
        flexCount = 0; //reset the counter
      }

    }

    callurlTimer = millis();// sets callurlTimer to be millis()/the time the arduino been on
  }


  // prints the current flex in the serial montitor
  Serial.print("flex: ");
  Serial.println(flexCount);
  Serial.print("Sensor value: ");
  Serial.println(sensor);

  delay(500);

}


