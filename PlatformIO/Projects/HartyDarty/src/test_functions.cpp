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

