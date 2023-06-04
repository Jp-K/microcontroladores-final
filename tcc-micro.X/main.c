/*
 * File:   main.c
 * Author: Ian Pinto de Almeida e João PAulo Kraisch
 *
 * Created on 31 de Maio de 2023, 08:18
 */


#include <xc.h>
#include<stdio.h>
#pragma config OSC = XT, WDT = OFF, BOREN = OFF
#define _XTAL_FREQ 16000000

#define LED1 PORTDbits.RD0
#define LED2 PORTDbits.RD1
#define LED3 PORTDbits.RD2
#define LED4 PORTDbits.RD3

#define BTN1 PORTBbits.RB5
#define BTN2 PORTBbits.RB4
#define BTN3 PORTBbits.RB7
#define BTN4 PORTBbits.RB6

void putch(char byte) { // cria um override do putch para usar no printf
    TXREG = byte;
    while (!TXSTAbits.TRMT);
}

//void __interrupt() interruption (void) {
//    
//    if (INTCONbits.RBIF == 0x01) {
//        if (SW1 == 0) // Verify if switch 1 was pressed
//        {
//            __delay_ms(50);
//            if (SW1 == 0) {
//                pressedSW1 = 1; // Set pressed switch 1 variable
//            }
//        }
//        INTCONbits.RBIF == 0x01 = 0; // Clear interruption flag
//    }
//};

//char getLED(char pos) {
//    switch (pos) {
//        case 1:
//            return LED1;
//        case 2:
//            return LED2;
//        case 3:
//            return LED3;
//        case 4:
//            return LED4;   
//    }
//    return 0;
//}

void setLED(char pos, char value) {
    switch (pos) {
        case 1:
            LED1 = value;
            break;
        case 2:
            LED2 = value;
            break;
        case 3:
            LED3 = value;
            break;
        case 4:
            LED4 = value;
            break;
    }
}

char getBTN(char pos) {
    switch (pos) {
        case 1:
            return !BTN1;
        case 2:
            return !BTN2;
        case 3:
            return !BTN3;
        case 4:
            return !BTN4;   
    }
    return 0;
}

void initUART(void) {
    TRISCbits.RC6 = 1; // portas de TX e RX como entrada digital
    TRISCbits.RC7 = 1;
    SPBRG = 34; // ajuste do gerador de baud rate (115200 bps p/ fosc = 16MHz)
    SPBRGH = 0; // deixa a parte alta vazia
    BAUDCONbits.BRG16 = 1; // modo de 16 bits
    TXSTAbits.BRGH = 1; // seta o baud rate
    TXSTAbits.SYNC = 0; // modo assíncrono
    TXSTAbits.TXEN = 1; // habilita tx
    RCSTAbits.SPEN = 1; // serial habilitada
    RCSTAbits.CREN = 1; // modo contínuo para recepção   
}

void init() {
    ADCON1 = 0x0F; // Configure all pins to digital
    TRISD  = 0x00; // Configure all port D pins as output
    PORTD = 0x00; // Initialize 0 in PORT D
    
    TRISB = 0xFF;
    INTCON2bits.RBPU = 0;
    
    RCONbits.IPEN      = 0; // Enable interruptions legacy mode
//
//    INTCONbits.INT0IF  = 0; // Set 0 for interruption flag 
//    INTCON3bits.INT1IF = 0; // Set 0 for interruption flag 
//    
//    //INT BUTTON EDGE
    INTCONbits.RBIE = 1; // Enable interruption on RB pins
    
    //INTCON2bits.INTEDG0 = 0; // RB0 Interrupt Rising Edge
    //INTCON2bits.INTEDG1 = 0; // RB1 Interrupt Rising Edge
//    
    //INTCONbits.INT0IE  = 1; // Set 1 for external interruptions
    //INTCON3bits.INT1IE = 1;
    //INTCONbits.GIE     = 1; // Enable global interruptions
    
}

void main(void) {
    init();
    initUART();
    printf("Init UART! \n");
    
    
    
    while(1){
        
        setLED(1, getBTN(1));
        
        setLED(2, getBTN(2));
        
        setLED(3, getBTN(3));
        
        setLED(4, getBTN(4));
        
//        LED2 = 1;
//        __delay_ms(2000);
//        LED2 = 0;
//        
//        LED3 = 1;
//        __delay_ms(2000);
//        LED3 = 0;
//        
//        LED4 = 1;
//        __delay_ms(2000);
//        LED4 = 0;
    }
    
    return;
}
