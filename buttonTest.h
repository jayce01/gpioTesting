#ifndef BUTTONTEST_H
#define BUTTONTEST_H
#include <iostream>
#include <cstdint>

using namespace std;

class buttonTest {
public:
    buttonTest();
    int testingFunction();
    void run();

private:
    static void buttonCallback(int gpio, int level, uint32_t tick); //gpioAlertFunc_t f
};

#endif //BUTTONTEST_H
