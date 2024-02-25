#include "pigpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define I2C_ADDR 0x27  // Replace with your LCD's I2C address
using namespace std;

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed!" << std::endl;
        return 1;
    }

    int handle = i2cOpen(1, I2C_ADDR, 0);

    if (handle < 0) {
        std::cerr << "I2C initialization failed!" << std::endl;
        gpioTerminate();
        return 1;
    }

    i2cWriteByte(handle, 0x01);  // Clear display
    time_sleep(0.1);  // Wait for the display to clear

    i2cWriteByte(handle, 0x08 | 0x04);  // Display on, no cursor
    i2cWriteByte(handle, 0x28);  // 4-bit mode, 2 lines, 5x8 font

    i2cWriteByte(handle, 0x80);  // Set cursor to beginning of first line

    i2cWriteByte(handle, 'H');
    i2cWriteByte(handle, 'e');
    i2cWriteByte(handle, 'l');
    i2cWriteByte(handle, 'l');
    i2cWriteByte(handle, 'o');

    time_sleep(2);  // Display for 2 seconds

    i2cWriteByte(handle, 0x01);  // Clear display
    time_sleep(0.1);

    i2cClose(handle);
    gpioTerminate();

    return 0;
}