
#include "lpc2468.h"
#include "adc.h"

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
