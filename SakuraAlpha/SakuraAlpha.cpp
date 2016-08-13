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

#include "mbed.h"
#include "SakuraAlpha.h"

void SakuraAlpha::writeChannel(uint8_t ch, int32_t data) {
    uint8_t *p = (uint8_t *)&data;

    begin();

    sendByte(CMD_WRITE);
    sendByte(ch);
    sendByte(TYPE_INT32);

    for ( int i = 0 ; i < 4 ; i++ ) {
        sendByte(p[i]);
    }

    finishSending();
    end();
}

void SakuraAlpha::writeChannel(uint8_t ch, uint32_t data) {
    uint8_t *p = (uint8_t *)&data;

    begin();

    sendByte(CMD_WRITE);
    sendByte(ch);
    sendByte(TYPE_UINT32);

    for ( int i = 0 ; i < 4 ; i++ ) {
        sendByte(p[i]);
    }

    finishSending();
    end();
}

void SakuraAlpha::writeChannel(uint8_t ch, int64_t data) {
    uint8_t *p = (uint8_t *)&data;

    begin();

    sendByte(CMD_WRITE);
    sendByte(ch);
    sendByte(TYPE_INT64);

    for ( int i = 0 ; i < 8 ; i++ ) {
        sendByte(p[i]);
    }

    finishSending();
    end();
}

void SakuraAlpha::writeChannel(uint8_t ch, uint64_t data) {
    uint8_t *p = (uint8_t *)&data;

    begin();

    sendByte(CMD_WRITE);
    sendByte(ch);
    sendByte(TYPE_UINT64);

    for ( int i = 0 ; i < 8 ; i++ ) {
        sendByte(p[i]);
    }

    finishSending();
    end();
}

void SakuraAlpha::writeChannel(uint8_t ch, float data) {
    uint8_t *p = (uint8_t *)&data;

    begin();

    sendByte(CMD_WRITE);
    sendByte(ch);
    sendByte(TYPE_FLOAT);

    for ( int i = 0 ; i < 4 ; i++ ) {
        sendByte(p[i]);
    }

    finishSending();
    end();
}

void SakuraAlpha::writeChannel(uint8_t ch, double data) {
    uint8_t *p = (uint8_t *)&data;

    begin();

    sendByte(CMD_WRITE);
    sendByte(ch);
    sendByte(TYPE_DOUBLE);

    for ( int i = 0 ; i < 8 ; i++ ) {
        sendByte(p[i]);
    }

    finishSending();
    end();
}

void SakuraAlpha::writeChannel(uint8_t ch, uint8_t *data) {
    uint8_t *p = (uint8_t *)data;

    begin();

    sendByte(CMD_WRITE);
    sendByte(ch);
    sendByte(TYPE_BYTES);

    for ( int i = 0 ; i < 8 ; i++ ) {
        sendByte(p[i]);
    }

    finishSending();
    end();
}

void SakuraAlpha::readChannel(uint8_t ch, char *type, uint8_t *data) {
    uint8_t *p = (uint8_t *)&data;

    begin();

    sendByte(CMD_READ);
    sendByte(ch);
    finishSending();

    startReceiving(9);
    *(uint8_t *)type = receiveByte();

    for ( int i = 0 ; i < 8 ; i++ ) {
        data[i] = receiveByte();
    }
    finishReceiving();

    end();

}

void SakuraAlpha::transmit(uint8_t mode){
    begin();
    sendByte(CMD_TRANSMIT);
    sendByte(mode);

    finishSending();
    end();
}

uint8_t SakuraAlpha::getTxChannelStatus(uint8_t ch){
    uint8_t status=0xff;

    begin();

    sendByte(CMD_TXCH_STATUS);
    sendByte(ch);
    finishSending();

    startReceiving(1);
    status = receiveByte();
    finishReceiving();

    end();
    return status;
}

uint8_t SakuraAlpha::getRxChannelStatus(uint8_t ch){
    uint8_t status=0xff;

    begin();

    sendByte(CMD_RXCH_STATUS);
    sendByte(ch);
    finishSending();

    startReceiving(1);
    status = receiveByte();
    finishReceiving();

    end();
    return status;
}

int SakuraAlpha::getUpdatedChannels(uint8_t *buff, uint8_t len){
    int num = 0;

    begin();
    sendByte(CMD_UPDATED);
    finishSending();

    wait_ms(100);

    startReceiving(len+1);
    num = receiveByte();

    for( int i=0; i<min(num,len); i++){
        buff[i]=receiveByte();
    }

    finishReceiving();

    end();
    return num;
}

int SakuraAlpha::getUntransmittedChannels(uint8_t *buff, uint8_t len){
    int num = 0;

    begin();
    sendByte(CMD_UNTRANSMITTED);
    finishSending();

    wait_ms(100);

    startReceiving(len+1);
    num = receiveByte();

    for( int i=0; i<min(num,len); i++){
        buff[i]=receiveByte();
    }

    finishReceiving();

    end();
    return num;
}

uint8_t SakuraAlpha::getNetworkStatus(){
    uint8_t status=0x00;
    
    begin();
    
    sendByte(CMD_NETWORKSTATUS);
    finishSending();
    
    startReceiving(1);
    status = receiveByte();
    finishReceiving();
    
    end();
    return status;
}


void SakuraAlphaI2C::begin(){
    _i2c.start();
    _i2c.write(SAKURACC_SLAVE_ADDR);
}
void SakuraAlphaI2C::end(){

}

void SakuraAlphaI2C::sendByte(uint8_t data){
    _i2c.write(data);
}
void SakuraAlphaI2C::finishSending(){
    _i2c.stop();
}

void SakuraAlphaI2C::startReceiving(uint16_t num_recieve){
    _num_recieve = num_recieve;
    _i2c.start();
    _i2c.write(SAKURACC_SLAVE_ADDR | 1);
}
uint8_t SakuraAlphaI2C::receiveByte(){
    if (_num_recieve <= 0) return 0;
    _num_recieve --;
    return _i2c.read(_num_recieve > 0 ? 1 : 0);
}
void SakuraAlphaI2C::finishReceiving(){
    while (_num_recieve > 0) {
        _num_recieve --;
        _i2c.read(_num_recieve > 0 ? 1 : 0);
    }
    _i2c.stop();
}

SakuraAlphaI2C::SakuraAlphaI2C (I2C &i2c) : _i2c(i2c) {
}
SakuraAlphaI2C::SakuraAlphaI2C (PinName sda, PinName scl) : _i2c(sda, scl) {
}


void SakuraAlphaSPI::begin(){
    _cs = 0;
}
void SakuraAlphaSPI::end(){
    _cs = 1;
    wait_ms(10);
}

void SakuraAlphaSPI::sendByte(uint8_t data){
    wait_ms(10);
    _spi.write(data);
}
void SakuraAlphaSPI::finishSending(){
}

void SakuraAlphaSPI::startReceiving(uint16_t num_recieve){
}
uint8_t SakuraAlphaSPI::receiveByte(){
    wait_ms(10);
    return _spi.write(0);
}
void SakuraAlphaSPI::finishReceiving(){
}

SakuraAlphaSPI::SakuraAlphaSPI(SPI &spi, PinName cs) : _spi(spi), _cs(cs) {
    _cs = 1;
}
SakuraAlphaSPI::SakuraAlphaSPI(PinName mosi, PinName miso, PinName sck, PinName cs) : _spi(mosi, miso, sck), _cs(cs) {
    _cs = 1;
}
