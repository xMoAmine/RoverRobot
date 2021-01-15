// Remote Motor Controller
//@author Boris de Wildt & Justin Smit
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
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <pthread.h>
#include <math.h>
#include <csignal>
#include <time.h>

#define PORT 50000
#define ENCODER 7

typedef  unsigned char uint8_t;

using namespace std;





void *snelheidSense(void *param) {


    int alarm_stop = FALSE;
    unsigned int alarm_period = 30;






}
void *afstandSense(void *param){
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

        usleep(5000000);


    }
}




void *kompasSense(void *param){
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

}



void  Motorinit();

uint8_t MotorHF[7] = {7,3,0xa5,2,3,0xa5,2};
// High speed forward left + right; add explanation
//forward right

uint8_t MotorFR[7] = {7,3,0xa5,2,3,0xa5,1};
uint8_t MotorFL[7] = {7,3,0xa5,1,3,0xa5,2};

uint8_t MotorST[7] = {7,0,0,0,0,0,0};
// Stop left + right; add explanation

uint8_t MotorHR[7] = {7,3,0xa5,1,3,0xa5,1};
//High speed reverse left + right; add explanation

int fd;




void Motorinit()
{
    uint8_t Totalpower[2]={4,250};     // power between 0 and 255
    uint8_t Softstart[3]={0x91,23,0};  // add explanation


    wiringPiSetup();
    pullUpDnControl(0, PUD_DOWN);

    fd = wiringPiI2CSetup(0x32);

    write(fd,&Totalpower[0], 2);
    write(fd,&Softstart[0],3);

    //number of bytes = 3
    //What is a soft start?
}



void forward(){
    write(fd,&MotorHF[0],7);
}

void backward(){
    write(fd,&MotorHR[0],7);
}

void left(){
    write(fd,&MotorFR[0],7);
}

void right(){
    write(fd,&MotorFL[0],7);
}


void stop(){
    write(fd,&MotorST[0],7);
}


template <size_t N>


void splitString(string (&arr)[N], string str){
    int n = 0;
    istringstream iss(str);
    for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < N; ++it, ++n)
        arr[n] = *it;

}

void* socketServer(void *param) {


    pthread_t thread0;
    pthread_t thread1;
    pthread_t thread2;

    socklen_t sockfd, newsockfd, portno, clilen;//socklen_t is een unsigned int van 32 bits
    char buffer[256];//Hier komt de ontvangen data van de client
    char buffer2[256];//Data dat wordt gestuurd naar de client wordt hier bewaard

    struct sockaddr_in serv_addr, cli_addr;//declaratie van het server adres/client adres als type struct sockaddr_in
    int n;

    //Roept de socket functie aan
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr,
          sizeof(serv_addr));//bzero plaatst bytes (lengte van serv_addr) met als null als waarde op de positie van serv_addr
    portno = PORT;
    //declaratie van de variabele van de struct serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //Geeft de socket weer vrij nadat het programma wordt afgesloten
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        printf("Error so_reusaddr");


    /* Now bind the host address using bind() call.*/
    //De local socket serv_addr wordt toegewezen aan het adres van socket sockfd
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }


        //wacht in slaapstand tot een client verbinding probeert te maken
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);

        //Accepteert connectie van de client
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

    while (1) {


        bzero(buffer, 256);//Buffer wordt naar null gezet
        n = read(newsockfd, buffer, 1024);//Leest de waarde van de buffer uit
        //Controleert of er data is ontvangen van de client
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }


        string commandArr[2];


        splitString(commandArr, buffer);

        for (const auto &i : commandArr) {
            cout << i << endl;
        }


        if (strcmp(commandArr[0].c_str(), "con") == 0) {

            if (strcmp(commandArr[1].c_str(), "f") == 0) {
                forward();
            }
            if (strcmp(commandArr[1].c_str(), "b") == 0) {
                backward();
            }
            if (strcmp(commandArr[1].c_str(), "l") == 0) {
                left();
            }
            if (strcmp(commandArr[1].c_str(), "r") == 0) {
                right();
            }
            if (strcmp(commandArr[1].c_str(), "s") == 0) {
                stop();
            }


        }


        if (strcmp(commandArr[0].c_str(), "aan") == 0) {

            if (strcmp(commandArr[1].c_str(), "a") == 0) {
                pthread_create(&thread0, NULL, afstandSense, NULL);
            }
            if (strcmp(commandArr[1].c_str(), "k") == 0) {
                pthread_create(&thread1, NULL, kompasSense, NULL);

            }
            if (strcmp(commandArr[1].c_str(), "e") == 0) {
                pthread_create(&thread2, NULL, snelheidSense, NULL);


            }
        }


        if (strcmp(commandArr[0].c_str(), "uit") == 0) {

            if (strcmp(commandArr[1].c_str(), "a") == 0) {
                pthread_cancel(thread0);


            }
            if (strcmp(commandArr[1].c_str(), "k") == 0) {
                pthread_cancel(thread1);


            }
            if (strcmp(commandArr[1].c_str(), "e") == 0) {
                pthread_cancel(thread2);


            }

        }



        if (strcmp(commandArr[0].c_str(), "arm") == 0) {

            if (strcmp(commandArr[1].c_str(), "h") == 0) {
                int angleHZ;
                angleHZ= 70 / 18 + 2;



            }
            if (strcmp(commandArr[1].c_str(), "l") == 0) {
                int angleHZ;

                angleHZ= 200 / 18 + 2;





            }


        }





    }
}


int main(int argc, char const *argv[]) {

    pthread_t thread010;

    Motorinit();

    bool loop = true;
    pthread_create(&thread010, NULL, socketServer, NULL);

    while (1){}




}


