/* 
 * File:   Telit.h
 * Author: milan
 *
 * Created on December 24, 2018, 6:50 PM
 */

#ifndef TELIT_H
#define	TELIT_H

#include <string.h>
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/pin_manager.h"
#include "MPU6050.h"
#include <stdbool.h>
#include "Global.h"

void UartClear(); //ocisti UART bafer
void uart_send_string(char* String);
//bool UartIsReady(); // obezbedjuje da se poruka sa UART-a porocita samo jednom
void ChangeRN(char* String);
void ClearNavodnike(char* String);
void ParseStr(char *String);
bool ValidSms();

void TelitPower(); //Ukljucivanje Telita
void TelitEcho(int Status); //Ukljucivanje/iskljucivanje echo funkcije Telita

bool Telit_Call_check(); // provera da li neko zove
void TelitHangUp(); //Odbi poziv

void Telit_delate_message(); //brisanje svih poruka
void Telit_SendSms(char *Message,char *Number); //telit salje sms na zadati broj
bool Telit_ReciveSms(); // telit prima sms poruku


#endif	/* TELIT_H */

