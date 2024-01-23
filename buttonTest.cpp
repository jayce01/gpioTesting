#include "buttonTest.h"
#include "pigpio.h"
//test
#define BUTTON_PIN 23 // GPIO pin connected to the button
#define LED_PIN 24    // GPIO pin connected to the LED

buttonTest::buttonTest() {}

void buttonTest::run() {
    // Initialize pigpio library
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
    }

    // Set up the button pin as an input with a pull-up resistor
    gpioSetMode(BUTTON_PIN, PI_INPUT);
    gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP);
    gpioSetMode(LED_PIN, PI_OUTPUT);

    // Main loop to read button state
    std::cout << "Press Ctrl+C to exit." << std::endl;
    while (true) {
        // Read the button state (1 for released, 0 for pressed)
        int buttonState = gpioRead(BUTTON_PIN);

        // Display the button state
        std::cout << "Button State: " << buttonState << std::endl;
        if(buttonState == 1){
            gpioPWM(LED_PIN, 0);
        }
        else{
            gpioPWM(LED_PIN, 128);
        }

        // Add a delay to avoid excessive readings
        time_sleep(0.1);
    }

    // Clean up pigpio resources
    gpioTerminate();
}

