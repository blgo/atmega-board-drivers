#include <avr/interrupt.h>
#include <avr/io.h>

void init_timer(void){
	//Normal mode with default values
	//Toggle OCOA on compare mach
	TCCROA = _BV(COMOA1) | _BV(COMOA0);
	//Set preescaler at clk/1024
	TCCROB = _BV(CSO2) & ~_BV(CSO1) | _BV(CSO0);
	//Compare match on TOP
	OCROA = TOP;
	//when OCIEOA is HI Interrupt enable for OCOA, load PCINT7 interrupt on compare mach
	TIMSKO |= _BV(OCIEOA);
}

ISR(PCINT7_vect){
	float N=0;
	DDRH= 0xFF	
	if(N % 2 == 0) PORTH = _BV(i);
	N++;
}

int main(void){
	while(1){
				
	}
}
