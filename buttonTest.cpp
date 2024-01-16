#include "buttonTest.h"
#include "pigpio.h"


#define BUTTON_PIN 17 // GPIO pin connected to the button
#define LED_PIN 18    // GPIO pin connected to the LED
buttonTest::buttonTest() {}

void buttonTest::run(){
    buttonTest::testingFunction();
}

void buttonTest::buttonCallback(int gpio, int level, uint32_t tick) {
    if (level == 0) { // Button is pressed (active low)
        cout << "Button pressed! LED will start blinking." << endl;

        // Start blinking the LED
        gpioPWM(LED_PIN, 128); // 50% duty cycle
    } else { // Button is released
        cout << "Button released! LED will stop blinking." << endl;

        // Stop the LED
        gpioPWM(LED_PIN, 0);
    }
}

int buttonTest::testingFunction(){
    if (gpioInitialise() < 0) {
        cerr << "pigpio initialization failed." << endl;
        return 1;
    }

    // Set up the button pin as an input with a pull-up resistor
    gpioSetMode(BUTTON_PIN, PI_INPUT);
    gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP);

    // Set up the LED pin as an output
    gpioSetMode(LED_PIN, PI_OUTPUT);

    // Set up a callback for button events
    gpioSetAlertFunc(BUTTON_PIN, buttonTest::buttonCallback);

    // Run the program until the user presses Enter
    cout << "Press Enter to exit." << endl;
    cin.get();

    // Clean up
    gpioTerminate();

    return 0;
}
