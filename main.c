 #define SLAVE_I2C_GENERIC_RETRY_MAX           100
 #define SLAVE_I2C_GENERIC_DEVICE_TIMEOUT      50   // define slave timeout 

#include "mcc_generated_files/system.h"
#include "MPU6050.h"
#include <stdio.h>
#include <xc.h>
#include <stdint.h>
#include <math.h>
#include "Telit.h"
#include "mcc_generated_files/tmr1.h"
#include "BME280.h"
#include "Global.h"




float MPUtemperature=0;
float MPUGyro[3]={0,0,0};
float MPUAcce[3]={0,0,0};
float BMPPressure =0;
float BMPHumidity=0;
float BMPtemperature=0;

int InitMsg=0;
char MSG[100];



int main(void)
{
    for(InitMsg=0;InitMsg<100;InitMsg++)
        MSG[InitMsg]='\0';
    
   SYSTEM_Initialize();
   TMR1_Stop();
   TMR1_SoftwareCounterClear(); // TMR1 buffer 0
   delay_ms(100);
 
   initMPU6050(); // Inicijalizacija senzora 6050
   delay_ms(100);
   BME280_Setup();
   delay_ms(100);
   
   TelitPower(); // Ukljucivanje modema
   delay_ms(5000);
   
   Telit_delate_message();
   Telit_delate_message();
   Telit_delate_message();
   TelitEcho(0); // iskljucivanje echo-a da ne salje sam sebi nazad podatke
   UartClear(); // Ocisti UartData
   
    while (1)
    {
        if(Telit_Call_check())
        {
            TelitHangUp();
            Telit_SendSms("Neko je nazvao senzore. Ko ko zeli da prica se senzorom xD ?!","+381637029828");
        }
        
        if(Telit_ReciveSms())
        {
            if(strcmp((char*)ReturnMSG(),"BME Temperatura") == 0)
            {
                BMPtemperature = BME280_readTemperature();
                sprintf(MSG,"Temperatura sa senzora BMP280 je : %2.2f C",(double)(BMPtemperature/100.0));
                Telit_SendSms(MSG,ReturnNUM());
            }
            
            if(strcmp((char*)ReturnMSG(),"BME Pritisak") == 0)
            {
                BMPPressure = BME280_readPressure();
                sprintf(MSG,"Pritisak sa senzora BMP280 je : %4.2f bar",(double)BMPPressure);
                Telit_SendSms(MSG,ReturnNUM());
            }
            
            if(strcmp((char*)ReturnMSG(),"BME Vlaznost") == 0)
            {
                BMPHumidity = BME280_readHumidity();
                sprintf(MSG,"Temperatura sa senzora BMP280 je : %2.2f procenata",(double)BMPHumidity);
                Telit_SendSms(MSG,ReturnNUM());
            }
            
            if(strcmp((char*)ReturnMSG(),"MPU Temperatura") == 0)
            {
                ReadDataMPU6050(&MPUtemperature,MPUGyro,MPUAcce);
                sprintf(MSG,"Temperatura sa senzora MPU6050 je : %2.2f C",(double)MPUtemperature);
                Telit_SendSms(MSG,ReturnNUM());
            }
            
            if(strcmp((char*)ReturnMSG(),"MPU Ubrzanje") == 0)
            {
                ReadDataMPU6050(&MPUtemperature,MPUGyro,MPUAcce);
                sprintf(MSG,"Ubrzanje sa senzora MPU6050 je : \r\n X = %1.5f g \r\n Y = %1.5f g \r\n Z = %1.5f g",(double)MPUAcce[0],(double)MPUAcce[1],(double)MPUAcce[2]);
                Telit_SendSms(MSG,ReturnNUM());
            }
            
            if(strcmp((char*)ReturnMSG(),"MPU Brzina") == 0)
            {
                ReadDataMPU6050(&MPUtemperature,MPUGyro,MPUAcce);
                sprintf(MSG,"Brzina sa senzora MPU6050 je : \r\n X = %1.5f g \r\n Y = %1.5f g \r\n Z = %1.5f g",(double)MPUGyro[0],(double)MPUGyro[1],(double)MPUGyro[2]);
                Telit_SendSms(MSG,ReturnNUM());
            }
            
            for(InitMsg=0;InitMsg<100;InitMsg++)
                MSG[InitMsg]='\0';
            
            delay_ms(500);
            Telit_delate_message();
            Telit_delate_message();
            
        }  
    }
    return 1;
}
