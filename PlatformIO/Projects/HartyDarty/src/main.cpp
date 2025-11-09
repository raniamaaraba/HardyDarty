#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <MS5611.h>
#include <test_functions.h>
#include <WiFi.h>
#include <PubSubClient.h>
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

  // Start Access Point (local-only WiFi)
  WiFi.softAP(ssid, password);
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);

  // Start web server
  server.begin();
  Serial.println("Server started");

  
}
      

void loop() {
  //data_print_test(dso32,MS5611,1);

  // Turn the GPIO ports for ignition and continuity into integer arrays for input to function
  int ig[3]={ig1,ig2,ig3};
  int cont[3]={cont1,cont2,cont3};
  continuity_test(0,ig,cont);


  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n') {
          //this is where you modify the website
          if (currentLine.length() == 0) {
            // Send HTML response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html><body>");
            client.println("<h2>ESP32 Local Control</h2>");
            client.println("<a href=\"/H\">Turn ON LED</a><br>");
            client.println("<a href=\"/L\">Turn OFF LED</a><br>");
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Handle LED control
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, LOW); // ON
          Serial.println("LED turned ON");
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, HIGH); // OFF
          Serial.println("LED turned OFF");
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
  
}