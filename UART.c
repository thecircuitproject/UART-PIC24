/*
 * File:   UART.c
 * Author: Jose Borrayo
 *
 * Created on February 24, 2021, 4:28 PM
 */


#include "xc.h"
#include "p24FJ128GC006.h"
#define FCY 8000000 //Internal oscillator frequency.
#include <libpic30.h>
#include <string.h>
#define baudrate 9600
#define BRGVAL ((FCY/baudrate)/16)-1

void setUARTpins(){
    TRISFbits.TRISF5 = 1; //Set PORTF5 as input.
    TRISBbits.TRISB4 = 0; //Set PORTB4 as output.
    RPINR17bits.U3RXR = 0b10001; //Set PORTB5 as UART3 receive.
    RPOR14bits.RP28R = 28; //Set PORTB4 as UART3 transmit.
}

void initializeUART(){
    U3MODEbits.STSEL = 0; //One stop bits.
    U3MODEbits.PDSEL = 0; //8-bit data, no parity.
    U3MODEbits.ABAUD = 0; //Baud rate measurement is disabled or completed.
    U3MODEbits.BRGH = 1; //High-Speed mode. 
    U3BRG = BRGVAL; //Calculated using Example 18-1;
    U3MODEbits.UEN1 = 0; //U3TX, and U3RX pins are enabled and used.
    //U3STAbits.UTXISEL1 = 1; //Interrupt when the last character is shifted out of
    //the transmit shift register; all transmit operations are completed.
    U3MODEbits.UARTEN = 1; //UARTx is enabled, all UARTx pins are controlled by UARTx as defined by UEN.
    U3STAbits.UTXEN = 1; //Transmit is enabled,UxTX pin is controlled by UARTx (PORTB5).
}

void outChar1(char *letter, unsigned int num){
    unsigned int i;
    for(i = 0;i<num;i++){
        while(U3STAbits.UTXBF);
        U3TXREG = letter[i];
    }
}

int inChar1(void) {
    while (!U3STAbits.URXDA);                   // char is ready
    return U3RXREG;                             //read the receive register
}

void turnLED1(){
    PORTFbits.RF4 = 1;
    __delay_ms(100);
    PORTFbits.RF4 = 0;
    __delay_ms(100);
}

int main(void) {
    TRISFbits.TRISF4 = 0;
    setUARTpins();
    initializeUART();
    turnLED1();
    int k;
    while(1){
        outChar1("Hello World",11);
        k = inChar1;
        turnLED1();
    }
}
