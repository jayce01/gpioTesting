from signal import signal, SIGTERM, SIGHUP, pause
from rpi_lcd import LCD

lcd = LCD();

def safe_exit(signum, frame):
    exit(1)

signal(SIGTERM, safe_exit)
signal(SIGHUP, safe_exit)
try:
    lcd.text("hello", 1)
    lcd.text("Wow", 2)
    lcd.text("He", 3)
    lcd.text("Ow", 4)
    pause()

except KeyboardInterrupt:
    pass

finally:
    lcd.clear()
