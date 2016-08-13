/*
 * mbed library program
 *  Low Power, High Accuracy Digital Humidity Sensor with Integrated Temperature Sensor
 *  HDC1000 Texas Instruments
 *
 * Copyright (c) 2015 Kenji Arai / JH1PJL
 *  http://www.page.sannet.ne.jp/kenjia/index.html
 *  http://mbed.org/users/kenjiArai/
 *      Created: Feburary   9th, 2015
 *      Revised: Feburary  22nd, 2015
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 *---------------- REFERENCE ----------------------------------------------------------------------
 *  http://www.ti.com/product/HDC1000/description
 *  http://akizukidenshi.com/catalog/g/gM-08775/
 */

#ifndef HDC1000_H
#define HDC1000_H

#include "mbed.h"

// Humidity / Temperature Sensor, HDC1000 T.I.
// Address b7=1,b6=0,b5=0,b4=0,b3=0,b2=0,b1=0, b0=R/W
#define HDC1000ADDR   (0x40 << 1)  // Akizuki Module (No other choice)
#define HDC1000ADDR1  (0x41 << 1)  // ADR1=0, ADR0=1
#define HDC1000ADDR2  (0x42 << 1)  // ADR1=1, ADR0=0
#define HDC1000ADDR3  (0x43 << 1)  // ADR1=1, ADR0=1

////////////// Registers //////////////////////////////////
// Register definition
#define HDC1000_REG_TEMP    0x00
#define HDC1000_REG_HUMI    0x01
#define HDC1000_REG_CONFIG  0x02
#define HDC1000_REG_S_ID_F  0xfb
#define HDC1000_REG_S_ID_M  0xfc
#define HDC1000_REG_S_ID_L  0xfd
#define HDC1000_REG_M_ID    0xfe
#define HDC1000_REG_D_ID    0xff

////////////// ID /////////////////////////////////////////
#define I_AM_HDC1000        0x1000
#define DEV_REG_ID          0x5449

////////////// Operating mode ///////////////////
#define ACQ_MODE_SEPARETE   (0UL << 12)
#define ACQ_MODE_BOTH       (1UL << 12)
#define TRES_14BIT          (0UL << 10)
#define TRES_11BIT          (1UL << 10)
#define HRES_14BIT          (0UL << 8)
#define HRES_11BIT          (1UL << 8)
#define HRES_08BIT          (2UL << 8)
#define BOTH_T_14_H_14      (TRES_14BIT + HRES_14BIT + ACQ_MODE_BOTH)

/** Interface for Humidity / Temperature Sensor, HDC1000
 * @code
 * #include "mbed.h"
 * #include "HDC1000.h"
 *
 * // I2C Communication
 *  HDC1000      hmtp(dp5,dp27);    // HDC1000 SDA, SCL (Akizuki module)
 * // If you connected I2C line not only this device but also other devices,
 * //     you need to declare following method.
 *  I2C          i2c(dp5,dp27);     // SDA, SCL
 *  HDC1000      hmtp(i2c);         // HDC1000 SDA, SCL (Akizuki module)
 *
 * int main() {;
 *   while(true){
 *      hmtp.get();    // Triger conversion
 *      printf("Temp: %+4.1fC, Humid: %4.1f%%\r\n", hmtp.temperature(), hmtp.humidity());
 *      wait(1.0);
 *   }
 * }
 * @endcode
 */

class HDC1000
{
public:
    /** Configure data pin
      * @param data SDA and SCL pins
      */
    HDC1000(PinName p_sda, PinName p_scl);
    HDC1000(PinName p_sda, PinName p_scl, uint8_t addr);

    /** Configure data pin (with other devices on I2C line)
      * @param I2C previous definition
      */
    HDC1000(I2C& p_i2c);
    HDC1000(I2C& p_i2c, uint8_t addr);

    /** Start convertion & data save
      * @param none
      * @return none
      */
    void get(void);

    /** Read temperature data
      * @param none
      * @return temperature
      */
    float temperature(void);

    /** Read humidity data
      * @param none
      * @return humidity
      */
    float humidity(void);

    /** HDC1000 Configuration
      * @param none
      * @return none
      */
    void config(void);

    /** Read Configuration
      * @param none
      * @return config. data
      */
    uint16_t read_config(void);

    /** Set config register
      * @param config parameter
      * @return config read data
      */
    uint16_t set_config(uint16_t cfg);

    /** Set I2C clock frequency
      * @param freq.
      * @return none
      */
    void frequency(int hz);

    /** check Device ID number
      * @param none
      * @return HDC1000 = 1, others  0
      */
    uint8_t who_am_i(void);

    /** Read Manufacturer ID
      * @param none
      * @return ID
      */
    uint16_t read_M_ID(void);

    /** Read Device ID
      * @param none
      * @return ID
      */
    uint16_t read_D_ID(void);

protected:
    I2C  _i2c;

    void get_IDs(void);
    void init(void);

private:
    uint8_t  HDC1000_addr;
    uint8_t  dt[4];
    uint16_t temp;
    uint16_t humi;
    uint16_t manufacturer_id_number;
    uint16_t device_id_number;
};

#endif      // HDC1000_H
