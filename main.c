//*********************************************************************************************
// Laboratorio 7
// Helder Ovalle
// 18349
//*********************************************************************************************

//*********************************************************************************************
// Librerias
//*********************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

//**********************************************************************************************
// Variables
//**********************************************************************************************
uint8_t bandera = 0;
//**********************************************************************************************
// Prototipos de funciones
//**********************************************************************************************
void display(uint8_t data);//display
//**********************************************************************************************
// Funcion Principal
//**********************************************************************************************
int main(void)
{
    //Configuraciones necesarias
    //Reloj
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);



    //Se utilizo el puerto b para el display
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);//pines a utilizar
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);
    //Puerto c para los push
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7); //pines a utilizar
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    //Puerto d para los leds rojos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    //HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
    //GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //pines a utilizar
    //Puerto e para leds verdes
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //pines a utilizar

    // uart config
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // enable A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1); // enable uart1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1); // uart controls pins
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); // config uart
    UARTIntClear(UART1_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX | UART_INT_FE | UART_INT_PE | UART_INT_BE | UART_INT_OE | UART_INT_RI | UART_INT_CTS | UART_INT_DCD | UART_INT_DSR); // clear isr
    UARTEnable (UART1_BASE);

    uint8_t bandera_dos = 1;

    while (1)
    {
        bandera = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7) >> 4;//CORRIMIENTO DE BITS 1101 0110 0011 0001
        if (bandera != bandera_dos)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, bandera);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ~bandera);
            display( 4 - (((bandera >> 3) & 0x01) + ((bandera >> 2) & 0x01) + ((bandera >> 1) & 0x01) + (bandera & 0x01)));
            bandera_dos = bandera;
            UARTCharPut(UART1_BASE, bandera);
        }
    }
}

void display(uint8_t bandera)
{
    uint8_t temp = 0x71;
    switch (bandera & 0x0F)
    {
        case 0:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0xFF);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
            break;
        case 1:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
            break;
        case 2:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0xFF);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
            break;
        case 3:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0xFF);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
            break;
        case 4:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
            break;
        default:
            temp = 0xFF;
    }
    //GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, temp);
}
