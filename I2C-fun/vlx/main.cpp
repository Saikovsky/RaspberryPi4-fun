#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "/usr/include/pigpio.h"

#define REG_IDENTIFICATION_MODEL_ID (0xC0)
#define REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV (0x89)
#define REG_MSRC_CONFIG_CONTROL (0x60)
#define REG_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT (0x44)
#define REG_SYSTEM_SEQUENCE_CONFIG (0x01)
#define REG_DYNAMIC_SPAD_REF_EN_START_OFFSET (0x4F)
#define REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD (0x4E)
#define REG_GLOGAL_CONFIG_REF_EN_START_SELECT (0x86)
#define REG_SYSTEM_INTERRUPT_CONFIG_GPIO (0x0A)
#define REG_REG_GPIO_HV_MUX_ACTIVE_HIGH (0x84)
#define REG_SYSTEM_INTERRUPT_CLEAR (0x0B)
#define REG_RESULT_INTERRUPT_STATUS (0x13)
#define REG_SYSRANGE_START (0x00)
#define REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0 (0xB0)
#define REG_RESULT_RANGE_STATUS (0x14)

static uint8_t stop_variable = 0;

bool data_init(int& i2cHandle)
{
    bool failure = true;

    uint8_t vhv_config_sc1_sda = 0;
    int readI2C = i2cReadByteData(i2cHandle, REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV);
    if(readI2C<0)
        return false;
    vhv_config_sc1_sda = static_cast<uint8_t>(readI2C);
    readI2C = 0;
    vhv_config_sc1_sda |= 0x01;
    failure = i2cWriteByteData(i2cHandle, REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, vhv_config_sc1_sda);
    if(failure)
        return false;

    failure = i2cWriteByteData(i2cHandle, 0x88, 0x00);

    failure &= i2cWriteByteData(i2cHandle, 0x80, 0x01);
    failure &= i2cWriteByteData(i2cHandle, 0xFF, 0x01);
    failure &= i2cWriteByteData(i2cHandle, 0x00, 0x00);
    readI2C = i2cReadByteData(i2cHandle, 0x91);
    stop_variable = static_cast<uint8_t>(readI2C);
    failure &= i2cWriteByteData(i2cHandle, 0x00, 0x01);
    failure &= i2cWriteByteData(i2cHandle, 0xFF, 0x00);
    failure &= i2cWriteByteData(i2cHandle, 0x80, 0x00);

    return !failure;
    
    
}

int main()
{
    if(gpioInitialise()<0)
        return 0;
    int i2cHandle = i2cOpen(1, 0x29, 0);
    printf("Status is %i\n", i2cHandle);

    int readByteData = i2cReadByteData(i2cHandle, 0xC0);

    printf("ReadByte is %i\n", readByteData);


    bool init = data_init(i2cHandle);
    printf("Data init is %u\n",init);
    gpioTerminate();
}