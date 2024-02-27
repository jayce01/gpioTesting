from signal import signal, SIGTERM, SIGHUP, pause
from rpi_lcd import LCD

lcd = LCD();

def safe_exit(signum, frame):
    exit(1)

signal(SIGTERM, safe_exit)
signal(SIGHUP, safe_exit)
try:
    lcd.text("hello", 1)
    pause()

except keyboardInterrupt:
    pass

finally:
    lcd.clear()