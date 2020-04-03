/*
 * homework4.h
 *
 *  Created on: 4/2/2020
 *      Author: Nathan Patskan
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();
bool charFSM(char rChar);

// TODO: Define any constants that are local to homework.c using #define

// This function returns the received char or 0xFF if no char is available
// valid args are
//   - \b EUSCI_A0_BASE
//   - \b EUSCI_A1_BASE
//   - \b EUSCI_A2_BASE
//   - \b EUSCI_A3_BASE

uint8_t UARTGetChar(uint32_t moduleInstance);


// This function Echos the received char

void UART_Echo(uint32_t moduleInstance, char transmitChar);



#endif /* HOMEWORK4_H_ */
