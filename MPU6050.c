/*
 * File:   MPU6050.c
 * Author: milan
 *
 * Created on 05 December 2018, 17:28
 */


#include "MPU6050.h"

// ********************* START DODATE VARIABLE **************************
// Specify sensor full scale

// ********************* END DODATE VARIABLE ****************************

void delay_ms(int a){
    int i,j;
    for(i = 0; i < 400; i++){
        for(j = 0; j < a; j++){
            Nop();
        }
    }   
}

uint8_t readByte(uint8_t address, uint8_t subAddress)
{   
    I2C3_MESSAGE_STATUS statusx=I2C3_MESSAGE_PENDING;
    uint8_t data[1];
    
    data[0]=subAddress;
           
    uint8_t rxbuf;
	rxbuf = 0;

    while(I2C3_MasterQueueIsFull() == true);
    I2C3_MasterWrite(&(data[0]),1,address,&statusx);
    I2C3_MasterRead(&rxbuf,1,address,&statusx); 
    delay_ms(10);

   return rxbuf;
}

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count1, uint8_t * dest)

{     
    I2C3_MESSAGE_STATUS statusx=I2C3_MESSAGE_PENDING;
    uint8_t data[14];
    uint8_t data_write[1];
    int ii;

    data_write[0] = subAddress;

    I2C3_MasterWrite(&(data_write[0]),1,address,&statusx);
    I2C3_MasterRead(data,count1,address,&statusx);
   
    delay_ms(10);
    
    for(ii = 0; ii < count1; ii++) 
    {
      dest[ii] = data[ii];
    }

} 
 void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
 {
     I2C3_MESSAGE_STATUS statusx;
     uint8_t Datax[2];
    
     Datax[1]=data;
     Datax[0]=subAddress;
     
     statusx = I2C3_MESSAGE_PENDING;
     
     while (statusx != I2C3_MESSAGE_COMPLETE)
     {
        I2C3_MasterWrite(Datax,2,address,&statusx);  
     }
     delay_ms(10);
 }
// ***************************** START FUNKCIJE DODATE ***********************
 
 void initMPU6050()
 {
   writeByte(0x68,SMPLRT_DIV,0x07);
   writeByte(0x68,PWR_MGMT_1,0x01); //budjenje
   writeByte(0x68,CONFIG,0x00);     //config
   writeByte(0x68,GYRO_CONFIG,0x18);//osetljivost ziroskopa
   writeByte(0x68,INT_ENABLE,0x01);
 }

 
 void ReadDataMPU6050(float *Temp,float *Gyro,float *Acce)
 {
    uint8_t AllData[14];
     
    readBytes(0x68,ACCEL_XOUT_H,14,AllData); 
    // akcelerometar
    Acce[0] = (float)( ((int)(AllData[0] << 8)) | AllData[1] ) / 16384.0;	
    Acce[1] = (float)( ((int)(AllData[2] << 8)) | AllData[3] ) / 16384.0;
    Acce[2] = (float)( ((int)(AllData[4] << 8)) | AllData[5] ) / 16384.0;
    // ziroskop
	Gyro[0] = (float)( ((int)(AllData[8] << 8)) | AllData[9] ) / 131.0;
	Gyro[1] = (float)( ((int)(AllData[10] << 8)) | AllData[11] ) / 131.0;
	Gyro[2] = (float)( ((int)(AllData[12] << 8)) | AllData[13] ) / 131.0;
    // temperatura
	(*Temp) = ((float)( ((int)(AllData[6] << 8)) | AllData[7] ) / 340.00) + 36.53;
 }
 //***************************** END FUNKCIJE DODATE *************************