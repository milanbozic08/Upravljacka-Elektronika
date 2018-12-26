#include "TelitGSM.h"



void TelitEcho(int Status)
{   
    if(Status==0)
        UART2_WriteBuffer( (uint8_t*)echo0 , strlen(echo0) );
    
    else
        UART2_WriteBuffer( (uint8_t*)echo1 , strlen(echo1) );
}

void TelitPower()
{
    IO_RD4_SetDigitalOutput();
    IO_RD4_SetHigh();

}

void TelitHangUp()
{
    delay_ms(100);
    UART2_WriteBuffer( (uint8_t*)HangUp , strlen(HangUp) );
    delay_ms(100);
}

bool SIM_Call_check()
{
	/*delay_ms(200);
	if(strcmp(UartData,"\r\nRING\r\n")==0)
	return true;*/
	return false;
}

void UartClear()
{   int z=0;
   /* for(z=0;z<70;z++)
    {
        UartData[z]='\0';
    }*/
}