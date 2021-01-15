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

#define LIGHT 25


int main() {


    wiringPiSetup() ;

    pinMode(LIGHT,INPUT);

    int input;


        input = digitalRead(LIGHT);

        while(1) {
            if (input) {
                printf("dark\n");
            }
        }

};