#include <Arduino.h>
#include <MS5611.h>
#include <Adafruit_LSM6DSO32.h>


void data_print_test(Adafruit_LSM6DSO32& IMU, MS5611& BARO,int plot=1){ // & after class declaration determines how to pass, where & means by reference
    if (plot==0) {
        //  /* Get a new normalized sensor event */
        sensors_event_t accel;
        sensors_event_t gyro;
        sensors_event_t temp;
        IMU.getEvent(&accel, &gyro, &temp);

        Serial.print("\t\tTemperature ");
        Serial.print(temp.temperature);
        Serial.println(" deg C");

        /* Display the results (acceleration is measured in m/s^2) */
        Serial.print("\t\tAccel X: ");
        Serial.print(accel.acceleration.x);
        Serial.print(" \tY: ");
        Serial.print(accel.acceleration.y);
        Serial.print(" \tZ: ");
        Serial.print(accel.acceleration.z);
        Serial.println(" m/s^2 ");

        /* Display the results (rotation is measured in rad/s) */
        Serial.print("\t\tGyro X: ");
        Serial.print(gyro.gyro.x);
        Serial.print(" \tY: ");
        Serial.print(gyro.gyro.y);
        Serial.print(" \tZ: ");
        Serial.print(gyro.gyro.z);
        Serial.println(" radians/s ");
        Serial.println();
    } else if (plot==1) {
        // serial plotter friendly format - Works w/ VS Code extension Serial Plotter
        // Hit Control(cmd)+Shift+P and type: "Serial Plotter: Open pane" into window and hit enter to open plotter

        /* Get a new normalized sensor event */
        sensors_event_t accel;
        sensors_event_t gyro;
        sensors_event_t temp;
        IMU.getEvent(&accel, &gyro, &temp);

        Serial.print('>');
        
        Serial.print("Ax:");
        Serial.print(accel.acceleration.x);
        Serial.print(',');

        Serial.print("Ay:");
        Serial.print(accel.acceleration.y);
        Serial.print(',');

        Serial.print("Az:");
        Serial.print(accel.acceleration.z);
        Serial.print(',');

        Serial.print("Gx:");
        Serial.print(gyro.gyro.x);
        Serial.print(',');

        Serial.print("Gy:");
        Serial.print(gyro.gyro.y);
        Serial.print(',');

        Serial.print("Gz:");
        Serial.print(gyro.gyro.z);
        Serial.print(',');

        BARO.read();

        Serial.print("Pressure:");
        Serial.print(BARO.getPressure(),2); // Pressure is in mBar so values near 999 are close to sea level atmospheric pressure
        Serial.print(',');

        Serial.print("Temp:");
        Serial.println(BARO.getTemperature(),2);

        delayMicroseconds(10000);
        }
}


// Function for varied continuity testing, NOT final function for reading continuity during flight
// Original code by Jessie, changes and conversion to function by Loring T
// Input descriptions:
// NOTE: Pass GPIO pins as integer array, for example declare ig[3]={ig1,ig2,ig3}, where ig1-ig3 are GPIO pins for igniters, and pass just ig (the array) to function
// mode: integer that determines how the test behaves - not currently implimented 
// ig: integer array that stores GPIO pins for igniters 
// cont: integer array that stores GPIO pins for continuity test (analog) lines
void continuity_test(int mode, int ig[3], int cont[3]){
    float ADC1 = analogRead(cont[0]);
    float Vout1 = ADC1* (3.3/4095.0);
    float Vigniter1 = Vout1*2;

    float ADC2 = analogRead(cont[1]);
    float Vout2 = ADC2* (3.3/4095.0);
    float Vigniter2 = Vout2*2;

    float ADC3 = analogRead(cont[2]);
    float Vout3 = ADC3* (3.3/4095.0);
    float Vigniter3 = Vout3*2;

    if (mode==0){
        if (Vigniter1 >= 3.2) {
            Serial.print("Continuity 1 Good: ");
        }
        if (Vigniter1 >= 3.6) {
            Serial.print("Good Voltage 1: ");
        } else {
            Serial.print("Low Battery 1: ");
        }
        Serial.print(Vigniter1); Serial.print(" ");

        if (Vigniter2 >= 3.2) {
            Serial.print("Continuity 2 Good: ");
        }
        if (Vigniter2 >= 3.6) {
            Serial.print("Good Voltage 2: ");
        } else {
            Serial.print("Low Battery 2: ");
        }
        Serial.print(Vigniter2); Serial.print(" ");

        if (Vigniter3 >= 3.2) {
            Serial.print("Continuity 3 Good: ");
        }
        if (Vigniter3 >= 3.6) {
            Serial.print("Good Voltage 3: ");
        } else {
            Serial.print("Low Battery 3: ");
        }
        Serial.println(Vigniter3);
        delay(1000);
    } else 
    // Sequential Blink Mode: Turns each LED on and off in sequence
    // DO NOT USE WITH ANY LIVE IGNITERS, WILL IGNITE AS SOON AS PROGRAM IS FLASHED!
    {
        for (int i=0; i<3; i++) {
            delay(1000);
            digitalWrite(ig[i],HIGH);
            delay(1000);
            digitalWrite(ig[i],LOW);
        }
    }
}