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

#include "OledFont8x8.h"
#include "OledFont8x16.h"
#include "OledI2C.h"

//-------------------------------------------------------------------------

namespace
{
volatile static std::sig_atomic_t run = 1;
}

//-------------------------------------------------------------------------
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

//-------------------------------------------------------------------------

void showTime(SSD1306::OledI2C& oled, char state)
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

int
main()
{
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

        SSD1306::OledI2C oled{"/dev/i2c-1", 0x3C};

        while (run)
        {
            showTime(oled, 'c');

            constexpr auto oneSecond(std::chrono::seconds(1));
            std::this_thread::sleep_for(oneSecond);
        }

        oled.clear();
        oled.displayUpdate();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}
