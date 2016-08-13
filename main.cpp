#include "mbed.h"
#include "HDC1000.h"
#include "SakuraAlpha.h"
//In use to "Sakura-IoT-module-Arduino shield" for 824MAX.
HDC1000 hmtp(D14,D15); //TI HDC1000 (scl,sda)
AnalogIn SoilAI(A1); //Soil moisuture sensor(Arduino compatible)Moisture Sensor (SKU:SEN0114)
AnalogIn LuxAI(A0); //DFRobot Ambient Light Sensor SKU:DFR0026
SakuraAlphaSPI sakura(D11, D12, D13, D10); // mosi, miso, sclk, cs
Serial pc(USBTX, USBRX);//Debug printf out.

float Soil,Lux,Temp,RH;
 
int main() {
pc.baud(115200);
pc.printf("Waiting to come online...\r\n");
    for(;;){
        if( sakura.getNetworkStatus() == 1 ) break;
        pc.printf("No Connecting \r\n");
        wait_ms(1000);
    }
    pc.printf("Connecting Now \r\n");

    while(true){
hmtp.get();
Soil = SoilAI.read() * 100;// Conversion　Percentages
Lux = LuxAI.read() * 100;// Conversion　Percentages
Temp = hmtp.temperature();// Temperature sensor Analog
RH = hmtp.humidity();// Humidity sensor Analog
sakura.writeChannel(1,Soil);
sakura.writeChannel(2,Lux);
sakura.writeChannel(3,Temp);
sakura.writeChannel(4,RH);

// USB Serial output
pc.printf("Soil%:=%.2f,Lux%:=%.2f,Temp:=%.2f,Humidity:=%.2f\r\n",Soil  ,Lux  ,Temp  ,RH);
   wait(3.0);    
    }

}
