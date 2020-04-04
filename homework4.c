#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.
    bool finished = false;

    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 UART_config =
    {
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,//uint_fast8_t selectClockSource;
     19,//uint_fast16_t clockPrescalar;
     8, //uint_fast8_t firstModReg;
     0x55,//uint_fast8_t secondModReg;
     EUSCI_A_UART_NO_PARITY,//uint_fast8_t parity;
     EUSCI_A_UART_LSB_FIRST,//uint_fast16_t msborLsbFirst;
     EUSCI_A_UART_ONE_STOP_BIT,//uint_fast16_t numberofStopBits;
     EUSCI_A_UART_MODE,//uint_fast16_t uartMode;
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,//uint_fast8_t overSampling;
     EUSCI_A_UART_8_BIT_LEN//uint_fast16_t dataLength;
    };

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,GPIO_PIN3,GPIO_PRIMARY_MODULE_FUNCTION); // set Tx
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN2,GPIO_PRIMARY_MODULE_FUNCTION); // set Rx

    // TODO: Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &UART_config);

    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        rChar = UARTGetChar(EUSCI_A0_BASE);

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if(rChar != 0xFF)
        {
            UART_Echo(EUSCI_A0_BASE, rChar);
            finished = charFSM(rChar);
        }


        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
        if(finished)
        {
            UART_TransmitResponse(EUSCI_A0_BASE, response);
            finished = false;
        }

    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool finished = false;
    typedef enum {SX, S2, S25, S253, S2534} state;
    static state currentState = SX;
    switch(currentState)
    {
    case SX:
        if(rChar == '2')
            currentState = S2;
        break;
    case S2:
        if(rChar == '5')
            currentState =S25;
        else if(rChar == '2')
           currentState = S2;
        break;
    case S25:
        if(rChar == '3')
            currentState =S253;
        else if(rChar == '2')
           currentState = S2;
        break;
    case S253:
        if(rChar == '4')
        {
            finished = true;
            currentState =SX;
        }
        else if(rChar == '2')
           currentState = S2;
        break;

    }


    //if(rChar == 'q')
     //   finished = true;


    return finished;
}

uint8_t UARTGetChar(uint32_t moduleInstance)
{
    if (UART_getInterruptStatus(moduleInstance,EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == 1)
        return UART_receiveData(moduleInstance);
    else return 0xFF;
}


void UART_Echo(uint32_t moduleInstance, char transmitChar)
{
    while (UART_getInterruptStatus(moduleInstance,EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == 0)
    {}
    UART_transmitData(moduleInstance, transmitChar);
}


void UART_TransmitResponse(uint32_t moduleInstance, char* response)
{
    char outputChar;
    int ii = 0;
    for (ii; ii<MAX_RESPONSE_STRING_LENGTH;ii++)
    {
        outputChar = response[ii];
        if (outputChar != '\0')
        {
           while (UART_getInterruptStatus(moduleInstance,EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == 0)
               {}
           UART_transmitData(moduleInstance, outputChar);
        }
        else return;
    }
}
