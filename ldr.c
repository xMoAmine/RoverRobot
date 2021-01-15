
#include <wiringPi.h>
#include <stdio.h>
#define LDR 25
#define LED 24
int main() {


    wiringPiSetup();

    pinMode(LDR, INPUT);

    int input;
    wiringPiSetup();

    for (;;) {
        input = digitalRead(25);

        if (input == 1) {
            digitalWrite(LED, HIGH);
            printf("Pin input: (%d) HIGH\n", input);

        }
        if (input == 0) {
            digitalWrite(LED, LOW);
            printf("Pin input: (%d) LOW\n", input);

        }

    }

}
