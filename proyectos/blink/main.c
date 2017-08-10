#include "mkl25-port.h"
#include "mkl25-gpio.h"

#define PORT_LED_R PORTB
#define GPIO_LED_R GPIOB
#define PIN_LED_R 18

#define PORT_LED_G PORTB
#define GPIO_LED_G GPIOB
#define PIN_LED_G 19

#define PORT_LED_B PORTD
#define GPIO_LED_B GPIOD
#define PIN_LED_B 1

int main() {
  volatile uint32_t i;

  //Configura los pines de los LEDs como GPIO
  PORT_LED_R->PCR[PIN_LED_R] = PORT_PCR_MUX_Gpio;
  PORT_LED_G->PCR[PIN_LED_G] = PORT_PCR_MUX_Gpio;
  PORT_LED_B->PCR[PIN_LED_B] = PORT_PCR_MUX_Gpio;

  //Configura los pines de los LEDs como salida
  GPIO_LED_R->PDDR |= 1 << PIN_LED_R;
  GPIO_LED_G->PDDR |= 1 << PIN_LED_G;
  GPIO_LED_B->PDDR |= 1 << PIN_LED_B;

  //Apaga los LEDs inicialmente (se colocan en alto)
  GPIO_LED_R->PSOR = 1 << PIN_LED_R;
  GPIO_LED_G->PSOR = 1 << PIN_LED_G;
  GPIO_LED_B->PSOR = 1 << PIN_LED_B;

  for (;;) {
    for (i = 0; i < 100000; i++);         //Espera un breve momento
    GPIO_LED_R->PCOR = 1 << PIN_LED_R;
    GPIO_LED_G->PSOR = 1 << PIN_LED_G;
    GPIO_LED_B->PSOR = 1 << PIN_LED_B;

    for (i = 0; i < 100000; i++);         //Espera un breve momento
    GPIO_LED_R->PSOR = 1 << PIN_LED_R;
    GPIO_LED_G->PCOR = 1 << PIN_LED_G;
    GPIO_LED_B->PSOR = 1 << PIN_LED_B;

    for (i = 0; i < 100000; i++);         //Espera un breve momento
    GPIO_LED_R->PSOR = 1 << PIN_LED_R;
    GPIO_LED_G->PSOR = 1 << PIN_LED_G;
    GPIO_LED_B->PCOR = 1 << PIN_LED_B;
  }

  return 0;
}
