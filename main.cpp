#include "pigpio.h"
#include <stdio.h>
#include <stdlib.h>

// Define some device parameters
#define I2C_ADDR   0x27 // I2C device address

// Define some device constants
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line
#define LINE3  0x94 // 3rd line
#define LINE4  0xD4 // 4th line

#define LCD_BACKLIGHT_ON   0x08  // On
#define LCD_BACKLIGHT_OFF  0x00  // Off

#define ENABLE  0b00000100 // Enable bit

void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);
void lcd_backlight(int state);

// added by Lewis
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); //move cursor
void ClrLcd(void); // clr LCD return home
void typeln(const char *s);
void typeChar(char val);
int fd;  // seen by all subroutines

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed.\n");
        exit(1);
    }

    fd = i2cOpen(1, I2C_ADDR, 0);

    lcd_init(); // setup LCD

    char array1[] = "Hello world!";

    while (1) {
        lcd_backlight(LCD_BACKLIGHT_ON);

        lcdLoc(LINE1);
        typeln("Pinche Leo ojala");
        lcdLoc(LINE2);
        typeln("te coma el cuero");

        gpioDelay(2500000); // 2.5s
        ClrLcd();
        lcdLoc(LINE1);
        typeln("en el pantano");
        lcdLoc(LINE2);
        typeln("al lado de shrek");

        gpioDelay(2500000);
        ClrLcd();
        lcdLoc(LINE3);
        typeln("Jila y la");
        lcdLoc(LINE4);
        typeln("Donkey Kong-C++");

        gpioDelay(2500000);
        ClrLcd();
        lcdLoc(LINE1);
        typeln(array1);

        gpioDelay(2500000);
        ClrLcd(); // defaults LINE1
        typeln("Int  ");
        int value = 20125;
        typeInt(value);

        gpioDelay(2500000);
        lcdLoc(LINE2);
        typeln("Float ");
        float FloatVal = 10045.25989;
        typeFloat(FloatVal);
        gpioDelay(2500000);

        lcd_backlight(LCD_BACKLIGHT_OFF);
    }

    i2cClose(fd);
    gpioTerminate();

    return 0;
}

// float to string
void typeFloat(float myFloat) {
    char buffer[20];
    sprintf(buffer, "%4.2f",  myFloat);
    typeln(buffer);
}

// int to string
void typeInt(int i) {
    char array1[20];
    sprintf(array1, "%d",  i);
    typeln(array1);
}

// clr lcd go home loc 0x80
void ClrLcd(void) {
    lcd_byte(0x01, LCD_CMD);
    lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void lcdLoc(int line) {
    lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void typeChar(char val) {
    lcd_byte(val, LCD_CHR);
}

// this allows use of any size string
void typeln(const char *s) {
    while (*s)
        lcd_byte(*(s++), LCD_CHR);
}

void lcd_byte(int bits, int mode) {
    // Send byte to data pins
    // bits = the data
    // mode = 1 for data, 0 for command
    int bits_high;
    int bits_low;
    // uses the two half-byte writes to LCD
    bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT_ON;
    bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT_OFF;
    if (mode == LCD_CMD) {
        bits_high &= ~LCD_BACKLIGHT_ON;
        bits_low &= ~LCD_BACKLIGHT_OFF;
    }

    // High bits
    i2cWriteByteData(fd, bits_high, 0);
    lcd_toggle_enable(bits_high);

    // Low bits
    i2cWriteByteData(fd, bits_low, 0);
    lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits) {
    // Toggle enable pin on LCD display
    gpioDelay(500);
    i2cWriteByteData(fd, (bits | ENABLE), 0);
    gpioDelay(500);
    i2cWriteByteData(fd, (bits & ~ENABLE), 0);
    gpioDelay(500);
}

void lcd_backlight(int state) {
    // Control the backlight of the LCD
    i2cWriteByteData(fd, state, 0);
}

void lcd_init() {
    // Initialise display
    lcd_byte(0x33, LCD_CMD); // Initialise
    lcd_byte(0x32, LCD_CMD); // Initialise
    lcd_byte(0x06, LCD_CMD); // Cursor move direction
    lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
    lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD); // Clear display
    gpioDelay(500);
}
