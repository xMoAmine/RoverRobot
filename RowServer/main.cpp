//modified by Mohamed Amine Sahla IT201
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sstream>
#include <iterator>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <array>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <thread>
#include <system_error>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "include/OledI2C.h"
#include "include/OledFont8x16.h"
#include "include/OledI2C.h"
// ff uitzoeken wat dit betekent


namespace
{
    volatile static std::sig_atomic_t run = 1;
}

#define LDR 25
#define ON true
#define OFF false
#define LED 29
#define UP true
#define DOWN false
typedef  unsigned char uint8_t;

void* socketServer();
void* Motorcontrol (void* param);
void* dist (void* param); //thread function
void Motorinit();

SSD1306::OledI2C oled{"/dev/i2c-1", 0x3C};


bool ldrState = OFF;
bool afsState = OFF;
bool komState = OFF;
bool bladeState = DOWN;



int afsWaarde;
int komWaarde;



pthread_t socketThread;
pthread_t kompasThread;
pthread_t afstandThread;
pthread_t ldrThread;




uint8_t MotorHF[7] = {7,3,0xa5,2,3,0xa5,2};
// High speed forward left + right; add explanation

uint8_t MotorST[7] = {7,0,0,0,0,0,0};
// Stop left + right; add explanation

uint8_t MotorHR[7] = {7,3,0xa5,1,3,0xa5,1};
//High speed reverse left + right; add explanation

uint8_t MotorTurnL[7] = {7,3,0xa5,1,3,0xa5,2};
//High speed turn Left ?

uint8_t MotorTurnR[7] = {7,3,0xa5,2,3,0xa5,1};
//High speed turn right ?

int fd;
int ds;



// signaal handeler voor de oLed
static void
signalHandler(int signalNumber)
{
    switch (signalNumber)
    {
        case SIGINT:
        case SIGTERM:

            run = 0;
            break;
    };
}



void bladeHandler(char state){
    ofstream file;


    if (state == 'u'){
        file.open ("Python/blade.txt");
        file << "u";
        file.close();
    }

    else if (state == 'd') {
        file.open ("Python/blade.txt");
        file << "d";
        file.close();

    }



};
void showState(SSD1306::OledI2C& oled, char state)
{

    char notConnected[13] = {'N', 'O', 'T', ' ', 'C', 'O', 'N', 'N', 'E', 'C', 'T', 'E', 'D'};
    char connected[13] = {'C', 'O', 'N', 'N', 'E', 'C', 'T', 'E', 'D'};
    int length = 6;
    int offsetNotConnected = (128 - (18 * length)) / 2;
    int offsetConnected = (128 - (15 * length)) / 2;

    static constexpr SSD1306::PixelStyle style{SSD1306::PixelStyle::Set};


    if(state == 'n'){
        SSD1306::OledPoint location{offsetNotConnected, 18};
        location = drawString8x16(location, notConnected, style, oled);
        oled.displayUpdate();
    }
    if(state == 'c'){
        SSD1306::OledPoint location{offsetConnected, 18};

       location = drawString8x16(location, connected, style, oled);
        oled.displayUpdate();
    }

}
//void displayState(char state){
//    if(state == 'n'){
//        oled.clear();
//        oled.displayUpdate();
//        showState(oled, 'n');
//    }
//    if(state == 'c'){
//        oled.clear();
//        oled.displayUpdate();
//        showState(oled, 'c');
//    }
//}

void *ldrSense(void *param) {




    pinMode(LDR, INPUT);
    pinMode(LED ,OUTPUT);

    int input;




    for (;;) {

        input = digitalRead(25);

        if (input == 1) {
            digitalWrite(LED, ON);
            printf("Pin input: (%d) HIGH\n", input);

        }

        if (input == 0) {
            digitalWrite(LED, OFF);
            printf("Pin input: (%d) LOW\n", input);

        }
        delay(2500);


    } }



void motorInit()
{
    uint8_t Totalpower[2]={4,250};     // power between 0 and 255
    uint8_t Softstart[3]={0x91,23,0};  // add explanation


    wiringPiSetup () ;
    pullUpDnControl(0,PUD_DOWN);

    fd=wiringPiI2CSetup(0x32);

    write(fd,&Totalpower[0], 2);
    write(fd,&Softstart[0],3);

    //number of bytes = 3
    //What is a soft start?
}



void *afstandSense(void *param){
    int fd;

    fd=wiringPiI2CSetup(0x70);

    usleep(500000);  //delay 0,5 seconds



    double afstand;



    while(afsState){
        wiringPiI2CWriteReg8(fd, 0x00, 0x51);
        usleep(50000);

        afstand = wiringPiI2CReadReg16(fd, 2);

        afstand = afstand / 225;


        afsWaarde = (int)afstand;


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

    while(komState)
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


        komWaarde = (int)angle;

        usleep(5000000); // delay 5 seconds
    }

}


void bestuur(char direction){
    if(direction == 'w'){
        write(fd,&MotorHF[0],7);

    }

    else if(direction == 'a'){
        write(fd,&MotorTurnL[0],7);

    }

    else if(direction == 's'){
        write(fd,&MotorHR[0],7);

    }

    else if(direction == 'd'){
        write(fd,&MotorTurnR[0],7);

    }

    else if(direction == 'x'){
        write(fd,&MotorST[0],7);
    }

    else if(direction == 'k' && komState == OFF){
        komState = ON;
        pthread_create(&kompasThread, NULL, kompasSense, NULL);

    }

    else if(direction == 'f' && afsState == OFF){
        afsState = ON;
        pthread_create(&afstandThread, NULL, afstandSense, NULL);

    }


    else if(direction == 'b' && bladeState == DOWN){
        bladeHandler('u');


    }
    else if(direction == 'l' && ldrState == OFF){
        ldrState = ON;
        pthread_create(&ldrThread, NULL, ldrSense, NULL);

    }
    else if(direction == 'k' && komState == ON){
        komState = OFF;
    }

    else if(direction == 'f' && afsState == ON){

        afsState = OFF;
    }

    else if(direction == 'l' && ldrState == ON){
        ldrState = OFF;
    }

    else if(direction == 'b' && bladeState == UP){
        bladeHandler('d');


    }

}

void *socketServer(void *param)
{
    showState(oled, 'n');
    socklen_t sockfd, newsockfd, portno, clilen;//socklen_t is een unsigned int van 32 bits
    char buffer[256];//Hier komt de ontvangen data van de client
    char buffer2[256];//Data dat wordt gestuurd naar de client wordt hier bewaard

    struct sockaddr_in serv_addr, cli_addr;//declaratie van het server adres/client adres als type struct sockaddr_in
    int  n;

    //Roept de socket functie aan
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));//bzero plaatst bytes (lengte van serv_addr) met als null als waarde op de positie van serv_addr
    portno = 50000;
    //declaratie van de variabele van de struct serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //Geeft de socket weer vrij nadat het programma wordt afgesloten
    int yes = 1;
    if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 )
        printf("Error so_reusaddr");


    /* Now bind the host address using bind() call.*/
    //De local socket serv_addr wordt toegewezen aan het adres van socket sockfd
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    while(1){
        //wacht in slaapstand tot een client verbinding probeert te maken
        listen(sockfd,5);
        clilen = sizeof(cli_addr);



        //Accepteert connectie van de client
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        oled.clear();
        showState(oled, 'c');
        if (newsockfd < 0)
        {
            perror("ERROR on accept");

            oled.clear();
            showState(oled, 'n');

            exit(1);
        }


        bzero(buffer,256);//Buffer wordt naar null gezet
        printf("new msg\n");
        n = read( newsockfd,buffer,255 );//Leest de waarde van de buffer uit
        //Controleert of er data is ontvangen van de client
        if (n < 0)
        {
            bestuur('x');
            perror("ERROR reading from socket");

            oled.clear();
            showState(oled, 'n');

            exit(1);
        }


        char ch;
        float hoek = komWaarde;
        int distance = afsWaarde;
        int speed = 0;

        ch = *buffer;//De ontvangen waarde wordt in button gedaan

        bestuur(ch);

        printf("I'm receiving this character: %c\n",ch);


        sprintf(buffer2, "%d cm  - %f - %d m/s \n", distance, hoek, speed);
        puts(buffer2);
        n = write(newsockfd,buffer2,strlen(buffer2));

        if (n < 0)
        {
            perror("ERROR writing to socket");

            oled.clear();
            showState(oled, 'n');

            exit(1);
        }
    }

}


int main()
{
    afsWaarde = 0;
    komWaarde = 0;
    motorInit();//Functie motorInit wordt aangeroepen

    pthread_create(&socketThread,NULL,socketServer,0);//Start socketserver in een thread
    pthread_create(&ldrThread, NULL, ldrSense, NULL); //Ldr wordt in een thread gezet


    try
    {
        constexpr std::array<int, 2> signals{SIGINT, SIGTERM};

        for (auto signal : signals)
        {
            if (std::signal(signal, signalHandler) == SIG_ERR)
            {
                std::string what{"installing "};
                what += strsignal(signal);
                what += " signal handler";

                throw std::system_error(errno,
                                        std::system_category(),
                                        what);
            }
        }


        while (run)
        {
//            if (socketState == CONNECTED) {
//                showState(oled, 'c');
//
//                constexpr auto oneSecond(std::chrono::seconds(1));
//                std::this_thread::sleep_for(oneSecond);
//            }
//            if(socketState == NOT_CONNECTED){
//                showState(oled, 'n');
//
//                constexpr auto oneSecond(std::chrono::seconds(1));
//                std::this_thread::sleep_for(oneSecond);
//
//            }

        }

        oled.clear();
        oled.displayUpdate();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

//Loop zodat het programma aan blijft
    while(1)
    {

    }


}
