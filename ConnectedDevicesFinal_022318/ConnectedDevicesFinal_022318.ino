/*
Author: Morgan Mueller
Connected Devices Final Project 2/25/19
*/


#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"
#include <LiquidCrystal.h>


char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char sessionKey[] = SECRET_KEY;
String MAC = SECRET_MAC;

//server info
const char serverAddress[] = "tigoe.io";  // server address
String route = "/data";
// set the content type and fill in the POST data:
String contentType = "application/json";
int port = 443;
int sensorPin = A1;

//LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int ipPin = 0;


//temperature values
String newPost, tempData;

unsigned long previousMillis = 0;        // will store last time LED was updated

const unsigned long interval = 3600000;



byte mac[6];
WiFiSSLClient wifi;
HttpClient client(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);              // initialize serial communication

  lcd.begin(16, 2); //define the LCD's size

  // while you're not connected to a WiFi AP, attempt to connect:
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);           // print the network name (SSID)
    status = WiFi.begin(ssid, pass);  // try to connect

  }

  WiFi.macAddress(mac);

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.print("Mac Address: ");
  Serial.println(MAC);

}

void loop() {
  
  unsigned long currentMillis = millis();

  int sensorReading = analogRead(sensorPin);

  float voltageReading = sensorReading * 3.3;
  voltageReading /= 1024.0;

  //temperature in Celcius
  float tempC = (voltageReading - 0.5) * 100;

  //Serial.print(tempC);
  //Serial.println(" degrees C");
  tempData = String(tempC);



  //ensure that the temperature is only sent to the server once an hour

  if (currentMillis - previousMillis >= interval) {
    Serial.println("sending next post in 60 seconds");

    //client.beginRequest();
    postData(tempData);
    //client.endRequest();
    previousMillis = currentMillis;


    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();


    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
  

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current Temp");
    lcd.setCursor(0, 1);

    lcd.print(tempC);
    lcd.print(" Celsius");


  }


}

//Function to format mac address, session key, and temperature into the proper json format to be received by the server
void postData(String newData) {

  String tempString = "\"{\'temperature\':";
  //tempString = "{\"Temperature\":\"";
  tempString += tempData;
  tempString += "}\"";

  newPost = "{\"macAddress\":\"";
  newPost += MAC;
  newPost += "\", \"sessionKey\":\"";
  newPost += sessionKey;
  newPost += "\", \"data\": ";
  newPost += tempString;
  newPost += "}";

  Serial.println(newPost);
  client.post(route, contentType, newPost);

}

String macToString(byte mac[]) {
  String result;
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      result += "0";
    }
    result += String(mac[i], HEX);
    if (i > 0)  result += ":";
  }
  return result;
}
