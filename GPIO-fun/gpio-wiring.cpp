#include <iostream>
#include <pigpio.h>
#include <signal.h>

unsigned int A = 440;
unsigned int B = 493;
unsigned int C = 261;
unsigned int Bm = 466;
unsigned int E = 329;
unsigned int D = 293;
unsigned int dutyCycle = 1000000;
unsigned int G = 392;
unsigned int F = 349;

void my_handler(int s){
           printf("Caught signal %d\n",s);
           gpioHardwarePWM(12, 0, 0);
           gpioTerminate();
           exit(1); 

}

void stop()
{
    gpioHardwarePWM(12, 0, dutyCycle/2);
    time_sleep(0.01);
}

void note(unsigned int note)
{
    gpioHardwarePWM(12, note, dutyCycle/24);
    time_sleep(0.3);
}

int main()
{
    if(gpioInitialise()<0)
        return 0;
    gpioSetMode(12, PI_OUTPUT);
    
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);


    auto hwPWM = gpioHardwarePWM(12, 0, dutyCycle);
    if(hwPWM>0)
        return 0;


    while(true)
    {
        note(G);
        stop();
        note(G);
        stop();
        stop();
        note(C);
        stop();
        note(F);
        stop();
        note(C);
        stop();
        note(G);
        stop();
    }
    
    
    /*while(true)
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
    }*/

    //gpioWrite(17, 1);
}