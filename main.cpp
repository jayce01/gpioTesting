#include <iostream>
#include "pigpio.h"

#define BUTTON_PIN 23

int main() {
    // Initialize pigpio library
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
        return 1;
    }

    // Set up the button pin as an input with a pull-up resistor
    gpioSetMode(BUTTON_PIN, PI_INPUT);
    gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP);

    // Main loop to read button state
    std::cout << "Press Ctrl+C to exit." << std::endl;
    while (true) {
        // Read the button state (1 for released, 0 for pressed)
        int buttonState = gpioRead(BUTTON_PIN);

        // Display the button state
        std::cout << "Button State: " << buttonState << std::endl;

        // Add a delay to avoid excessive readings
        time_sleep(0.1);
    }

    // Clean up pigpio resources
    gpioTerminate();

    return 0;
}
