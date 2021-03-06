/*
  Autor : Johan Ramirez
  date  : 2/05/2020
  Description : SPI ABP sensor reading is made by SPI communication, it values
              changes from sensor to sensor depending the references, so you have to change the
              values to ajust sensor you have.
  Sensor used : ABP-D-JJ-T-001PD-S-A3
  Min pressure : 0 psi
  max pressure : +- 1 psi
  Tranfer Function : 10% to 90% of Vsupply
  Voltaje supply : 3.3 V
  Pressure type : Differential
*/

#include <SPI.h>
#define slaveAPin 53

double tranferFuntion(int16_t data);

// set up the speed, data order and data mode
SPISettings settingsA(425000, MSBFIRST, SPI_MODE0);


short data;
int setting_offset = 14;                          //offset in bytes

void setup() {
  // set the Slave Select Pins as outputs:
  pinMode (slaveAPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
  // Inicializacion Serial
  Serial.begin(9600);
}

void loop() {
  SPI.beginTransaction(settingsA);
  digitalWrite (slaveAPin, LOW);
  data = SPI.transfer16(0x00);
  digitalWrite (slaveAPin, HIGH);
  SPI.endTransaction();
  Serial.println(String(tranferFuntion(data))+ " [psi]");
  delay(20);
}

double tranferFuntion(int16_t data) {
  if (data <= 16384 * 0.1) return -1;
  else if (data >= 16384 * 0.9) return 1;
  else return (data - 0.1 * 16384 ) / (0.4 * 16384) - 1;
}
