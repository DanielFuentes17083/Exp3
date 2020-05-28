/*
 * Autor: Daniel Fuentes Oajaca
 * Carnet: 17083
 * Curso: Digital 2
 * Seccion: 20
 * Experimento 2
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"               // Librerias a utilizar
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

/**
 * main.c
 */

int32_t uVal = 0;                           // Variable para guardar el valor que venga del UART
int color = 0;
char* carnet = "17083";
char* id_color = "";
char* server = "192.168.1.33";
char* temp;
char* temp2;
void sendString(char word[]);
void concatenate_string(char*, char*);
void sendPOST(void);


int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);               // Se configura el reloj a 20MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                                    // Se activan los periferico GPIOF para los LEDs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);                                                    // Se activan los periferico GPIOA para el UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);                                                    // Se activam los periferico GPIOB para el UART1
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);                       // Se configuran los pines 1, 2 y 3 del puero F (LED RGB) como salidas

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;                                           // Se realizan configuraciones para SW2
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0X1;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);                                              // Se configura el pin 4 del puerto F (boton SW1 y SW2) como entrada
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);        // Se configura el boton con una "fuerza" de 2mA y como weak Pull-Up

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);                                                    // Se activa el periferico del puerto UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);                                                    // Se activa el periferico del puerto UART0
    GPIOPinConfigure(GPIO_PB0_U1RX);                                                                //Se configuran TX y RX del UART1 en el puerto B
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); // Se configura el UART1 igual al UART0

    IntMasterEnable();                                                                              // Se habilitan las interrupciones

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1);                                        // Se indican los pines para el puerto UART
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));      // Se configura el UART, sera el puerto 0, con el reloj del sistema, a un BaudRate de 115200 y el formato de datos sera de 8 bits con 1 STOP bit y sin parity bit
    IntEnable(INT_UART1);                                                                           // Se habilita la interrupcion del puerto UART0
    UARTIntEnable(UART1_BASE, UART_INT_RX|UART_INT_RT);                                             // Se habilita las interrupciones por dato recibido y Receive Timeout interrup que es cuando termina de leerlo
    SysCtlDelay(30000000);
    sendString("ATE0\r\n");                                                                 // Comando AT para quitar eco
    SysCtlDelay(30000000);
    sendString("AT+CWMODE=1\r\n");                                                          // Comando AT para poner el modulo en modo cliente
    SysCtlDelay(30000000);
    sendString("AT+CWJAP=");                                                                // Comando AT para conectarse a alguna red
    UARTCharPut(UART1_BASE, '"');
    sendString("HOTEL_DON_ALFONSO");                                                        // Nombre de la red
    UARTCharPut(UART1_BASE, '"');
    UARTCharPut(UART1_BASE, ',');
    UARTCharPut(UART1_BASE, '"');
    sendString("Alfonso123");                                                               // Contraseña de la red
    UARTCharPut(UART1_BASE, '"');
    sendString("\r\n");
    SysCtlDelay(30000000);


    while(1){                                                                                       // Loop principal
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0){
            while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0);                                   // Espera a que deje de presionar para comenzar la rutina
            color = (color + 1)%8;                                                                  // Aumenta el valor de la variable color en el rango de 0 a 7
        }
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0){
            while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0);                                   // Espera a que deje de presionar para comenzar la rutina
            sendPOST();                                                                             // Funcion para hacer el POST en la pagina
        }
        if(color&0x01 == 1){                                                                        // Mediante una revision de cada bit de la variable color enciende o apaga la LED correspondiente
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0F);
        } else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
        }
        if((color>>1)&0x01 == 1){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0F);
        } else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
        }
        if((color>>2)&0x01 == 1){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0F);
        } else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
        }
    }
}

void sendString(char word[]){                                                       // Funcion para enviar Strings al UART1
    int i;
    for(i = 0;i<(strlen(word)); i = i+1){
        unsigned char letra = word[i];
        UARTCharPut(UART1_BASE, letra);
    }
}

void UARTIntHandler(void){                                                          // Interrupcion del puerto UART0
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART1_BASE, true);                                   // Se obtiene el masked interrupt status
    UARTIntClear(UART1_BASE, ui32Status);                                           // Se limpian la bandera de la interrupcion que se dio
    while(UARTCharsAvail(UART1_BASE)){                                              // Si se recibio un dato...
        uVal = UARTCharGet(UART1_BASE);
        UARTCharPut(UART0_BASE, uVal);                                              // Pone el caracter en el UART0 para poder ver la respuesta del módulo
    }
}

void sendPOST(void){
    sendString("AT+CIPSTART=");                                                     // Comando AT para iniciar un post
    UARTCharPut(UART1_BASE, '"');
    sendString("TCP");                                                              // Tipo del post
    UARTCharPut(UART1_BASE, '"');
    UARTCharPut(UART1_BASE, ',');
    UARTCharPut(UART1_BASE, '"');
    sendString(server);                                                             // Server al que se desea hacer el POST
    UARTCharPut(UART1_BASE, '"');
    SysCtlDelay(3000);
    sendString(",80\r\n");                                                          // Puerto
    SysCtlDelay(30000000);
    if (color == 0){
        sendString("AT+CIPSEND=170\r\n");                                           // Comando AT para enviar la cantidad de caracteres del post
        SysCtlDelay(30000000);                                                      // contenido del POST (este es igual para todos, unicamente cambia el valor de las variables dependiendo del color del led
        sendString("POST /index.php HTTP/1.0\r\nHost:  192.168.1.33\r\nAccept: */*\r\nContent-Length: 35\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid_color=0&carnet=17083&color=Negro");
    }else if (color == 1){
        sendString("AT+CIPSEND=170\r\n");
        SysCtlDelay(30000000);
        sendString("POST /index.php HTTP/1.0\r\nHost:  192.168.1.33\r\nAccept: */*\r\nContent-Length: 34\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid_color=1&color=Azul&carnet=17083");
    }else if (color == 2){
        sendString("AT+CIPSEND=170\r\n");
        SysCtlDelay(30000000);
        sendString("POST /index.php HTTP/1.0\r\nHost:  192.168.1.33\r\nAccept: */*\r\nContent-Length: 35\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid_color=2&color=Verde&carnet=17083");
    }else if (color == 3){
        sendString("AT+CIPSEND=170\r\n");
        SysCtlDelay(30000000);
        sendString("POST /index.php HTTP/1.0\r\nHost:  192.168.1.33\r\nAccept: */*\r\nContent-Length: 38\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid_color=3&color=Turquesa&carnet=17083");
    }else if (color == 4){
        sendString("AT+CIPSEND=170\r\n");
        SysCtlDelay(30000000);
        sendString("POST /index.php HTTP/1.0\r\nHost:  192.168.1.33\r\nAccept: */*\r\nContent-Length: 34\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid_color=4&color=Rojo&carnet=17083");
    }else if (color == 5){
        sendString("AT+CIPSEND=170\r\n");
        SysCtlDelay(30000000);
        sendString("POST /index.php HTTP/1.0\r\nHost:  192.168.1.33\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid_color=5&color=Violeta&carnet=17083");
    }else if (color == 6){
        sendString("AT+CIPSEND=170\r\n");
        SysCtlDelay(30000000);
        sendString("POST /index.php HTTP/1.0\r\nHost:  192.168.1.33\r\nAccept: */*\r\nContent-Length: 38\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid_color=6&color=Amarillo&carnet=17083");
    }else {
        sendString("AT+CIPSEND=170\r\n");
        SysCtlDelay(30000000);
        sendString("POST /index.php HTTP/1.0\r\nHost:  192.168.1.33\r\nAccept: */*\r\nContent-Length: 36\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid_color=7&color=Blanco&carnet=17083");
    }
    SysCtlDelay(40000000);
    sendString("\r\nAT+CIPCLOSE\r\n");                                                          // Se cierra el POST
}

