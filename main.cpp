#include <iostream>
#include "pigpio.h"
#include <unistd.h>
#include <cstring>

#define LCD_ADDR 0x27
#define LCD_BUS 1

int lcdHandle = -1;

void lcdPrint(const std::string& message, int row, int col) {
    if (lcdHandle >= 0) {
        char buf[32];
        snprintf(buf, sizeof(buf), "\x1B[%d;%dH%s", row + 1, col + 1, message.c_str());
        i2cWriteI2CBlockData(lcdHandle, 0x80, buf, strlen(buf));
    }
}

void initializeLCD() {
    if (gpioInitialise() < 0) {
        std::cout << "pigpio initialization failed." << std::endl;
        return;
    }

    lcdHandle = i2cOpen(LCD_BUS, LCD_ADDR, 0);
    if (lcdHandle < 0) {
        std::cout << "Unable to open I2C communication with LCD." << std::endl;
        gpioTerminate();
        return;
    }
}

void closeLCD() {
    if (lcdHandle >= 0) {
        i2cClose(lcdHandle);
        gpioTerminate();
    }
}

int main() {
    initializeLCD();
    time_sleep(1);

    // Clear screen command (0x01)
    i2cWriteByte(lcdHandle, 0x01);
    time_sleep(1);  // Wait for the LCD to process the command

// Set cursor position to the beginning of the first line
    i2cWriteByte(lcdHandle, 0x80);
    time_sleep(1);  // Wait for the LCD to process the command


    lcdPrint("Hello, World!", 0, 0);
    time_sleep(3);

    //closeLCD();

    return 0;
}
