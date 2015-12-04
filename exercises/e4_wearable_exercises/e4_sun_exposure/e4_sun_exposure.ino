

//Libraries for the Bluetooth connection
#include <SoftwareSerial.h>

//pins for the bluetooth 10/RX to the bluetooths TX, and 11/TX to the bluetooths RX
SoftwareSerial mySerial(10, 11); // RX, TX


// your group number
int group = 1;

// threshold for the LDR  sensor
int thresholdValue = 300;// adjust this to fit your light levels, make sure that the value in the sun is higher then this

// time in the sun
unsigned long int lightTime = 0;

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
  int sensor = analogRead(0); // from analog sensor 0

  //check if the light levels are lower then the threshold
  if (sensor > thresholdValue ) {
    lightTime = lightTime + 500; // 500 = 1/2 sec 
    //the light levels show high sun exposure, lightTime goes up

  }



  // Update the lightTime state to the api

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {


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
      url += "/index.php?sunExposure=";
      APIvalue = APIvalue + (lightTime / 1000); // change the local lighttime value to seconds  and add that to the APIvalue

      url += APIvalue; // add the updated value to the url

      // Sends the lightTime state to the phone
      mySerial.println(url); // sends the url to the bluetooth

      Serial.println("");
      Serial.println("Api updated");
      Serial.println("");

      //clear the serials

      mySerial.flush();
      Serial.flush();//Waits for the transmission of outgoing serial data to complete

      checkState = 1; // change the state

      lightTime = 0; // reset the lighttime
    }
    
    callurlTimer = millis();// sets callurlTimer to be millis()/the time the arduino been on
  
  }

  // prints the current light levels and the time in the light in the serial montitor
 
  Serial.print(lightTime/1000);
  Serial.print(" seconds of high light");
  Serial.println(" ");
  Serial.print("Current light level: ");
  Serial.println(sensor);

  delay(500); // small delay
}


