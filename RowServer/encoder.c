#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <time.h>

#define ENCODER 7
#define PI 3.14

int main() {


    wiringPiSetup();

    pinMode(ENCODER, INPUT);

    int input;
    input = 0;

    wiringPiSetup();

    int program_done = FALSE;
    time_t start;



    int counter = 0;
    start = time(NULL);
    while (time(NULL) - start < 60 ) {

        input = digitalRead(7);

        if (input == 0) {
            counter += 1;
        }


    }
    double eenGatinMeters = 0.010995575;
    double speed;
    speed = (counter * eenGatinMeters)/60;



    printf("%f M/S", speed);

}

