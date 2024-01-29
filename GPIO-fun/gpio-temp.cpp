#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <string>
#include <pigpio.h>

int main()
{
    //std::system("cat /sys/bus/w1/devices/28-7691f31d64ff/w1_slave");
    if(gpioInitialise() < 0)
        return 0;
    
    gpioSetPWMfrequency(17, 160);
    std::string filePath = "/sys/bus/w1/devices/28-7691f31d64ff/w1_slave";
    std::fstream file;
    float temperature = 0;
    
    while(true)
    {
        file.open(filePath, file.in);
        if(!file.is_open())
        {
            printf("failed to open!\n");
        }
        else
        {
            std::string line;
            while(getline(file, line))
            {
                std::string::size_type findT = line.find("t=");
                if(findT != std::string::npos)
                {
                    //std::cout<<line.substr(findT+2);
                    int tmp = std::stoi(line.substr(findT+2));
                    temperature = static_cast<float>(tmp)/1000;
                }
            }
            printf("Temperature: %.2fC\n", temperature);
        }
        file.close();

        if(temperature>=30.0f)
        {
            //gpioSetMode(17, PI_OUTPUT);
            //gpioWrite(17, 1);
            gpioPWM(17, 175);
        }
        else
        {
            gpioPWM(17, 0);
            //gpioWrite(17, 0);
        }



        time_sleep(1.0);
    }
    gpioTerminate();
    
    //std::cout<<i<<std::endl;
}