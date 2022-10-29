//#include <sim900.h> //download library from https://github.com/Seeed-Studio/GPRS_SIM900
#include <SoftwareSerial.h> //default library

int Incomingch;
String data, Fdata;
char call;

const int dtmf_pin1 = A7; //2;
const int dtmf_pin2 = A6; //3;
const int dtmf_pin3 = A5; //4;
const int dtmf_pin4 = A4; //5;

int d1, d2, d3, d4;

//Connect Tx pin of GSM to 9 of Arduino
//Connect Rx pin of GSM to 10 of Arduino
//SoftwareSerial mySerial( 12, 13);
SoftwareSerial gsm(9, 10); //TX,RX


void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  digitalWrite(2, 0);
  digitalWrite(3, 1);

  Serial.begin(9600);
  gsm.begin(9600);

  autocall_disconnect();
}

void loop()
{
  call_auto_connect();

}


void autocall_disconnect() {
  gsm.println ( "ATD +918209829808;" ); // Cellular
  gsm.println ();

  delay(10000);// wait for 10 seconds …

  gsm.println ( "ATH" ); // Drop the phone

  delay(1000);

}

void call_auto_connect() {
  //  status_leds(19);


  if (gsm.available() > 0)
  {
    check_Incoming();
    if (Fdata == "RING") {
      gsm.println("ATA");
      delay(1000);



      while (Fdata != "NO CARRIER") {
        check_Incoming();
        //        delay(3000);

        GsmWeightUpdate();
      }
      delay(500);

    }
  }


}

void GsmWeightUpdate() {
  int temp_update_weight = 0;
  d1 = analogRead(dtmf_pin1);
  d2 = analogRead(dtmf_pin2);
  d3 = digitalRead(dtmf_pin3);
  d4 = digitalRead(dtmf_pin4);

  if (d1 > 511) {
    d1 = 1;
  }
  else {
    d1 = 0;
  }
  if (d2 > 511) {
    d2 = 1;
  }
  else {
    d2 = 0;
  }
  //    d1=1,d2=0,d3=0,d4=0;
  //measure weight here


  if ((d1 == 1) && (d2 == 0) && (d3 == 0) && (d4 == 0)) { // one
    digitalWrite(2, 0);

  }
  else if ((d1 == 0) && (d2 == 1) && (d3 == 0) && (d4 == 0)) { //two
    digitalWrite(2, 1);

  }
  else if ((d1 == 1) && (d2 == 1) && (d3 == 0) && (d4 == 0)) { //three
    digitalWrite(3, 0);
  }
  else if ((d1 == 0) && (d2 == 0) && (d3 == 1) && (d4 == 0)) { //four
    digitalWrite(3, 1);
  }
  else {
    digitalWrite(2, 0);
    digitalWrite(3, 0);

  }
  delay(100);


  check_Incoming();



}



void check_Incoming() {

  if (gsm.available()) { //If GSM is saying something

    Incomingch = gsm.read(); // Listen to it and store in this variable
    if (Incomingch == 10 || Incomingch == 13) //If it says space (10) or Newline (13) it means it has completed one word
    {
      //Serial.println(data);   //Print the word and clear the variable to start fresh
      Fdata = data;
      data = "";
    }
    else
    {
      String newchar = String (char(Incomingch)); //convert the char to string by using string objects
      data = data + newchar; // After converting to string, do string concatenation
    }
  }
}
