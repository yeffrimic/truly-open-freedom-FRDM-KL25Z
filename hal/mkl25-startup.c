//+------------------------------------------------------------------------------------------------+
//| Codigo de inicializacion para el MCU Kinetis MKL25.                                            |
//|                                                                                                |
//| Este archivo implementa infraestructura de codigo basica para el microcontrolador, tal como la |
//| tabla de vectores, inicializacion del procesador y los perifericos, inicializacion de la       |
//| memoria y bits de configuracion no volatiles.                                                  |
//|                                                                                                |
//| Autor: Joksan Alvarado.                                                                        |
//+------------------------------------------------------------------------------------------------+

#include <stdint.h>

#include "mkl25-sim.h"

//Definicion de tipo para las funciones de manejo de interrupciones.
typedef void (* handler_t)();

//Estructura para el campo de configuracion de flash.
struct flash_configuration_field_type {
  uint8_t backdoor_comparison_key[8];
  uint8_t FPROT[4];
  uint8_t FSEC;
  uint8_t FOPT;
  uint8_t reserved0;
  uint8_t reserved1;
};

//Simbolos exportados por el guion de enlazador.
extern uint32_t __stack_end__;
extern const uint32_t __relocate_flash_start__;
extern uint32_t __relocate_sram_start__;
extern uint32_t __relocate_sram_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

//Funciones externas invocadas por el codigo de inicializacion.
extern void __libc_init_array();
extern int main();

//--------------------------------------------------------------------------------------------------

//Rutina de arranque, ubicada en el vector de arranque.
void startup() {
  const uint32_t *flash;
  uint32_t *sram;

  //Desactiva el watchdog.
  SIM->COPC = SIM_COPC_COPW_Normal | SIM_COPC_COPCLKS_Int_1kHz | SIM_COPC_COPT_Disabled;

  //Se habilitan los relojes para todos los puertos.
  SIM->SCGC5 = SIM_SCGC5_PORTA_Enabled | SIM_SCGC5_PORTB_Enabled | SIM_SCGC5_PORTC_Enabled |
               SIM_SCGC5_PORTD_Enabled | SIM_SCGC5_PORTE_Enabled;

  //Se copian los datos de inicializacion desde la flash hacia la RAM.
  flash = &__relocate_flash_start__;
  sram = &__relocate_sram_start__;
  while (sram < &__relocate_sram_end__)
    *sram++ = *flash++;

  //Se inicializa la seccion .bss a cero.
  sram = &__bss_start__;
  while (sram < &__bss_end__)
    *sram++ = 0;

  //Se inicializa la libreria de C.
  __libc_init_array();

  //El sistema esta listo. Se llama la funcion main.
  main();
}

//Manejador de interrupciones por defecto.
static void unused_handler() {
  //El manejador por defecto no hace nada. Solo bloquea el CPU.
  for (;;);
}

//--------------------------------------------------------------------------------------------------

//Referencias debiles para los manejadores de vectores de sistema.
void __attribute__((weak, alias("unused_handler"))) nmi_handler();
void __attribute__((weak, alias("unused_handler"))) hard_fault_handler();
void __attribute__((weak, alias("unused_handler"))) svcall_handler();
void __attribute__((weak, alias("unused_handler"))) pendablesrvreq_handler();
void __attribute__((weak, alias("unused_handler"))) systick_handler();

//Referencias debiles para los demas vectores (perifericos).
void __attribute__((weak, alias("unused_handler"))) dma_channel_0_handler();
void __attribute__((weak, alias("unused_handler"))) dma_channel_1_handler();
void __attribute__((weak, alias("unused_handler"))) dma_channel_2_handler();
void __attribute__((weak, alias("unused_handler"))) dma_channel_3_handler();
void __attribute__((weak, alias("unused_handler"))) flash_memory_module_handler();
void __attribute__((weak, alias("unused_handler"))) low_voltage_handler();
void __attribute__((weak, alias("unused_handler"))) low_leakage_wakeup_handler();
void __attribute__((weak, alias("unused_handler"))) i2c_0_handler();
void __attribute__((weak, alias("unused_handler"))) i2c_1_handler();
void __attribute__((weak, alias("unused_handler"))) spi_0_handler();
void __attribute__((weak, alias("unused_handler"))) spi_1_handler();
void __attribute__((weak, alias("unused_handler"))) uart_0_handler();
void __attribute__((weak, alias("unused_handler"))) uart_1_handler();
void __attribute__((weak, alias("unused_handler"))) uart_2_handler();
void __attribute__((weak, alias("unused_handler"))) adc_0_handler();
void __attribute__((weak, alias("unused_handler"))) cmp_0_handler();
void __attribute__((weak, alias("unused_handler"))) tpm_0_handler();
void __attribute__((weak, alias("unused_handler"))) tpm_1_handler();
void __attribute__((weak, alias("unused_handler"))) tpm_2_handler();
void __attribute__((weak, alias("unused_handler"))) rtc_alarm_handler();
void __attribute__((weak, alias("unused_handler"))) rtc_seconds_handler();
void __attribute__((weak, alias("unused_handler"))) pit_handler();
void __attribute__((weak, alias("unused_handler"))) usb_otg_handler();
void __attribute__((weak, alias("unused_handler"))) dac_0_handler();
void __attribute__((weak, alias("unused_handler"))) tsi_0_handler();
void __attribute__((weak, alias("unused_handler"))) mcg_handler();
void __attribute__((weak, alias("unused_handler"))) lptmr_0_handler();
void __attribute__((weak, alias("unused_handler"))) port_a_handler();
void __attribute__((weak, alias("unused_handler"))) port_d_handler();

//--------------------------------------------------------------------------------------------------

//Tabla de vectores del procesador, localizada en 0x00000000.
static __attribute__ ((section(".vectors"), used))
handler_t vectors[48] = {
  //Manejadores para los vectores de sistema.
  (handler_t) &__stack_end__,   //0 - Puntero de pila inicial
  startup,                      //1 - Contador de programa inicial (vector de arranque)
  nmi_handler,                  //2 - Interrupcion no enmascarable
  hard_fault_handler,           //3 - Interrupcion de excepcion de fallo
  unused_handler,               //4
  unused_handler,               //5
  unused_handler,               //6
  unused_handler,               //7
  unused_handler,               //8
  unused_handler,               //9
  unused_handler,               //10
  svcall_handler,               //11 - LLamada de supervisor
  unused_handler,               //12
  unused_handler,               //13
  pendablesrvreq_handler,       //14 - Llamada pendiente para servicio de sistema
  systick_handler,              //15 - Interrupcion del temporizador de sistema

  //Manejadores para los vectores aparte de los de sistema.
  dma_channel_0_handler,            //16 - DMA canal 0 transferencia completa y error
  dma_channel_1_handler,            //17 - DMA canal 1 transferencia completa y error
  dma_channel_2_handler,            //18 - DMA canal 2 transferencia completa y error
  dma_channel_3_handler,            //19 - DMA canal 3 transferencia completa y error
  unused_handler,                   //20
  flash_memory_module_handler,      //21 - Modulo mem. flash, comando completo y colision de lectura
  low_voltage_handler,              //22 - Deteccion y advertencia de bajo nivel de voltaje
  low_leakage_wakeup_handler,       //23 - Despertar con baja corriente de fuga
  i2c_0_handler,                    //24 - I2C 0
  i2c_1_handler,                    //25 - I2C 1
  spi_0_handler,                    //26 - SPI 0
  spi_1_handler,                    //27 - SPI 1
  uart_0_handler,                   //28 - UART 0 estado y error
  uart_1_handler,                   //29 - UART 1 estado y error
  uart_2_handler,                   //30 - UART 2 estado y error
  adc_0_handler,                    //31 - ADC 0
  cmp_0_handler,                    //32 - CMP 0
  tpm_0_handler,                    //33 - TPM 0
  tpm_1_handler,                    //34 - TPM 1
  tpm_2_handler,                    //35 - TPM 2
  rtc_alarm_handler,                //36 - RTC interrupcion de alarma
  rtc_seconds_handler,              //37 - RTC interrupcion de segundos
  pit_handler,                      //38 - PIT interrupcion para todos los canales
  unused_handler,                   //39
  usb_otg_handler,                  //40 - USB OTG
  dac_0_handler,                    //41 - DAC 0
  tsi_0_handler,                    //42 - TSI 0
  mcg_handler,                      //43 - MCG
  lptmr_0_handler,                  //44 - Low power timer
  unused_handler,                   //45
  port_a_handler,                   //46 - Port A deteccion de pin
  port_d_handler,                   //47 - Port D deteccion de pin
};

//--------------------------------------------------------------------------------------------------

//Instancia del campo de configuracion de flash, localizado en 0x00000400.
static __attribute__ ((section(".flash_configuration_field"), used))
struct flash_configuration_field_type flash_config = {
  .backdoor_comparison_key = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, },
  .FPROT = { 0xFF, 0xFF, 0xFF, 0xFF, },
  .FSEC = 0xFE,     //Acceso de puerta trasera y seguridad apagados. Borrado masivo y acceso de
                    //fabrica activos.
  .FOPT = 0xFB,     //Inicializacion rapida, RESET_b como reset, apaga NMI, OUTDIV1 a 0 (alta
                    //velocidad)
  .reserved0 = 0xFF,
  .reserved1 = 0xFF,
};
