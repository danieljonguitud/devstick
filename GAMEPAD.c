// Programa de control para probar un GAMEPAD compuesto por 1 control anal�gico y 10 botones digitales.
// Utilizaremos un PIC18F2550 con conexi�n USB.
// Programador: Daniel Jonguitud Del Angel
// Librer�as de descriptores: Programada por LIAMNESSON, Moyano y Daniel Jonguitud.
// 22/07/15
//*****************************************************************************************************

#include <18F2550.h> // Definici�n de registros internos del PIC18F2550.
#device ADC=8 // CAD a 8 bits, justificaci�n a a la derecha.
#fuses NOMCLR,XTPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL1,CPUDIV1,VREGEN,NOPBADEN
// NOMCLR: No vamos ha usar el PIN MCLR, el reset se har� por soft.
// XTPLL: Vamos a usar un cristal de 4.00Mhz.
// NOWDT: No vamos a usar el perro guardian.
// NOPROTECT: Memoria no protejida contra lecturas.
// NODEBUG: No utilizamos c�digo para debugear.
// NOLVP: No utilizamos el modo de programaci�n con bajo voltaje.
// USBDIV: signfica que el clock del usb se tomar� del PLL/2 = 96Mhz/2 = 48Mhz.
// PLL1: significa que el PLL prescaler no dividir� la frecuencia del cristal. para XT = 4Mhz.
// CPUDIV1: El PLL postscaler decide la divisi�n en 2 de la frecuencia de salida del PLL de 96MHZ, si queremos 48MHZ, lo dejamos como est�.
// VREGEN: habilita el regulador de 3.3 volts que usa el m�dulo USB.
// NOPBADEN: Deshabilitamos el m�dulo conversor ADC del puerto B.

#use delay(clock=48000000)


#DEFINE USB_HID_DEVICE TRUE // Vamos a utilizar el protocolo HID.

// Solamente definimos el buffer de salida ya que solamente enviaremos datos a la PC.

#define USB_EP1_TX_ENABLE USB_ENABLE_INTERRUPT
#define USB_EP1_TX_SIZE 8 // Definici�n del tama�o del buffer de salida.

/*********************************************************************************************************/

// Definici�n de las librer�as utilizadas.
#include <pic18_usb.h>    // Drivers's USB del PIC18F2550.
#include <GAMEPAD.h> // Descriptores USB para el GAMEPAD.
#include <usb.c> // Funciones del USB.

/*********************************************************************************************************/

// Comienza programa principal.

void main(void) {

int envia[4]; // Definimos los 3 bytes a enviar al EP1.
#define     eje_x       envia[0]     // Byte del eje X del mando anal�gico.
#define     eje_y       envia[1]     // Byte del eje Y del mando anal�gico.
#define     botones     envia[2]     // Byte de los 8 botones del PAD.
#define     menus       envia[3]     // Byte de los 2 botones de los menus.

int botonera; // Variable que toma el valor de los botones presionados.
int botonesmenu;    // Variable que toma el valor de los botones presionados.

output_b(0x00); // Limpiamos el puerto B.
output_c(0x00); // Limpiamos el puerto C.
// Configuraciones varias:
   disable_interrupts(global);
   disable_interrupts(int_timer1);
   disable_interrupts(int_rda);
   disable_interrupts(int_ext);
   disable_interrupts(int_ext1);
   disable_interrupts(int_ext2);
   setup_spi(FALSE);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   port_b_pullups(FALSE);
   setup_adc_ports(AN0_TO_AN1);
   setup_adc(ADC_CLOCK_DIV_64);


// Iniciamos el USB.
      usb_init();                           // Inicializamos el USB.
      usb_wait_for_enumeration();           // Esperamos hasta que el dispositivo sea configurado por la PC.
      while(true) {
      usb_task();                                                         // Habilita el periferico usb y las interrupciones.

        if (usb_enumerated()) // Si el puerto es enumerado y configurado por el host..
                  {

                          set_adc_channel(0); // Selecciono el canal de conversi�n para el eje X.
                          delay_us(10); // Espero 10uS, a que termine la conversi�n.
                          eje_x = read_adc(); // Guardamos el valor de la variable.

                          set_adc_channel(1); // Selecciono el canal de conversi�n para el eje Y.
                          delay_us(10); // Espero 10uS, a que termine la conversi�n.
                          eje_y = read_adc(); // Guardamos el valor de la variable.

                          botonera = input_b(); // Tomamos el valor de los botones.
                          botones = botonera;   // Guardamos el valor de la variable.

                          botonesmenu = input_c(); // Tomamos el valor de los botones.
                          menus = botonesmenu;     // Guardamos el valor de la variable.

                          usb_put_packet(1,envia,4,USB_DTS_TOGGLE);   // Enviamos los datos por USB.
}

                      }
                            }
