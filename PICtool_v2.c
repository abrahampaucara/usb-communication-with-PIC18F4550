/////////////////////////////////////////////////////////////////////////
//// Proyecto: PICtool v2.0                                          ////
//// Descripción: Manipular 4 pines del puerto B, con niveles TTL    ////
////              mediante una conexión USB 2.0, con interfaz        //// 
////              grafica para windows                               ////
//// Microcontrolador: PIC18F2550                                    ////
//// Modo USB: Custom Drive / 12Mbs                                  ////
//// Controlador: WinUSB para Windows 7 y 8                          ////
//// web: www.desarrollopic.com                                      ////
/////////////////////////////////////////////////////////////////////////

#include <18F4550.h>


#Fuses HSPLL, NOWDT, NOPROTECT, NOLVP, NODEBUG, USBDIV, PLL5, CPUDIV1, VREGEN

#use delay(clock=20M)

#use     FAST_IO  (B)

#define USB_HID_DEVICE     FALSE             //disabled HID
#define USB_EP1_TX_ENABLE  USB_ENABLE_BULK   //turn on EP1(EndPoint1) for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE  USB_ENABLE_BULK   //turn on EP1(EndPoint1) for OUT bulk/interrupt transfers
#define USB_EP1_TX_SIZE    2                 //size to allocate for the tx endpoint 1 buffer
#define USB_EP1_RX_SIZE    2                 //size to allocate for the rx endpoint 1 buffer

#include <pic18_usb.h>                       //Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#include "pic18_WinUSB.h"                    //Descriptors and USB configuration
#include <usb.c>                             //handles usb setup tokens and get descriptor reports

#define led1     PIN_B1
#define led2     PIN_B2
#define led3     PIN_B3
#define led4     PIN_B4
#define led5     PIN_B5
#define led6     PIN_B6
#define pin_on   output_high
#define pin_off  output_low

void main(void) {

   int8 iBuff[2];//Tamaño de Buffer a utilizar/2bytes
   int8 envia[2];
   set_tris_b(0x00);
   set_tris_d(0xFF);
   delay_cycles(5);

   pin_off(led1);                            //Apagar LEDs
   pin_off(led2);
   pin_off(led3);
   pin_off(led4);
   pin_off(led5);
   pin_off(led6);

   usb_init();

   usb_task();
   usb_wait_for_enumeration();
   
   while (TRUE)
   {
      if(usb_enumerated())                   //Conexión USB establecida
      {
         if (usb_kbhit(1))                   //Verificar recepción de datos
         {
            salir:
                  pin_off(led1);                            //Apagar LEDs
                  pin_off(led2);
                  pin_off(led3);
                  pin_off(led4);
                  pin_off(led5);
                  pin_off(led6);
                  
            usb_get_packet(1, iBuff, 2);     //Leer paquete de 2 bytes EP 1

            if (iBuff[0] == 0)               //Modo LED
            {
               if (iBuff[1] == 1) output_toggle(led1); // Toggle LED1
               if (iBuff[1] == 2) output_toggle(led2); // Toggle LED2
               if (iBuff[1] == 3) output_toggle(led3); // Toggle LED3
               if (iBuff[1] == 4) output_toggle(led4); // Toggle LED4
               if (iBuff[1] == 5) output_toggle(led5); // Toggle LED5
               if (iBuff[1] == 6) output_toggle(led6); // Toggle LED6
            }
            if (iBuff[0] == 1)
            {
               if (iBuff[1] == 1)
                  {                    
                     repetir:                  
                     output_high(led1); 
                     delay_ms(2000); 
                     output_low(led1);
                     usb_get_packet(1, iBuff, 2);
                     if (iBuff[1] == 2) goto salir;
                     output_high(led2); 
                     delay_ms(2000); 
                     output_low(led2);
                     usb_get_packet(1, iBuff, 2);
                     if (iBuff[1] == 2) goto salir;
                     output_high(led3); 
                     delay_ms(2000); 
                     output_low(led3);
                     usb_get_packet(1, iBuff, 2);
                     if (iBuff[1] == 2) goto salir;
                     output_high(led4); 
                     delay_ms(2000); 
                     output_low(led4);
                     usb_get_packet(1, iBuff, 2);
                     if (iBuff[1] == 2) goto salir;
                     output_high(led5); 
                     delay_ms(2000); 
                     output_low(led5);
                     usb_get_packet(1, iBuff, 2);
                     if (iBuff[1] == 2) goto salir;
                     output_high(led6); 
                     delay_ms(2000); 
                     output_low(led6);
                     usb_get_packet(1, iBuff, 2);
                     if (iBuff[1] == 2) goto salir;
                     goto repetir;
                  }
               if (iBuff[1] == 2) goto salir; 
               if (iBuff[1] == 3)
                  {
                     if(input(PIN_D0==TRUE))
                        {
                           envia[0] = 0;
                           envia[1] = 1;
                           usb_put_packet(1, envia, 2, USB_DTS_TOGGLE); //enviamos el paquete de tamaño 2 bytes del EP1 al PC
                           //dato = input(PIN_B1);
                           //envia[0] = dato;
                           //usb_put_packet(1, envia, 1, USB_DTS_TOGGLE); 
                           ////////// int8 envia[1];
                           /////////int dato;
                        }
                  }
            }
         }
      }
   }
}
