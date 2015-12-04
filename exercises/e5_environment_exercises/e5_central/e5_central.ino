
//Libraries for the Yúns connection
#include <Bridge.h>// communication between the ATmega32U4(the arduino) and the AR9331 (Linux side)
#include <HttpClient.h> //Creates a HTTP client on Linux.

HttpClient client; // the HTTP client



// teacher short memory api for each group
int groupOne = 6; //to the carpark
int groupTwo = 7; // to the carcount
int groupThree = 8; // to the windstrength
int groupFour = 9;  // to the coupon
int groupFive = 10; //to the seat

// Url for each groups api
// the numbers in these url need to be in sync with what group does what exercise
// if group 5 does the "windstrength" exercise, change that url so its has the number 5 instead of the current 3
String groupOneUrl = "http://verkstad.cc/iot/mmx/1/get_value.php"; // for the carpark exercise
String groupTwoUrl = "http://verkstad.cc/iot/mmx/2/get_value.php"; // for the carcount exercise
String groupThreeUrl = "http://verkstad.cc/iot/mmx/3/get_value.php"; // for the windstrength exercise
String groupFourUrl = "http://verkstad.cc/iot/mmx/4/get_value.php"; // for the coupon exercise
String groupFiveUrl = "http://verkstad.cc/iot/mmx/5/get_value.php"; // for the seat exercise

//general timer
unsigned long int timer = 1000; //  1000=1 sec

// Control limit variables
//  Time for the car to on a carparking spot
unsigned long int carparkTime = 5; //seconds it waits
int oldcarparkStatus = 0; // for checking if the carpark status has changed
int carOvertime = 0; // times that a car has been parked to long

//  number of cars limit
int carLimit = 5; // the amount of cars
int oldcarcountStatus = 0; // for checking if the carCount status has changed
int carOverlimit = 0; // sets of times the limit has been reached


//  windstrength threshold
int windThreshold = 180;
int oldwindThresholdStatus = 0; // for checking if the windThreshold status has changed
int windOverlimit = 0; // sets of times the limit has been reached


//  coupon dispenser threshold
int couponDispened = 5; //the limit before the coupon is dispened
int oldcouponStatus = 0; // for checking if the coupon status has changed
int couponOverlimit = 0; // number of coupon that have been given out


//  seat sitting time limit
int seatTime = 5; //seconds it waits
int oldseatkStatus = 0; // for checking if the seat status has changed
int sittingOvertime = 0; // times that someone has been sitting to long

void setup() {
  // configure the 13 pin as a OUTPUT
  pinMode(13, OUTPUT);


  // Blink once with the led on pin 13 when the connection is ready
  digitalWrite(13, LOW);
  Bridge.begin();  Bridge.begin(); //starts the Bridge
  digitalWrite(13, HIGH);



  // configure the serial
  Serial.begin(9600);


  while (!Serial);// waits for the serial monitor to open
}

void loop() {

  //----------------------------------
  //Group One Carpackchecker
  //----------------------------------

  //Set the threshold on the API for the sensor unit
  //Setup for the content of the URL/web address
  String url =  "http://verkstad.cc/iot/mmx/";
  url += groupOne;
  url += "/index.php?carparkTime=";
  url += carparkTime; // time for the car to park
  // Sends the carparkTime to the api
  client.get(url); // sends the url to the client
  Serial.flush();//Waits for the transmission of outgoing serial data to complete



  // get current carparkchecker status
  client.get(groupOneUrl); // sends the url to the client

  //the carpark status from the api
  int carparkChecker = 0;

  // get the  carparkchecker value
  while (client.available()) {
    char c = client.read();
    carparkChecker = carparkChecker * 10 + c - 48;
  }

  Serial.flush();//Waits for the transmission of outgoing serial data to complete

  //add to the total amount of cars overstaying thier time
  if (carparkChecker == 1 && oldcarparkStatus == 0) {

    carOvertime = carOvertime + carparkChecker;
    oldcarparkStatus = carparkChecker; // update the oldcarparkStatus
  } else {

    oldcarparkStatus = carparkChecker; // update the oldcarparkStatus
  }
  Serial.print("current time limit per parking= ");
  Serial.println(carparkTime);
  Serial.print("Total times that a car stayed over the timelimit= ");
  Serial.println(carOvertime);

  //----------------------------------
  //Group Two Carcounter
  //----------------------------------

  //Set the threshold on the API for the sensor unit
  //Setup for the content of the URL/web address
  url =  "http://verkstad.cc/iot/mmx/";
  url += groupTwo;
  url += "/index.php?carLimit=";
  url += carLimit; // the amount of cars
  // Sends the carLimit to the api
  client.get(url); // sends the url to the client
  Serial.flush();//Waits for the transmission of outgoing serial data to complete



  // get current carLimit status
  client.get(groupTwoUrl); // sends the url to the client

  //the carStatus status from the api
  int carCountChecker = 0;

  // get the  carCountChecker value
  while (client.available()) {
    char c = client.read();
    carCountChecker = carCountChecker * 10 + c - 48;
  }

  Serial.flush();//Waits for the transmission of outgoing serial data to complete

  //add to the total amount of cars overstaying thier time
  if (carCountChecker == 1 && oldcarcountStatus == 0) {

    carOvertime = carOvertime + carCountChecker;
    oldcarcountStatus = carCountChecker; // update the oldcarcountStatus
  } else {

    oldcarcountStatus = carCountChecker; // update the oldcarcountStatus
  }
  Serial.print("current car limit= ");
  Serial.println(carLimit);
  Serial.print("Total times that a car stayed over the timelimit= ");
  Serial.println(carOverlimit);

  //----------------------------------
  //Group Three Windstrength
  //----------------------------------

  //Set the threshold on the API for the sensor unit
  //Setup for the content of the URL/web address
  url =  "http://verkstad.cc/iot/mmx/";
  url += groupThree;
  url += "/index.php?windthreshold=";
  url += windThreshold ; // the wind threshold
  // Sends the windThreshold  to the api
  client.get(url); // sends the url to the client
  Serial.flush();//Waits for the transmission of outgoing serial data to complete



  // get current windThreshold status
  client.get(groupThreeUrl); // sends the url to the client

  //the windThreshold  status from the api
  int windThresholdChecker = 0;

  // get the  windThresholdChecker value
  while (client.available()) {
    char c = client.read();
    windThresholdChecker = windThresholdChecker * 10 + c - 48;
  }

  Serial.flush();//Waits for the transmission of outgoing serial data to complete

  //add to the total amount times when the wind is over the threshold
  if (windThresholdChecker == 1 && oldwindThresholdStatus == 0) {

    windOverlimit  = windOverlimit  + windThresholdChecker;
    oldwindThresholdStatus  = windThresholdChecker; // update the oldwindThresholdStatus
  } else {

    oldwindThresholdStatus  = windThresholdChecker; // update the oldwindThresholdStatus
  }
  Serial.print("current wind threshold = ");
  Serial.println(windThreshold );
  Serial.print("Total times that the wind has been over threshold= ");
  Serial.println(windOverlimit);


  //----------------------------------
  //Group Four Coupon count
  //----------------------------------

  //Set the threshold on the API for the sensor unit
  //Setup for the content of the URL/web address
  url =  "http://verkstad.cc/iot/mmx/";
  url += groupFour;
  url += "/index.php?carLimit=";
  url += couponDispened; // couponDispened limit
  // Sends the couponDispened to the api
  client.get(url); // sends the url to the client
  Serial.flush();//Waits for the transmission of outgoing serial data to complete



  // get current carLimit status
  client.get(groupFourUrl); // sends the url to the client

  //the coupon status from the api
  int couponStatus = 0;

  // get the couponStatus value
  while (client.available()) {
    char c = client.read();
    couponStatus = couponStatus * 10 + c - 48;
  }

  Serial.flush();//Waits for the transmission of outgoing serial data to complete

  //add to the total amount of coupons
  if (couponStatus == 1 && oldcouponStatus  == 0) {

    couponOverlimit   = couponOverlimit   + couponStatus;
    oldwindThresholdStatus  = couponStatus; // update the oldcarcountStatus
  } else {

    oldcouponStatus   = couponStatus; // update the oldcarcountStatus
  }
  Serial.print("current coupon number = ");
  Serial.println(couponDispened);
  Serial.print("Total amount of coupon dispened");
  Serial.println(couponOverlimit);


  //----------------------------------
  //Group Five Seatchecker
  //----------------------------------

  //Set the threshold on the API for the sensor unit
  //Setup for the content of the URL/web address
  url =  "http://verkstad.cc/iot/mmx/";
  url += groupFive;
  url += "/index.php?seatTime=";
  url += seatTime; // time for for sitting
  // Sends the seatTime to the api
  client.get(url); // sends the url to the client
  Serial.flush();//Waits for the transmission of outgoing serial data to complete



  // get current seatChecker status
  client.get(groupFiveUrl); // sends the url to the client

  //the seatChecker status from the api
  int seatChecker = 0;

  // get the  seatChecker value
  while (client.available()) {
    char c = client.read();
    seatChecker = seatChecker * 10 + c - 48;
  }

  Serial.flush();//Waits for the transmission of outgoing serial data to complete

  //add to the total amount of someone sitting to long
  if (seatChecker == 1 && oldseatkStatus == 0) {

    sittingOvertime = sittingOvertime + seatChecker;
    oldseatkStatus = seatChecker; // update the oldseatkStatus
  } else {

    oldseatkStatus = seatChecker; // update the oldseatkStatus
  }
  Serial.print("current time limit per sitting= ");
  Serial.println(carparkTime);
  Serial.print("Total times that someone sat over the timelimit= ");
  Serial.println(carOvertime);

  delay(5000); //wait 5 sec

}



