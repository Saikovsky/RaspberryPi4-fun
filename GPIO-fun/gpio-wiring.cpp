#include <iostream>
#include <pigpio.h>



int main()
{
    if(gpioInitialise()<0)
        return 0;
    gpioSetMode(17, PI_OUTPUT);
    while(true)
    {
        for(uint8_t dutycycle = 255; dutycycle>10; dutycycle-=10)
        {
            gpioPWM(17, dutycycle);
            time_sleep(0.10);
        }
        for(uint8_t dutycycle = 0; dutycycle<240; dutycycle+=10)
        {
            gpioPWM(17, dutycycle);
            time_sleep(0.10);
        }
    }

    //gpioWrite(17, 1);
}