#include <iostream>
#include "buttonTest.h"
#include "pigpio.h"

#define BUTTON_PIN 23

int main() {
    buttonTest button;
    button.run();
}
