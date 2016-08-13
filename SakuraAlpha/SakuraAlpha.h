/* SAKURA Internet IoT Alpha Communication Module Library for mbed
 *
 * Copyright (c) SAKURA Internet Inc.
 *   The MIT License (MIT)
 *   https://github.com/sakura-internet/SakuraAlphaArduino
 *
 * https://iot.sakura.ad.jp/
 *
 * Porting to mbed by sakurafan. 2016
 */

#ifndef _SAKURAALPHA_H_
#define _SAKURAALPHA_H_

#include "mbed.h"

#define SAKURACC_SLAVE_ADDR (0x4F << 1)

#define CMD_NETWORKSTATUS  0x01

#define CMD_UPDATED     0x10
#define CMD_UNTRANSMITTED 0x11
#define CMD_TXCH_STATUS    0x12
#define CMD_RXCH_STATUS   0x13

#define CMD_READ      0x20

#define CMD_WRITE     0x30

#define CMD_TRANSMIT    0x40


#define TRANSMIT_NONE   0x00
#define TRANSMIT_AUTO   0x01
#define TRANSMIT_ONCE   0x02

#define TYPE_INT32      'i'
#define TYPE_UINT32     'I'
#define TYPE_INT64      'l'
#define TYPE_UINT64     'L'
#define TYPE_FLOAT      'f'
#define TYPE_DOUBLE     'd'
#define TYPE_BYTES      'b'


#define CH_SIZE 128

#define TX_STATUS_NONE    0x00
#define TX_STATUS_MODIFIED  0x01

#define RX_STATUS_NONE    0x00
#define RX_STATUS_UPDATED 0x01

#define min(a,b) (a<b?a:b)

class SakuraAlpha {
protected:

    virtual void begin(){}
    virtual void end(){}

    virtual void sendByte(uint8_t data){}
    virtual void finishSending(){}

    virtual void startReceiving(uint16_t num_recieve){}
    virtual uint8_t receiveByte(){return 0;}
    virtual void finishReceiving(){}

public:

    void writeChannel(uint8_t ch, int32_t data);
    void writeChannel(uint8_t ch, uint32_t data);
    void writeChannel(uint8_t ch, int64_t data);
    void writeChannel(uint8_t ch, uint64_t data);
    void writeChannel(uint8_t ch, float data);
    void writeChannel(uint8_t ch, double data);
    void writeChannel(uint8_t ch, uint8_t *data);
    void readChannel(uint8_t ch, char *type, uint8_t *data);

    void transmit(uint8_t mode);

    uint8_t getTxChannelStatus(uint8_t ch);
    uint8_t getRxChannelStatus(uint8_t ch);

    int getUpdatedChannels(uint8_t *buff, uint8_t len);

    int getUntransmittedChannels(uint8_t *buff, uint8_t len);
    
    uint8_t getNetworkStatus();

};


class SakuraAlphaI2C : public SakuraAlpha {

protected:
    I2C _i2c;
    int _num_recieve;

    virtual void begin();
    virtual void end();

    virtual void sendByte(uint8_t data);
    virtual void finishSending();

    virtual void startReceiving(uint16_t num_recieve);
    virtual uint8_t receiveByte();
    virtual void finishReceiving();

public:
    SakuraAlphaI2C (I2C &i2c);
    SakuraAlphaI2C (PinName sda, PinName scl);
};


class SakuraAlphaSPI : public SakuraAlpha{

protected:
    SPI _spi;
    DigitalOut _cs;

    virtual void begin();
    virtual void end();

    virtual void sendByte(uint8_t data);
    virtual void finishSending();

    virtual void startReceiving(uint16_t num_recieve);
    virtual uint8_t receiveByte();
    virtual void finishReceiving();

public:
    SakuraAlphaSPI(SPI &spi, PinName cs);
    SakuraAlphaSPI(PinName mosi, PinName miso, PinName sck, PinName cs);

};

#endif /* _SAKURAALPHA_H_ */
