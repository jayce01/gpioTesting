#include "buttonTest.h"
#include "pigpio.h"

#define BUTTON_PIN 23 // GPIO pin connected to the button
#define LED_PIN 24    // GPIO pin connected to the LED

buttonTest::buttonTest() {}

void buttonTest::run() {
    if (gpioInitialise() < 0) {
        cerr << "pigpio initialization failed." << endl;
        return;
    }

    // Set up the button pin as an input with a pull-up resistor
    gpioSetMode(BUTTON_PIN, PI_INPUT);
    gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP);

    // Set up the LED pin as an output
    gpioSetMode(LED_PIN, PI_OUTPUT);

    // Run the loop until the user presses Enter
    cout << "Press Enter to exit." << endl;
    int buttonStatus = 0;
    while (cin.get() != '\n') {
        // Check the button status
        buttonStatus = gpioRead(BUTTON_PIN);
        if(buttonStatus == 0){
            cout << "not pressed" << endl;
        }

        // Button is pressed (active low)
        if (buttonStatus == PI_LOW) {
            cout << "Button pressed! LED will start blinking." << endl;

            // Start blinking the LED
            gpioPWM(LED_PIN, 128); // 50% duty cycle
        } else { // Button is released
            cout << "Button released! LED will stop blinking." << endl;

            // Stop the LED
            gpioPWM(LED_PIN, 0);
        }
    }

    // Clean up
    gpioTerminate();
}
