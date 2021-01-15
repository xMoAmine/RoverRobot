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
int fd;

    fd=wiringPiI2CSetup(0x70);

    usleep(500000);  //delay 0,5 seconds



    double afstand;



while(1){

    wiringPiI2CWriteReg8(fd, 0x00, 0x51);
    usleep(50000);

    afstand = wiringPiI2CReadReg16(fd, 2);

    afstand = afstand / 225;


    printf("%f \n", afstand);
}
;







}