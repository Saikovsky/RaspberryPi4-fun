#include <gpiod.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>

/*Gpio defines signle GPIO, consider arraying next time
gpiod_chip_get_line
gpiod_chip_get_lineS?*/

class GPIO
{
public:
    GPIO(unsigned int pinNumber)
    {
        this->offsets = pinNumber;

        chip = gpiod_chip_open("/dev/gpiochip0");
        if(!chip)
        {
            perror("gpiod_chip_open");
            cleanup();
            throw;
        }

    }
    ~GPIO()
    {
        cleanup();
    }

enum class DIRECTION : unsigned int
{
    AS_IS = 1,
    INPUT,
    OUTPUT,
    EVENT_FALLING_EDGE,
    EVENT_RISING_EDGE,
    EVENT_BOTH_EDGES
};

void setDirection(GPIO::DIRECTION direction)
{
    unsigned int lineRequest = GPIOD_LINE_REQUEST_DIRECTION_AS_IS;
    switch (direction)
    {
        case DIRECTION::AS_IS:
            lineRequest = GPIOD_LINE_REQUEST_DIRECTION_AS_IS;
        case DIRECTION::INPUT:
            lineRequest = GPIOD_LINE_REQUEST_DIRECTION_INPUT;
            break;
        case DIRECTION::OUTPUT:
            lineRequest = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT;
            break;
        case DIRECTION::EVENT_FALLING_EDGE:
            lineRequest = GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES;
            break;
        case DIRECTION::EVENT_RISING_EDGE:
            lineRequest = GPIOD_LINE_REQUEST_EVENT_RISING_EDGE;
            break;
        case DIRECTION::EVENT_BOTH_EDGES:
            lineRequest = GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES;
            break;
        default:
            lineRequest = GPIOD_LINE_REQUEST_DIRECTION_AS_IS;
            break;
    }

    line = gpiod_chip_get_line(chip, offsets);
    if(line == nullptr)
    {
        perror("gpiod_chip_get_lines");
        cleanup();
    }

    std::memset(&config, 0, sizeof(config));
    config.consumer = "gpio-wrapper";
    config.request_type = lineRequest;
    config.flags = 0;

    err = gpiod_line_request(line, &config, 0);
    if(err)
    {
        perror("gpiod_line_request_bulk");
        cleanup();
    }

}

void setValue(bool value)
{
    err = gpiod_line_set_value(line, value);
    if(err)
    {
        perror("gpiod_line_set_value_bulk");
        cleanup();
    }
}

int getValue()
{
    return gpiod_line_get_value(line);
}

int getGpioFd()
{
    return gpiod_line_event_get_fd(line);
}

private:

struct gpiod_chip* chip;
struct gpiod_line_request_config config;
struct gpiod_line* line;
unsigned int offsets;
bool value{0};
int err;

void cleanup()
{
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}

};

int main()
{
    GPIO gpio(26);
    gpio.setDirection(GPIO::DIRECTION::INPUT);
    /*gpio.setValue(1);
    sleep(1);
    //gpio.setDirection(INPUT);
    gpio.setValue(0); */

    struct pollfd gpioPoll;
    std::memset(&gpioPoll, 0, sizeof(gpioPoll));
    gpioPoll.fd = gpio.getGpioFd();
    gpioPoll.events = POLLIN;

    while(true)
    {
        poll(&gpioPoll, 1, 100);
        if(gpioPoll.revents & POLLIN)
        {
            printf("Event!\n");
            break;
        }

    }
    return 0;
}