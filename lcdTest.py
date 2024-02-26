import smbus2
import time

# Define I2C parameters
I2C_ADDR = 0x27  # I2C address of the LCD
I2C_BUS = 1      # I2C bus number (1 for Raspberry Pi 3, 0 for Raspberry Pi 1 and 2)

# LCD commands
LCD_CLEAR = 0x01
LCD_RETURN_HOME = 0x02
LCD_ENTRY_MODE_SET = 0x04
LCD_DISPLAY_CONTROL = 0x08
LCD_CURSOR_SHIFT = 0x10
LCD_FUNCTION_SET = 0x20
LCD_SET_CGRAM_ADDR = 0x40
LCD_SET_DDRAM_ADDR = 0x80

# Flags for display entry mode
LCD_ENTRY_RIGHT = 0x00
LCD_ENTRY_LEFT = 0x02
LCD_ENTRY_SHIFT_INCREMENT = 0x01
LCD_ENTRY_SHIFT_DECREMENT = 0x00

# Flags for display on/off control
LCD_DISPLAY_ON = 0x04
LCD_DISPLAY_OFF = 0x00
LCD_CURSOR_ON = 0x02
LCD_CURSOR_OFF = 0x00
LCD_BLINK_ON = 0x01
LCD_BLINK_OFF = 0x00

# Flags for function set
LCD_8BIT_MODE = 0x10
LCD_4BIT_MODE = 0x00
LCD_2LINE = 0x08
LCD_1LINE = 0x00
LCD_5x10_DOTS = 0x04
LCD_5x8_DOTS = 0x00

# Flags for backlight control
LCD_BACKLIGHT_ON = 0x08
LCD_BACKLIGHT_OFF = 0x00

class LCD:
    def __init__(self, addr, bus):
        self.addr = addr
        self.bus = smbus2.SMBus(bus)
        self.backlight_state = LCD_BACKLIGHT_ON

        self._init_lcd()

    def _init_lcd(self):
        self._send_command(LCD_FUNCTION_SET | LCD_4BIT_MODE | LCD_2LINE | LCD_5x8_DOTS)
        self._send_command(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF)
        self._send_command(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT)
        self.clear()

    def _send_command(self, cmd):
        self.bus.write_byte(self.addr, cmd | self.backlight_state)
        time.sleep(0.0001)

    def _send_data(self, data):
        self.bus.write_byte(self.addr, data | LCD_BACKLIGHT_ON)
        time.sleep(0.0001)

    def clear(self):
        self._send_command(LCD_CLEAR)
        time.sleep(2)

    def return_home(self):
        self._send_command(LCD_RETURN_HOME)
        time.sleep(2)

    def set_cursor(self, row, col):
        offsets = [0x00, 0x40, 0x14, 0x54]
        self._send_command(LCD_SET_DDRAM_ADDR | (col + offsets[row]))

    def write_string(self, string):
        for char in string:
            self._send_data(ord(char))

    def set_backlight(self, state):
        if state:
            self.backlight_state = LCD_BACKLIGHT_ON
        else:
            self.backlight_state = LCD_BACKLIGHT_OFF
        self._send_command(0)

if __name__ == "__main__":
    lcd = LCD(I2C_ADDR, I2C_BUS)
    try:
        lcd.write_string("Hello, World!")
        time.sleep(2)
        lcd.clear()
        lcd.write_string("LCD on Raspberry Pi")
        time.sleep(2)
        lcd.clear()
    except KeyboardInterrupt:
        pass
    finally:
        lcd.clear()
        lcd.set_backlight(False)
