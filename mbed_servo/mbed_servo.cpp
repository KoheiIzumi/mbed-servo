/*--------------------------------------------------*/
/* @Program name ; mbed_servo                       */
/* @Author : Kohei Izumi                            */
/* @Comment : Servo motor operation                 */
/*                                                  */
/* Copyright(c) 2014 Kohei Izumi                    */
/*--------------------------------------------------*/

#include "mbed.h"
Serial device(p9, p10);         // tx = P9, rx = P10
DigitalOut REDE(p11);           // RS485 Transmit Enable
/*--------------------------------------------------*/
/* Funcyion     : mbed initialize                   */
/* NAME         : Init                              */
/* Argument     : ---                               */
/* Return value : ---                               */
/*--------------------------------------------------*/
void Init(void){
    device.baud(115200);        // baud Rate = 115.2kbps [Futaba default]
    REDE = 0;                   // RS485 Transmit disable
}
/*--------------------------------------------------*/
/* Funcyion     : Servo torque enable               */
/* NAME         : Torque                            */
/* Argument     : ID (Servo ID)                     */
/*              : data (Torque enable)              */
/* Return value : ---                               */
/*--------------------------------------------------*/
void Torque (unsigned char ID, unsigned char data){

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
/*--------------------------------------------------*/
/* Function     : Servo goal position instruction   */
/*                (Not specify time)                */
/* NAME         : SetPosition                       */
/* Argument     : ID (Servo ID)                     */
/*              : data (Set position)              */
/* Return value : ---                               */
/*--------------------------------------------------*/
void SetPosition (unsigned char ID, short data){

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
    for(int i=3; i<=8; i++){
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

/*--------------------------------------------------*/
/* Function     : Set the time and position         */ 
/* NAME         : SetTimeAndPosition                */
/* Argument     : ID (servo ID)                     */
/*              : data (set position)               */
/*              : time (time to move)               */ 
/* Return value : ---                               */
/*--------------------------------------------------*/
void SetTimeAndPosition(unsigned char ID, short data, unsigned short stime){
    unsigned char TxData[12];   // TransmitByteData [10byte]
    unsigned char CheckSum = 0; // CheckSum calculation
	
    TxData[0] = 0xFA;           // Header
    TxData[1] = 0xAF;           // Header
    TxData[2] = ID;             // ID
    TxData[3] = 0x00;           // Flags
    TxData[4] = 0x1E;           // Address
    TxData[5] = 0x04;           // Length
    TxData[6] = 0x01;           // Count
                                // Data
    TxData[7] = (unsigned char)0x00FF & data;           // Low byte
    TxData[8] = (unsigned char)0x00FF & (data >> 8);    // Hi  byte
    TxData[9] = (unsigned char)0x00FF & stime;           // Low byte
    TxData[10] = (unsigned char)0x00FF * (stime >> 8);   // Hi  byte
    
    // CheckSum calculation
    for (int i=3; i<=10; i++) {
        CheckSum = CheckSum ^ TxData[i];
    }
    TxData[11] = CheckSum;      // Sum

    // Send Packet
    REDE = 1;                   // Transmit Enable
    for(int i=0; i<=11; i++){
        device.putc(data[i]);
    }
    wait_us(250);               // Wait for transmission
    REDE = 0;                   // Transmitt disable
}

/*--------------------------------------------------*/
/* Function     : main program                      */ 
/* NAME         : main                              */
/* Argument     : ---                               */
/* Return value : ---                               */
/*--------------------------------------------------*/
int main() {
    Init();                     // initialize
    Torque(0x01, 0x01);         // ID = 1(0x01) , torque = OFF (0x00)
                                // torque = OFF(0x00), ON(0x01), BRAKE(0x02)
    wait(1);                    // wait (1sec)
    while(1){
        SetPosition(0x01,300); // ID = 1(0x01) , GoalPosition = 30.0deg(300)
        wait(1);                // wait (1sec)
        SetPosition(0x01,-300);// ID = 1(0x01) , GoalPosition = -30.0deg(-300) 
        wait(1);                // wait (1sec)

        SetTimeAndPosition(0x01, 500, 35);
	wait(1);
	SetTimeAndPosition(0x01, -500, 35);
	wait(1);
    }
}
