#include "Telit.h"

const char echo0[] = {"ATE0\r\n"};
const char echo1[] = {"ATE1\r\n"};
const char HangUp[] = {"AT+CHLD=0\r\n"};
const char Delate0[] = {"AT+CMGD=0\r\n"};
const char Delate1[] = {"AT+CMGD=1\r\n"};
const char Delate2[] = {"AT+CMGD=2\r\n"};
const char Delate3[] = {"AT+CMGD=3\r\n"};
const char Delate4[] = {"AT+CMGD=4\r\n"};
const char ReadSMS[] = {"AT+CMGR=1\r\n"};


void TelitEcho(int Status)
{   
    if(Status == 0){
        UART2_WriteBuffer( (uint8_t*)echo0 , strlen(echo0) ); //Iskljuciti echo  (da ne vraca poruke nazad)
    }
    else{
        UART2_WriteBuffer( (uint8_t*)echo1 , strlen(echo1) );
    }
}

void TelitPower()
{
    IO_RD4_SetDigitalOutput();  //MCC generated
    IO_RD4_SetHigh();

}

void TelitHangUp()
{
    delay_ms(100);
    UART2_WriteBuffer( (uint8_t*)HangUp , strlen(HangUp) );
    delay_ms(100);
}

bool Telit_Call_check()
{
	delay_ms(200);
	if(strcmp(UartData,"\r\nRING\r\n") == 0)
	{
        UartClear();
        return true;
    }
    UartClear();
	return false;
}

void UartClear()
{   int z = 0;
    for(z = 0; z < 100; z++)
    {
        UartData[z] = '\0';
    }
    UartNum = 0;
}

void Telit_delate_message()
{
	delay_ms(500);
    UART2_WriteBuffer( (uint8_t*)Delate0 , strlen(Delate0) );

	delay_ms(500);
    UART2_WriteBuffer( (uint8_t*)Delate1 , strlen(Delate1) );

	delay_ms(500);
    UART2_WriteBuffer( (uint8_t*)Delate2 , strlen(Delate2) );

	delay_ms(500);
    UART2_WriteBuffer( (uint8_t*)Delate3 , strlen(Delate3) );

	delay_ms(500);
    UART2_WriteBuffer( (uint8_t*)Delate4 , strlen(Delate4) );

	delay_ms(100);
    UartClear();
}

void Telit_SendSms(char *Message,char *Number)
{
	char num[50] = "AT+CMGS=\""; //posle ove AT komande sledi br telefona

	strcat(num,Number); // nalepi br telefona
	strcat(num,"\"\r\n"); // zavrsi poruku

	uart_send_string("AT+CSCS=\"GSM\"\r\n"); // mod rada GSM
	delay_ms(1000);
	uart_send_string("AT+CMGF=1\r\n");  // Text mode
	delay_ms(1000);

	uart_send_string(num); // posalji br telefona na koji da posalje poruku
    delay_ms(1000);

	uart_send_string(Message); // posalji ovu poruku
	delay_ms(20);

	UART2_Write(0x1A); // poruka mora da se zavrsi ovim karakterom
	delay_ms(1000);

}

void uart_send_string(char* String)
{
    while(UART2_TransmitBufferIsFull());
    UART2_WriteBuffer( (uint8_t*)String , strlen(String) );
}

/*bool UartIsReady()
{
    if(UartDataReady)
    {
        UartDataReady = 0;
        return true;
    }
    return false;
}*/

bool Telit_ReciveSms()  //Vratice true ako je poruka stigla
{
	delay_ms(200);
	uart_send_string("AT+CSCS=\"GSM\"\r\n"); // telit mod je gsm
    
	delay_ms(500);
	uart_send_string("AT+CMGF=1\r\n"); // Text mode
    
    delay_ms(500);
    UartClear();
	delay_ms(200);
    
    
    UartDataReady=0;
    
	uart_send_string("AT+CMGR=1\r\n"); // posalji mi poruku iz bafera
	
    delay_ms(50);
    
	while(!UartDataReady); // cekaj dok poruka ne stigne
    
    ChangeRN(UartData);
    ClearNavodnike(UartData);
    ParseStr(UartData);
    
    if(ValidSms())
    {
        int b;
        for(b=0;b<30;b++)
        {
            Number[b]='\0';
            Message[b]='\0';
        }
        
        strcpy(Number,ParseMSG[4]);
        strcpy(Message,ParseMSG[8]);
       
        return true;
    }
	return false;
}

void ChangeRN(char* String)
{
	int len=strlen(String);
	int i=0;

	for(i=0;i<len;i++)
	{
		if(String[i]=='\n' || String[i]=='\r')
		{
			String[i]='"';
		}
	}
}

void ClearNavodnike(char* String)
{
	int Size=strlen(String);
	int i=0,k=0;

	for(i=0;i<Size-1;i++)
	{
		if(String[i]=='"' && String[i+1]=='"')
			{
				for(k=i+1;k<(Size-1);k++)
				{
					String[k]=String[k+1];

				}
				i=0;
				String[Size-1]='\0';
			}
	}
}

void ParseStr(char *String)
{
    int b,b1;
    
    for(b=0;b<15;b++)
        for(b1=0;b1<50;b1++)
            ParseMSG[b][b1]='\0';
    
    int len=strlen(String);
    int broj,ParseBr=0,StringBr=0;
    
    for(broj=0;broj<len;broj++)
    {
        while(String[broj]!='"')
        {
            ParseMSG[ParseBr][StringBr]=String[broj];
            broj++;
            StringBr++;   
            
            if(broj>len) break;
        }
        
        StringBr=0;
        ParseBr++;
    }
    return;
}

bool ValidSms()
{
    if( ParseMSG[9][0]=='O' && ParseMSG[9][1]=='K' )
        return true;
    return false;
}