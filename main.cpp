#include <iostream>
#include "pigpio.h"
#include <unistd.h>
#include <cstring>

#define LCD_ADDR 0x27
#define LCD_BUS 1

// Control byte to turn on/off the backlight
#define LCD_BACKLIGHT_ON 0x08
#define LCD_BACKLIGHT_OFF 0x00

int lcdHandle = -1;

void lcdPrint(const std::string& message, int row, int col) {
    if (lcdHandle >= 0) {
        char buf[32];
        snprintf(buf, sizeof(buf), "\x1B[%d;%dH%s", row + 1, col + 1, message.c_str());
        int result = i2cWriteI2CBlockData(lcdHandle, 0x80, buf, strlen(buf));
        if (result < 0) {
            std::cerr << "Error writing to LCD: " << std::endl;
        }
    }
}

void initializeLCD() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
        return;
    }

    lcdHandle = i2cOpen(LCD_BUS, LCD_ADDR, 0);
    if (lcdHandle < 0) {
        std::cerr << "Unable to open I2C communication with LCD: "  << std::endl;
        gpioTerminate();
        return;
    }

    // Turn on the backlight
    i2cWriteByte(lcdHandle, LCD_BACKLIGHT_ON);
}

void closeLCD() {
    if (lcdHandle >= 0) {
        int result = i2cClose(lcdHandle);
        if (result != 0) {
            std::cerr << "Error closing I2C communication: " <<  std::endl;
        }
        gpioTerminate();
    }
}

int main() {
    initializeLCD();
    time_sleep(1);

    lcdPrint("Hello, World!", 0, 0);
    time_sleep(20);

    // Turn off the backlight before closing
    i2cWriteByte(lcdHandle, LCD_BACKLIGHT_OFF);
    closeLCD();

    return 0;
}
