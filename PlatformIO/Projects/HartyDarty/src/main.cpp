#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <MS5611.h>
#include <test_functions.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
// File system 
#include <LittleFS.h>
#include "FS.h"

// Set these to your desired credentials.
  const char *ssid = "XIAO_ESP32S3";
  const char *password = "password";

  WiFiServer server(80);

// LSM6DSO32 sensor
// For SPI mode, we need a CS pin
#define LSM_CS 44

//check that all components are up and running
Adafruit_LSM6DSO32 dso32;

// Set I2C adress for barometer 
MS5611 MS5611(0x77);

// Define pins for continuity testing
// Ig for ignition wires and cont for continuity wires
#define ig1 1
#define cont1 2
#define ig2 3
#define cont2 4
#define ig3 9
#define cont3 8

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // All sensor initializations offloaded to 
  sensor_init(dso32,MS5611);

  // Setup PinModes for continuity testing
  // Setting low for continuity testing
  // Mostfet 1
  analogSetPinAttenuation(cont1,ADC_11db);
  pinMode(ig1,OUTPUT);
  pinMode(cont1,INPUT);
  digitalWrite(ig1,LOW); // Sets mosfet, LOW means off, HIGH means on
  float ADC = 0;
  // Mostfet 2
  analogSetPinAttenuation(cont2,ADC_11db);
  pinMode(ig2,OUTPUT);
  pinMode(cont2,INPUT);
  digitalWrite(ig2,LOW); // Sets mosfet, LOW means off, HIGH means on
  // Mosfet 3
  analogSetPinAttenuation(cont3,ADC_11db);
  pinMode(ig3,OUTPUT);
  pinMode(cont3,INPUT);
  digitalWrite(ig3,LOW); // Sets mosfet, LOW means off, HIGH means on

  // WiFi Setup -----------------
  WiFiServer server(80);

  //get call for wifi setup
  //hostname: esp32s3-000000
  /*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
    OR
    Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
  */

  //#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
  
}

void loop() {
  //data_print_test(dso32,MS5611,1);

  // Turn the GPIO ports for ignition and continuity into integer arrays for input to function
  int ig[3]={ig1,ig2,ig3};
  int cont[3]={cont1,cont2,cont3};
  continuity_test(0,ig,cont);


  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, LOW);                 // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, HIGH);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}