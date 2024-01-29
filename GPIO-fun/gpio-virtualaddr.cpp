#include <iostream>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    //GPFSEL1 => FSETL17 = 0b001 << 23;
    //GPSET0 => SET17 = 1 << 17;
    //
    //-1, addr = 0x7E200000, len = 0xF4
    int fdMem = open("/dev/gpiomem", O_RDWR | O_SYNC);
    if(fdMem < 0 )
    {
        std::cout<<"devmem failed\n";
        return 0;
    }
    uint32_t* gpioReg = (uint32_t*)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fdMem, 0x7E200000);
    if(gpioReg == MAP_FAILED)
    {
        std::cout<<"Map failed "<<errno<<std::endl;
        return 0;
    }
    //std::cout<<getpagesize()<<std::endl;
    //volatile uint32_t* GPFSEL1 = (volatile uint32_t*)0x7E200004;
    //*GPFSEL1 = 0b001 << 23;
    //volatile uint32_t* GPSET0 = (volatile uint32_t*)0x7E20001C;
    //*GPSET0 = 17 << 0;

    uint32_t GPFSEL1 = 0x04;
    uint32_t GPFSEL2 = 0x08;
    uint32_t GPSET0 = 0x1c;
    uint32_t GPCLR0 = 0x28;
    uint32_t GPLEV0 = 0x34;

    //*(gpioReg+GPCLR0) = 0xFFFFFFFF;
    //std::cout<<*gpioReg<<std::endl;
    //std::cout<<*(gpioReg+GPFSEL1)<<std::endl;
    //std::cout<<*(gpioReg+GPFSEL1)<<std::endl;
    //std::cout<<*(gpioReg+GPSET0)<<std::endl;
    //std::cout<<*(gpioReg<<std::endl;
    int reg, shift;
    reg = 17/10;
    shift = (17%10) * 3;
    gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (1 << shift);

    std::cout<<gpioReg[1]<<std::endl;
    std::cout<<&gpioReg[1]<<std::endl;
    std::cout<<&gpioReg<<std::endl;
}