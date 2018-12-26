/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc.h"
#include "i2c3_driver.h"
#include <libpic30.h>

void (*i2c3_driver_busCollisionISR)(void);
void (*i2c3_driver_Masteri2cISR)(void);
void (*i2c3_driver_Slavei2cISR)(void);

inline void i2c3_driver_close(void)
{
    I2C3CONbits.I2CEN = 0;
}

/* Interrupt Control */
inline void i2c3_enableIRQ(void)
{
    IEC5bits.MI2C3IE = 1;
    IEC5bits.SI2C3IE = 1;
}

inline bool i2c3_IRQisEnabled(void)
{
    return IEC5bits.MI2C3IE || IEC5bits.SI2C3IE;
}

inline void i2c3_disableIRQ(void)
{
    IEC5bits.MI2C3IE = 0;
    IEC5bits.SI2C3IE = 0;
}

inline void i2c3_clearIRQ(void)
{
    IFS5bits.MI2C3IF = 0;
    IFS5bits.SI2C3IF = 0;
}

inline void i2c3_setIRQ(void)
{
    IFS5bits.MI2C3IF = 1;
    IFS5bits.SI2C3IF = 1;
}

inline void i2c3_waitForEvent(uint16_t *timeout)
{
    //uint16_t to = (timeout!=NULL)?*timeout:100;
    //to <<= 8;
    if((IFS5bits.MI2C3IF == 0) && (IFS5bits.SI2C3IF == 0))
    {
        while(1)// to--)
        {
            if(IFS5bits.MI2C3IF || IFS5bits.SI2C3IF) break;
            __delay_us(100);
        }
    }
}

bool i2c3_driver_driver_open(void)
{
    if(!I2C3CONbits.I2CEN)
    {
        // initialize the hardware
        // STAT Setting 
        I2C3STAT = 0x0;
        
        // CON Setting
        I2C3CON = 0x8000;
        
        // Baud Rate Generator Value: I2CBRG 100000;   
        I2C3BRG = 39;
        
        return true;
    }
    else
        return false;
}
bool i2c3_driver_initSlaveHardware(void)
{
    if(!I2C3CONbits.I2CEN)
    {


/* NOTE on SEN:
 * SEN will be set enabling clock stretching.  This is because we don't know how
 * long the user will take to process data bytes in their callbacks.  If they are fast,
 * we may not need to stretch the clock.  If they are slow, we need to stretch the clock.
 * If we ALWAYS stretch the clock, we will release the clock when the ISR is complete.
 */

/* NOTE on PCIE:
 * PCIE will be set to enable interrupts on STOP.  This will allow us know when
 * the master is finished
 */
        
/* NOTE on SCIE:
 * SCIE will be set to enable interrupts on START.  This will allow us to detect
 * both a START and a RESTART event and prepare to restart communications.
 */
        I2C3CONbits.STREN = 1;
        
        I2C3CONbits.I2CEN = 1;
        return true;
    }
    return false;
}

inline void i2c3_driver_resetBus(void)
{
    
}

inline void i2c3_driver_start(void)
{
    I2C3CONbits.SEN = 1;
}

inline void i2c3_driver_restart(void)
{
    I2C3CONbits.RSEN = 1;
}

inline void i2c3_driver_stop(void)
{
    I2C3CONbits.PEN = 1;
}

inline bool i2c3_driver_isNACK(void)
{
    return I2C3STATbits.ACKSTAT;
}

inline void i2c3_driver_startRX(void)
{
    I2C3CONbits.RCEN = 1;
}

inline char i2c3_driver_getRXData(void)
{
    return I2C3RCV;
}

inline void i2c3_driver_TXData(uint8_t d)
{
    I2C3TRN = d;
}

inline void i2c3_driver_sendACK(void)
{
    I2C3CONbits.ACKDT = 0;
    I2C3CONbits.ACKEN = 1; // start the ACK/NACK
}

inline void i2c3_driver_sendNACK(void)
{
    I2C3CONbits.ACKDT = 1;
    I2C3CONbits.ACKEN = 1; // start the ACK/NACK
}

inline void i2c3_driver_releaseClock(void)
{
    I2C3CONbits.SCLREL = 1;
}

inline bool i2c3_driver_isBuferFull(void)
{
    return I2C3STATbits.RBF || I2C3STATbits.TBF;
}

inline bool i2c3_driver_isStart(void)
{
    return I2C3STATbits.S;
}

inline bool i2c3_driver_isAddress(void)
{
    return !I2C3STATbits.D_A;
}

inline bool i2c3_driver_isStop(void)
{
    return I2C3STATbits.P;
}

inline bool i2c3_driver_isData(void)
{
    return I2C3STATbits.D_A;
}

inline bool i2c3_driver_isRead(void)
{
    return I2C3STATbits.R_W;
}

inline void i2c3_driver_clearBusCollision(void)
{
    I2C3STATbits.BCL = 0; // clear the bus collision.
}

inline void i2c3_driver_enableStartIRQ(void)
{
    
}

inline void i2c3_driver_disableStartIRQ(void)
{
    
}

inline void i2c3_driver_enableStopIRQ(void)
{
    
}

inline void i2c3_driver_disableStopIRQ(void)
{
    
}

inline void i2c3_driver_setBusCollisionISR(void *f){
    i2c3_driver_busCollisionISR = f;
}

inline void i2c3_driver_setMasterI2cISR(void *f){
    i2c3_driver_Masteri2cISR = f;
}

inline void i2c3_driver_setSlaveI2cISR(void *f){
    i2c3_driver_Slavei2cISR = f;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _MI2C3Interrupt ( void )
{
    (*i2c3_driver_Masteri2cISR)();
}
void __attribute__ ( ( interrupt, no_auto_psv ) ) _SI2C3Interrupt ( void )
{
    (*i2c3_driver_Slavei2cISR)();
}
