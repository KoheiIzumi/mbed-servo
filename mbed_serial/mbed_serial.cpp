/*-------------------------------------------------*/
/* Turnof the power supplyinjection 
/*  1. Servo
/*  2. mbed
/*-------------------------------------------------*/

#include "mbed.h"
Serial device(p9, p10);         // tx = P9, rx = P10
DigitalOut REDE(p11);           // RS485 Transmit Enable
/*-------------------------------------------------*/
/* Funcyion     : mbed initialize                  */
/* NAME         : init                             */
/* Argument     : ---                              */
/* Return value : ---                              */
/*-------------------------------------------------*/
void init(void){
    device.baud(115200);        // baud Rate = 115.2kbps [Futaba default]
    REDE = 0;                   // RS485 Transmit disable
}
/*-------------------------------------------------*/
/* Funcyion     : servo torque enable              */
/* NAME         : torque                           */
/* Argument     : ID (Servo ID)                    */
/*              : data (Torque enable)             */
/* Return value : ---                              */
/*-------------------------------------------------*/
void torque (unsigned char ID, unsigned char data){

    unsigned char TxData[9];    // TransmitByteData [9byte]
    unsigned char CheckSum = 0; // CheckSum calculation
    
    TxData[0] = 0xFA;           // Header
    TxData[1] = 0xAF;           // Header
    TxData[2] = ID;             // ID
    TxData[3] = 0x00;           // Flags
    TxData[4] = 0x24;           // Address
    TxData[5] = 0x01;           // Length
    TxData[6] = 0x01;           // Count
    TxData[7] = data;           // Data
    
    // CheckSum calculation
    for(int i=2; i<=7; i++){
        CheckSum = CheckSum ^ TxData[i];                // XOR from ID to Data
    }
    
    TxData[8] = CheckSum;       // Sum
    
    // Send Packet 
    REDE = 1;                   // RS485 Transmit Enable
    for(int i=0; i<=8; i++){
        device.putc(TxData[i]);
    }
    wait_us(250);               // Wait for transmission
    REDE = 0;                   // RS485 Transmitt disable
}
/*-------------------------------------------------*/
/* Funcyion     : servo goal position instruction  */
/* NAME         : GoalPosition                     */
/* Argument     : ID (Servo ID)                    */
/*              : data (Goal Position)             */
/* Return value : ---                              */
/*-------------------------------------------------*/
void GoalPosition (unsigned char ID, int data){

    unsigned char TxData[10];   // TransmitByteData [10byte]
    unsigned char CheckSum = 0; // CheckSum calculation
    
    TxData[0] = 0xFA;           // Header
    TxData[1] = 0xAF;           // Header
    TxData[2] = ID;             // ID
    TxData[3] = 0x00;           // Flags
    TxData[4] = 0x1E;           // Address
    TxData[5] = 0x02;           // Length
    TxData[6] = 0x01;           // Count
                                // Data
    TxData[7] = (unsigned char)0x00FF & data;           // Low byte
    TxData[8] = (unsigned char)0x00FF & (data >> 8);    // Hi  byte
    
    // CheckSum calculation
    for(int i=2; i<=8; i++){
        CheckSum = CheckSum ^ TxData[i];                // XOR from ID to Data
    }
    TxData[9] = CheckSum;       // Sum
    // Send Packet
    REDE = 1;                   // RS485 Transmitt Enable
    for(int i=0; i<=9; i++){
        device.putc(TxData[i]);
    }
    wait_us(250);               // Wait for transmission
    REDE = 0;                   // RS485 Transmit disable
}
/*-------------------------------------------------*/
/* Funcyion     : main program                     */ 
/* NAME         : main                             */
/* Argument     : ---                              */
/* Return value : ---                              */
/*-------------------------------------------------*/
int main() {
    init();                     // initialize
    torque(0x01, 0x01);         // ID = 1(0x01) , torque = OFF (0x00)
                                // torque = OFF(0x00), ON(0x01), BRAKE(0x02)
    wait(1);                    // wait (1sec)
    while(1){
        GoalPosition(0x01,300); // ID = 1(0x01) , GoalPosition = 30.0deg(300)
        wait(1);                // wait (1sec)
        GoalPosition(0x01,-300);// ID = 1(0x01) , GoalPosition = -30.0deg(-300) 
        wait(1);                // wait (1sec)
    }
}
