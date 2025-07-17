#ifndef I2C_INIT_H
#define I2C_INIT_H

#include <Wire.h>

void initI2CBuses(); // Function to initialize I2C buses
void initSlave();
void onReceive(int len);
void onRequest();
void testSlave();
String getMessageFromSlave(uint8_t addr, uint8_t len);

#define I2C_DEV_ADDR 0x55

#endif