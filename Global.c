#include "Global.h"


int UartDataReady = 0; // Flag podaci spremni za citanje
char UartData[100]; //Bafer za podatke sa uart-a
int TimerData = 0; //Broji koliko puta je timer usao interrupt 
int TimerStart = 0; //Flag da znamo da li je stigao novi blok podataka
int UartNum = 0; // Broj podataka u baferu
char ParseMSG[15][50];
char Message[30]={" "};
char Number[30]={" "};

char *ReturnMSG()
{
    return Message;
}
char *ReturnNUM()
{
    return Number;
}