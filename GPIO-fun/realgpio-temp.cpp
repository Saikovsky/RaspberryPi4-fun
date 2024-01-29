#include <iostream>
#include <pigpio.h>
#include <unistd.h>
#include <bitset>

int pin = 4;

bool OWTouchReset()
{
    gpioSetMode(pin, PI_OUTPUT);
    gpioWrite(pin, PI_LOW);
    gpioDelay(480);
    gpioSetMode(pin, PI_INPUT);
    gpioDelay(70);
    int result = gpioRead(pin)^1;
    gpioDelay(410);
    std::cout<<result<<std::endl;
    return result;
}

void OWWriteBit(bool bit)
{
    gpioSetMode(pin, PI_OUTPUT);
    gpioWrite(pin, PI_LOW);
    gpioDelay(bit ? 10 : 65);
    gpioSetMode(pin, PI_INPUT);
    gpioDelay(bit ? 55 : 5);
}

bool OWReadBit()
{
    gpioSetMode(pin, PI_OUTPUT);
    gpioWrite(pin, PI_LOW);
    gpioDelay(24);
    gpioSetMode(pin, PI_INPUT);
    gpioDelay(4);
    bool result = (gpioRead(pin)== PI_HIGH);
    gpioDelay(28);
    return result;
    //return (gpioRead(pin) == PI_HIGH);
}

void OWWriteByte(uint8_t byte)
{
    for(int i = 0; i < 8; ++i)
    {
        OWWriteBit((byte & (1 << i)) != 0);
    }
}

uint8_t OWReadByte()
{
    uint8_t byte = 0;
    for(int i = 0; i<8; i++)
    {
        byte >>=1;
        //byte |= (OWReadBit() ? (1 << i) : 0);
        if(OWReadBit())
            byte |= 0x80;
    }
    return byte;
}

float readTemperature()
{
    for (int retry = 0; retry < 10; ++retry)
    {
        if(OWTouchReset())
        {
            break;
        }
        if (retry == 9)
        {
            std::cerr << "Failed to reset 1-Wire bus" <<std::endl;
            return -1.0f;
        }
    }

    OWWriteByte(0xCC);

    OWWriteByte(0x44);

    gpioDelay(750000);

    if(!OWTouchReset())
    {
        std::cerr <<"Failed to reset 1-wire after conversion."<<std::endl;
        return -1.0f;
    }

    OWWriteByte(0xCC);

    OWWriteByte(0xBE);

    uint8_t lsb = OWReadByte();
    uint8_t msb = OWReadByte();

    std::cout<<std::bitset<8>(lsb)<<std::endl;
    std::cout<<std::bitset<8>(msb)<<std::endl;
    uint16_t rawTemperature = (msb<<8) | lsb;
    std::cout<<std::bitset<16>(rawTemperature)<<std::endl;
    return static_cast<float>(rawTemperature) / 16.0;
}

int main()
{
    if(gpioInitialise()<0)
        return 1;
    
    float temperature = readTemperature();

    if(temperature >=0.0)
    {
        std::cout<< "Temperature: "<< temperature << " C "<<std::endl;
    }


    int devices = gpioSerialReadOpen(pin, 9600, 8);
    std::cout<< "Number of devices on the bus:" <<devices<<std::endl;
    gpioTerminate();
    return 0;
}