#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>

int main()

{

    unsigned short int xl, yl, xh, yh;
    short int x,y;    // size of a short is 2 bytes !!
    int fd;
    double angle;

    fd=wiringPiI2CSetup(0x1E);
    usleep(500000);  //delay 0,5 seconds


    wiringPiI2CWriteReg8(fd, 0x00, 0x70);
    wiringPiI2CWriteReg8(fd, 0x01, 0xA0);
    wiringPiI2CWriteReg8(fd, 0x02, 0x01);

    while(1)
    {

        // start a new single measurement
        wiringPiI2CWriteReg8(fd, 0x02, 0x01);
        usleep(500000);  //delay 0,5 seconds

        xh = wiringPiI2CReadReg8(fd, 0x03); // x msb
        xl = wiringPiI2CReadReg8(fd, 0x04); // x lsb
        yh = wiringPiI2CReadReg8(fd, 0x07); // y msb
        yl = wiringPiI2CReadReg8(fd, 0x08); // y lsb


        x = (xh << 8) | xl;
        y = (yh << 8) | yl;


        angle = atan2((double)y, (double)x) * 180 / M_PI;
        if(angle < 0) angle +=360.0;
        printf("Angle is: %f \n",angle);

        usleep(5000000); // delay 5 seconds
    }

    return 0;


}

