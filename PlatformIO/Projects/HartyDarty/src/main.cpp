#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <MS5611.h>
#include <test_functions.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
// File system setup
#include <LittleFS.h>
#include "FS.h"

// Formats file system if not already formatted
#define FORMAT_LITTLEFS_IF_FAILED true


// Set these to your desired credentials.
const char *ssid = "XIAO_ESP32S3";
const char *password = "password";

WiFiServer server(80);

//delays for file writing
unsigned long startTime = 0;
unsigned long lastWriteTime = 0;
const unsigned long writeInterval = 20000; // 20 seconds
const unsigned long runDuration = 120000;  // 2 minutes
bool loggingActive = true;
bool startTimeLogged = false;



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

// LittleFS functions - Will be moved out of main later!
#define FORMAT_LITTLEFS_IF_FAILED true

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("- failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\r\n", path);
  if(fs.remove(path)){
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path){
  Serial.printf("Testing file I/O with %s\r\n", path);

  static uint8_t buf[512];
  size_t len = 0;
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }

  size_t i;
  Serial.print("- writing" );
  uint32_t start = millis();
  for(i=0; i<2048; i++){
    if ((i & 0x001F) == 0x001F){
      Serial.print(".");
    }
    file.write(buf, 512);
  }
  Serial.println("");
  uint32_t end = millis() - start;
  Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
  file.close();

  file = fs.open(path);
  start = millis();
  end = start;
  i = 0;
  if(file && !file.isDirectory()){
    len = file.size();
    size_t flen = len;
    start = millis();
    Serial.print("- reading" );
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      if ((i++ & 0x001F) == 0x001F){
        Serial.print(".");
      }
      len -= toRead;
      }
    Serial.println("");
    end = millis() - start;
    Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
    file.close();
  } else {
    Serial.println("- failed to open file for reading");
  }
}

void setup(void) {
  startTime = millis();

  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // Initialize & Format file system
  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LittleFS Mount Failed");
    return;
  }

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
  //Serial.begin(115200);
  Serial.println();

  // Start Access Point (local-only WiFi)
  WiFi.softAP(ssid, password);
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);

  // Start web server
  server.begin();
  Serial.println("Server started");

  // Create file 
  writeFile(LittleFS,"/data.txt","");
}

void loop() {
  //data_print_test(dso32,MS5611,1);

  // Turn the GPIO ports for ignition and continuity into integer arrays for input to function
  int ig[3]={ig1,ig2,ig3};
  int cont[3]={cont1,cont2,cont3};
  continuity_test(0,ig,cont);


  unsigned long currentTime = millis();

  // Stop logging after 2 minutes
  if (loggingActive && (currentTime - startTime >= runDuration)) {
    loggingActive = false;
    Serial.println("Logging complete after 2 minutes.");
  }

  // Log start time once
  if (loggingActive && !startTimeLogged) {
    File file = LittleFS.open("/data.txt", "w");  // overwrite any previous content
    if (file) {
      time_t now = time(nullptr);  // optional: if RTC or NTP is available
      file.print("Logging started at millis: ");
      file.println(startTime);
      file.close();
      startTimeLogged = true;
      Serial.println("Start time logged.");
    }
  }

  // Log sensor data every 20 seconds
  if (loggingActive && (currentTime - lastWriteTime >= writeInterval)) {
    lastWriteTime = currentTime;

    float temp = MS5611.getTemperature();
    float pressure = MS5611.getPressure();
    //edit this section in the future when we are ACTUALLY logging data


    //
    File file = LittleFS.open("/data.txt", "a");  // append mode
    if (file) {
      file.printf("Temp: %.2f C, Pressure: %.2f mbar\n", temp, pressure);  // each entry on its own line with f
      file.close();
      Serial.println("Logged sensor data.");
    } else {
      Serial.println("Failed to open /data.txt");
    }
  }




  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          //file download for html
          if (currentLine.startsWith("GET /data.txt")) {
            File file = LittleFS.open("/data.txt", "r");
            if (file) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/plain");
              client.println("Content-Disposition: attachment; filename=\"data.txt\"");
              client.println();
              while (file.available()) {
                client.write(file.read());
              }
              file.close();
            } else {
              client.println("HTTP/1.1 404 Not Found");
              client.println("Content-Type: text/plain");
              client.println();
              client.println("File not found");
            }
            break;
          }

          //html edit page
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html><body>");
            client.println("<h2>ESP32 Local Control</h2>");
            client.println("<a href=\"/H\">Turn ON LED</a><br>");
            client.println("<a href=\"/L\">Turn OFF LED</a><br>");
            client.println("<a href=\"/data.txt\" download>Download File</a><br>");
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        //testing with LEDs
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, LOW);
          Serial.println("LED turned ON");
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, HIGH);
          Serial.println("LED turned OFF");
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}