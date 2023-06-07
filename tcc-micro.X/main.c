/*
 * File:   main.c
 * Author: Ian Pinto de Almeida e João PAulo Kraisch
 *
 * Created on 31 de Maio de 2023, 08:18
 */


#include <xc.h>
#include<stdio.h>
#include <stdlib.h>
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

int pressedBTN1 = 0;
int pressedBTN2 = 0;
int pressedBTN3 = 0;
int pressedBTN4 = 0;

char game_iniciou = 0;
int esperando_sequencia_facil = 0;
unsigned long long  MILI_CNT = 0;

void putch(char byte) { // cria um override do putch para usar no printf
    TXREG = byte;
    while (!TXSTAbits.TRMT);
}
// fazer aqui o debounce dos 4 botões
void __interrupt() interruption (void) {
    
    if (INTCONbits.TMR0IF) {
        TMR0H = 0xF8;
        TMR0L = 0x47;
        
        MILI_CNT++;
        
        INTCONbits.TMR0IF = 0;
    }
    
    if (INTCONbits.RBIF == 0x01 && INTCONbits.RBIE == 1) {
        if (BTN1 == 0) // Verify if switch 1 was pressed
        {
            __delay_ms(50);
            if (BTN1 == 0) {
                pressedBTN1 = 1; // Set pressed switch 1 variable
                LED1 = 1;
                for (int i = 0; i < 255; i++) {
                    PORTCbits.RC2 = 1;
                    __delay_us(500);
                    PORTCbits.RC2 = 0;
                    __delay_us(500);  
                } 
                LED1 = 0;
            } 
        } else if (BTN2 == 0){
            __delay_ms(50);
            if (BTN2 == 0) {
                pressedBTN2 = 1; // Set pressed switch 1 variable
                LED2 = 1;
                for (int i = 0; i < 255; i++) {
                    PORTCbits.RC2 = 1;
                    __delay_us(400);
                    PORTCbits.RC2 = 0;
                    __delay_us(400);  
                }
                LED2 = 0;
            }
        } else if (BTN3 == 0){
            __delay_ms(50);
            if (BTN3 == 0) {
                pressedBTN3 = 1; // Set pressed switch 1 variable
                LED3 = 1;
                for (int i = 0; i < 255; i++) {
                    PORTCbits.RC2 = 1;
                    __delay_us(300);
                    PORTCbits.RC2 = 0;
                    __delay_us(300);  
                }
                LED3 = 0;
            }
        } else if (BTN4 == 0){
            __delay_ms(50);
            if (BTN4 == 0) {
                pressedBTN4 = 1; // Set pressed switch 1 variable
                LED4 = 1;
                for (int i = 0; i < 255; i++) {
                    PORTCbits.RC2 = 1;
                    __delay_us(200);
                    PORTCbits.RC2 = 0;
                    __delay_us(200);  
                }
                LED4 = 0;
            }
        }
        INTCONbits.RBIF = 0; // Clear interruption flag ? == 0x01
    }
};

unsigned long long millis() {
    return MILI_CNT;
}

void reproduz_som_led(char pos) {
    switch (pos) {
        case 1:
            for (int i = 0; i < 255; i++) {
                PORTCbits.RC2 = 1;
                __delay_us(500);
                PORTCbits.RC2 = 0;
                __delay_us(500);  
            } 
            break;
        case 2:
            for (int i = 0; i < 255; i++) {
                PORTCbits.RC2 = 1;
                __delay_us(400);
                PORTCbits.RC2 = 0;
                __delay_us(400);  
            } 
            break;
        case 3:
            for (int i = 0; i < 255; i++) {
                PORTCbits.RC2 = 1;
                __delay_us(300);
                PORTCbits.RC2 = 0;
                __delay_us(300);  
            } 
            break;
        case 4:
            for (int i = 0; i < 255; i++) {
                PORTCbits.RC2 = 1;
                __delay_us(200);
                PORTCbits.RC2 = 0;
                __delay_us(200);  
            }  
            break;
    }
}

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
    TRISCbits.RC2 = 0;
    INTCON2bits.RBPU = 0;
    
    RCONbits.IPEN      = 0; // Enable interruptions legacy mode
//
    INTCONbits.INT0IF  = 0; // Set 0 for interruption flag 
    INTCON3bits.INT1IF = 0; // Set 0 for interruption flag 
//    
//    //INT BUTTON EDGE
    INTCONbits.RBIE = 1; // Enable interruption on RB pins
    
    INTCON2bits.INTEDG0 = 0; // RB0 Interrupt Rising Edge
    INTCON2bits.INTEDG1 = 0; // RB1 Interrupt Rising Edge
//    
    INTCONbits.INT0IE  = 1; // Set 1 for external interruptions
    INTCON3bits.INT1IE = 1;
    INTCONbits.GIE     = 1; // Enable global interruptions
    
    // Timer configuration
    T0CONbits.T08BIT  = 0; // Configure 16 bits to timer0
    T0CONbits.T0CS    = 0; // Timer0 clock = fosc/4
    T0CONbits.PSA     = 1; // Set usage of prescaler
    //    T0CONbits.T0PS    = 4; // 1:32
    T0CONbits.TMR0ON  = 1; // Turn on timer0
    // Initialize timer0 values
    TMR0H = 0xF8;
    TMR0L = 0x47;
    
    INTCONbits.TMR0IF = 0; // Apaga a flag de interrup??o
    INTCONbits.TMR0IE = 1; // Habilita a interrup??o de timer0
}

int *generate_random_sequence_easy() {
    
    int array[4] = {};
    int r = 0;
    for (int i = 0; i < 4; i++) {
        r = rand() % 4;
        r = r + 1;
        array[i] = r;
    }
    return array;
}

void reproduz_vitoria() {
    
    LED1 = 1;
    for (int i = 0; i < 255; i++) {
        PORTCbits.RC2 = 1;
        __delay_us(700);
        PORTCbits.RC2 = 0;
        __delay_us(700);  
    }
    LED1 = 0;
    
    LED2 = 1;
    for (int i = 0; i < 255; i++) {
        PORTCbits.RC2 = 1;
        __delay_us(600);
        PORTCbits.RC2 = 0;
        __delay_us(600);  
    }
    LED2 = 0;
    
    LED3 = 1;
    for (int i = 0; i < 255; i++) {
        PORTCbits.RC2 = 1;
        __delay_us(500);
        PORTCbits.RC2 = 0;
        __delay_us(500);  
    }
    LED3 = 0;
    
    LED4 = 1;
    for (int i = 0; i < 255; i++) {
        PORTCbits.RC2 = 1;
        __delay_us(400);
        PORTCbits.RC2 = 0;
        __delay_us(400);  
    }
    LED4 = 0;
    
}

void reproduz_derrota() {
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    for (int i = 0; i < 255; i++) {
        PORTCbits.RC2 = 1;
        __delay_us(600);
        PORTCbits.RC2 = 0;
        __delay_us(600);  
    }
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;   
    __delay_ms(200);
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    for (int i = 0; i < 255; i++) {
        PORTCbits.RC2 = 1;
        __delay_us(800);
        PORTCbits.RC2 = 0;
        __delay_us(800);  
    }
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;    
    __delay_ms(200);
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    for (int i = 0; i < 255; i++) {
        PORTCbits.RC2 = 1;
        __delay_us(1000);
        PORTCbits.RC2 = 0;
        __delay_us(1000);  
    }
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;    
    __delay_ms(200);
    
}

void main(void) {
    init();
    initUART();
    printf("Init UART! \n");
    int array[12]= {};
    int r = 0;
    
    int estagio = 1;
    
    int possui_sequencia_facil = 0;
    int sequencia_usuario[12] = {0};
    int sequencia_facil_finalizou = 0;
    int sequencia_facil_perdeu = 0;
    int possui_sequencia_media = 0;
    int possui_sequencia_dificil = 0;
    
    while(1){
        
        // Start game logic
        if (game_iniciou == 0) {
            setLED(1, 1);
            if(pressedBTN1 == 1) {
                pressedBTN1 = 0;
                setLED(1, 0);
                game_iniciou = 1;
                
                srand(millis());
                
                __delay_ms(1000);
            }
            continue;
        }
        
        if (possui_sequencia_facil == 0) {
            for (int i = 0; i < 12; i++) {
                r = rand() % 4;
                r = r + 1;
                array[i] = r;
            }
            possui_sequencia_facil = 1;
        }
        // reproduz o nivel fácil
        if (esperando_sequencia_facil == 0) {
            INTCONbits.RBIE = 0; // Enable interruption on RB pins
            for (int i = 0; i < estagio; i++) {
                printf("%d ", array[i]);
                
                if (estagio <= 4) {
                    setLED(array[i], 1);
                    reproduz_som_led(array[i]);
                    __delay_ms(1000);
                    setLED(array[i], 0);
                    __delay_ms(500);
                } else if (estagio > 4 && estagio <= 8) {
                    setLED(array[i], 1);
                    reproduz_som_led(array[i]);
                    __delay_ms(400);
                    setLED(array[i], 0);
                    __delay_ms(200);
                } else if (estagio > 8 && estagio <= 12){
                    setLED(array[i], 1);
                    reproduz_som_led(array[i]);
                    __delay_ms(150);
                    setLED(array[i], 0);
                    __delay_ms(25);
                }
                
            }
            esperando_sequencia_facil = 1;
            pressedBTN1 = 0;
            pressedBTN2 = 0;
            pressedBTN3 = 0;
            pressedBTN4 = 0;
            INTCONbits.RBIE = 1; // Enable interruption on RB pins
            INTCONbits.RBIF = 0;
        }

        if (esperando_sequencia_facil == 1) {
            if (pressedBTN1 == 1) {
                pressedBTN1 = 0;
                sequencia_facil_finalizou = 1;
                for (int i = 0; i < estagio; i++) {
                    if (sequencia_usuario[i] == 0) {
                        sequencia_usuario[i] = 1;
                        if (i < estagio-1 && array[i] == sequencia_usuario[i]) {
                            sequencia_facil_finalizou = 0;
                        }
                        break;
                    }
                }

            } else if (pressedBTN2 == 1) {
                pressedBTN2 = 0;
                sequencia_facil_finalizou = 1;
                for (int i = 0; i < estagio; i++) {
                    if (sequencia_usuario[i] == 0) {
                        sequencia_usuario[i] = 2;
                        if (i < estagio-1 && array[i] == sequencia_usuario[i]) {
                            sequencia_facil_finalizou = 0;
                        }
                        break;
                    }
                }

            } else if (pressedBTN3 == 1) {
                pressedBTN3 = 0;
                sequencia_facil_finalizou = 1;
                for (int i = 0; i < estagio; i++) {
                    if (sequencia_usuario[i] == 0) {
                        sequencia_usuario[i] = 3;
                        if (i < estagio-1 && array[i] == sequencia_usuario[i]) {
                            sequencia_facil_finalizou = 0;
                        }
                        break;
                    }
                }

            } else if (pressedBTN4 == 1) {
                pressedBTN4 = 0;
                sequencia_facil_finalizou = 1;
                for (int i = 0; i < estagio; i++) {
                    if (sequencia_usuario[i] == 0) {
                        sequencia_usuario[i] = 4;
                        if (i < estagio-1 && array[i] == sequencia_usuario[i]) {
                            sequencia_facil_finalizou = 0;
                        }
                        break;
                    }
                }

            }

            if (sequencia_facil_finalizou == 1) {
                for (int i = 0; i < estagio; i++) {
                    printf("%d ", sequencia_usuario[i]);
                    if (sequencia_usuario[i] !=  array[i]) {
                        sequencia_facil_perdeu = 1;
                    }
                }
                if (sequencia_facil_perdeu == 1) {
                    printf("perdeu ");
                    reproduz_derrota();
                    
                    possui_sequencia_facil = 0;
                    esperando_sequencia_facil = 0;
                    sequencia_facil_finalizou = 0;
                    sequencia_facil_perdeu = 0;
                    estagio = 1;
                    
                    for (char i = 0; i < 12; i++) {
                        sequencia_usuario[i] = 0;
                    }
                    
                    __delay_ms(1000);
                    game_iniciou = 0;
                    
                } else {
                    printf("ganhou ");
                    estagio++;
                    
                    esperando_sequencia_facil = 0;
                    sequencia_facil_finalizou = 0;
                    sequencia_facil_perdeu = 0;
                    
                    for (char i = 0; i < 12; i++) {
                        sequencia_usuario[i] = 0;
                    }
                    
                    __delay_ms(1000);
                }
            }
        }
        
        if (estagio > 12) {
            possui_sequencia_facil = 0;
            estagio = 1;
            game_iniciou = 0;
            
            for (char i = 0; i < 3; i++) {
                reproduz_vitoria();
            }
        }
    }
    
    return;
}
