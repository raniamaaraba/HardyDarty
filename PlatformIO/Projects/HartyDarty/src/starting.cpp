//IF YOU ARE USING CLANG MAKE SURE YOU DISABLE IT OTHERWISE THERE ARE A LOT OF EXTRA WARNING FILES
// :)

#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <MS5611_SPI.h>



#define LSM_CS 10
#define LSM_SCK 13
#define LSM_MISO 12
#define LSM_MOSI 11


//check if devices are on
Adafruit_LSM6DSO32 dso32;
void setup(void) {
    Serial.begin(115200);
    while (!Serial)
        delay(10); // will pause Zero, Leonardo, etc until serial console opens

    //ms tests
    Serial.println();
    Serial.println(__FILE__);
    Serial.print("MS5611_SPI_LIB_VERSION: ");
    Serial.println(MS5611_SPI_LIB_VERSION);
    Serial.println();

    Serial.println("Adafruit LSM6DSO32 test!");

    if (!dso32.begin_I2C()) {
        if (!dso32.begin_SPI(LSM_CS)) {
            if (!dso32.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
                Serial.println("Failed to find LSM6DSO32 chip"); //same here what are we doing if fails
            }
        }
    while (1) {
        delay(10);
        }
    }

    Serial.println("LSM6DSO32 Found!");

    dso32.setAccelRange(LSM6DSO32_ACCEL_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");

    switch (dso32.getAccelRange()) {
        case LSM6DSO32_ACCEL_RANGE_4_G:
        Serial.println("+-4G");
        break;

        case LSM6DSO32_ACCEL_RANGE_8_G:
        Serial.println("+-8G");
        break;

        case LSM6DSO32_ACCEL_RANGE_16_G:
        Serial.println("+-16G");
        break;

        case LSM6DSO32_ACCEL_RANGE_32_G:
        Serial.println("+-32G");
        break;
    }

    pinMode(LED_BUILTIN, OUTPUT);

    SPI.begin();

    if (MS5611.begin() == true){
        Serial.println("MS5611 found.");
    }else{
        Serial.println("MS5611 not found. halt."); //need to switch this code for what we are doing if fails
        while (1)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(1000);
            digitalWrite(LED_BUILTIN, LOW);
            delay(1000);
        }
    }
    
    Serial.println();
} 

