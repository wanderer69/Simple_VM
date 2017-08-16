/******************************************************************************
 *
 * A simple program which sends a greeting to UART0, then echos
 * characters on UART0 and blinks an LED every 1/2 second.
 * from: Bill Knight, R O SoftWare <BillK@rosw.com>
 * ----------------------------------------------------------------------------
 *
 * - Adapted to the Olimex LPC-P2106 demo-board (Philips LPC2106).
 * - Sends message if button/switch on demo-board is hit.
 * - Slightly modified and extended as WinARM demo-application.
 * by Martin THOMAS <eversmith@heizung-thomas.de>
 *
 * $RCSfile: $
 * $Revision: $
 *
 *****************************************************************************/
#ifndef __BORLANDC__
#ifndef __LINUX
#include "lib/types.h"
#include "lib/LPC22xx.h"
#include "lib/config.h"
#include "lib/armVIC.h"
#include "lib/sysTime.h"
#include "lib/uart.h"
#include "lib/i2c.h"
#else
#include <stdio.h>
#include <stdlib.h>
#endif
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#ifndef __BORLANDC__
#ifndef __LINUX
/******************************************************************************
 *
 * Function Name: lowInit()
 *
 * Description:
 *    This function starts up the PLL then sets up the GPIO pins before
 *    waiting for the PLL to lock.  It finally engages the PLL and
 *    returns
 *
 * Calling Sequence:
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
static void lowInit(void)
{
  // set PLL multiplier & divisor.
  // values computed from config.h
  PLLCFG = PLLCFG_MSEL | PLLCFG_PSEL;

  // enable PLL
  PLLCON = PLLCON_PLLE;
  PLLFEED = 0xAA;                       // Make it happen.  These two updates
  PLLFEED = 0x55;                       // MUST occur in sequence.

  // setup the parallel port pin
  IOCLR = PIO_ZERO_BITS;                // clear the ZEROs output
  IOSET = PIO_ONE_BITS;                 // set the ONEs output
  IODIR = PIO_OUTPUT_BITS;              // set the output bit direction

  PINSEL0 = 0;
  PINSEL1 = 0;
  // P1.26 : P1.31  - JTAG
  // P2.0 : P2.31   - Data bus
  // P3.1 : P3.22   - Address bus
  // P1.0           - CS0 (Flash)
  // P3.26          - CS1 (RAM)
  // P3.31          - BLS0
  // P3.30          - BLS1
  // P3.29          - BLS2
  // P3.28          - BLS3
  // P3.26          - WE
  // P1.1           - OE
  PINSEL2 = 0x0F000924;           //0000 1111 0000 0000 0000 1001 0010 0100  
  IO0DIR = 0x60000001;            //0110 0000 0000 0000 0000 0000 0000 0001 - LED,TXD0,PULL_PIN0
  // All pins are output
  IO1DIR = 0x02000003;            //0000 0010 0000 0000 0000 0000 0000 0011 - PULL_PIN_P1, CS0, OE

  // wait for PLL lock
  while (!(PLLSTAT & PLLSTAT_LOCK))
    continue;

  VPBDIV = 0x11;                // set the peripheral bus clock speed

  // Initialization of MAM
  // Initialization of EMC
  // FLASH 16bit; IDCY = F; WST1 = 4; RBLE = 1; WST2 = 6
  BCFG0=0x1000348F;
  // SRAM 32bit; IDCY = F; WST1 = 0; RBLE = 1; WST2 = 0
  BCFG1=0x20001440;
  // setup & enable the MAM
  MAMCR = 0;
  MAMTIM = 4;
  MAMCR = 1;
}

/******************************************************************************
 *
 * Function Name: sysInit()
 *
 * Description:
 *    This function is responsible for initializing the program
 *    specific hardware
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
static void sysInit(void)
{
  lowInit();                            // setup clocks and processor port pins

  // set the interrupt controller defaults
#if defined(RAM_RUN)
  MEMMAP = MEMMAP_SRAM;                 // map interrupt vectors space into SRAM
#elif defined(ROM_RUN)
  MEMMAP = MEMMAP_FLASH;                // map interrupt vectors space into FLASH
#else
#error RUN_MODE not defined!
#endif
  VICIntEnClear = 0xFFFFFFFF;           // clear all interrupts
  VICIntSelect = 0x00000000;            // clear all FIQ selections
  VICDefVectAddr = (uint32_t)reset;     // point unvectored IRQs to reset()

//  wdtInit();                            // initialize the watchdog timer

  initSysTime();                        // initialize the system timer
  uart0Init(UART_BAUD(HOST_BAUD), UART_8N1, UART_FIFO_8); // setup the UART
//  uart1Init(UART_BAUD(HOST_BAUD), UART_8N1, UART_FIFO_8); // setup the UART
}
#else

#endif

#endif

void function_start(int flag, char * name, char *buff);
void vm_init(void);

#ifdef __LINUX
void main(int argc, char *argv[])
#else
void main(void)
#endif
{
#ifndef __BORLANDC__
#ifndef __LINUX
  sysInit();
  i2c_init();
#if defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE)
  enableIRQ();
#endif
  uart0Puts("\r\nHello World!\r\n");
#else
  printf("\r\nHello World!\r\n");
#endif
#else
  printf("\r\nHello World!\r\n");
#endif
#ifdef __LINUX
  if(argc<2) {
    printf("Use unit1 arg1 arg2 ... \r\n");
    exit(0);
  }; 
#endif
  vm_init();
/*
#ifndef __BORLANDC__
  uart0Puts("\r\n>");
#else
  printf("\r\n>");
#endif
*/
#ifdef __LINUX
//printf("%s %s \n",argv[0],argv[1]); 
  function_start(1,argv[1],NULL);
#else
  function_start(0,0,0);
#endif
};
