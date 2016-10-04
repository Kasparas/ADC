#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "rprintf.h"
#include "type.h"
//#include "adc.h"
#include <timer_delay.h>
#include "config.h"

/* main loop
 * basic CPU initialization is done here.
 */
void adcInit(void) {

	PCONP |= 1 << 12; 		// Enable ADC power
	AD0CR |= 1 << 21; 		// ADC is now operational / disable power down
	AD0CR |= 19 << 8; 		// Set CLKDIV to divide (57,6MHz/2)/(19+1) = 1,44 MHz
	AD0CR |= 1 << 2;		// Select AD0.2 converter
	AD0CR |= 1 << 16;		// Select burst mode
	PINSEL1 |= 1 << 18; 	// Set P0.25 to analog input AD0
	PINMODE1 |= 0x10 << 18; // Remove pull-up and pull-down resistor

}

unsigned short adcRead(void) {

	while(!(AD0DR2 & (1 << 31))); //check if done

	return (AD0DR2 >> 6); // shift to "overwrite" unused bits
}

int main(void)
{

	/* init low level stuff */
	lowLevelInit();


	/*initialize uart #1: 115200 bps, 8N1*/
	initUart0(B115200(Fpclk), UART_8N1);

	/* initialize rprintf system */
	rprintfInit(putchar);

	/* print msg */
	rprintf("ADC test program \n");

	/* Init ADC*/
	adcInit();

	unsigned short result;
	float resultconv;
	unsigned short output;

	while (1)
	{
		delayMs(1,1000); // delay loop 1 s
		result = adcRead(); // read value from A/D converter
		resultconv = (float)result*3.2258; // convert register value to mV (10bit A/D = 1023 thats 3.3mV per step)
		output = (unsigned short)resultconv; // cast back to unsigned short 

	rprintf("value %d",output); // print out as decimal value

	}
	return 0;
}

