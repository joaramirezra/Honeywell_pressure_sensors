/*
	Arduino library for Honeywell pressure sensors
	SPI Models supported, I2C and Analog support to come
	Fletcher Bach - Dec. 2016
	MIT license
*/

#include "Arduino.h"
#include "Honeywell_pressure_sensors.h"
#include <SPI.h>

HPS::HPS(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    _pin = pin;
}

float HPS::readPressure() {
    int whichChip = _pin;

    byte firstByte;
    byte secondByte;

    // asserting this sensor by bringing CS pin low
    digitalWrite(whichChip, LOW);

    // here we're asking for our two bytes
    firstByte = SPI.transfer(0x00);
    secondByte = SPI.transfer(0x00);

    // here we're bitshifting the first byte 8 bits to the left
    uint16_t bothBytes = (firstByte << 8) | secondByte;
    float psiOutput = transferFunction(bothBytes);

    // de-asserting this sensor by bringing CS pin high
    digitalWrite(whichChip, HIGH);
    //return (bothBytes);
    return psiOutput;
}

void HPS::readTemperature(){

}

void HPS::getStatus(){

}

float HPS::transferFunction(uint16_t dataIn) {
    float outputMax = 16384.0; // 2 to the 14th power (from sensor's data sheet)
    float outputMin = 0.0;
    float pressureMax = 30.0; // max 30 psi (from sensor's datea sheet)
    float pressureMin = 0.0;
    float pressure = 0.0;

    // transfer function: using sensor output to solve for pressure
    pressure = (dataIn - outputMin) * (pressureMax - pressureMin) / (outputMax - outputMin);
    pressure = pressure + pressureMin;

    Serial.print(pressure);
    return (pressure);
}


