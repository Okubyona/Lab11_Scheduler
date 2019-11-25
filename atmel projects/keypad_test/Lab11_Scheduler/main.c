
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "keypad.h"

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0;   // Current internal count of 1 ms ticks.

void TimerOn();
void TimerOff();
void TimerSet(unsigned long);
void TimerISR();
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--;   // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

int main(void) {
	TimerOn();
	TimerSet(10);

	unsigned char x;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	while(1) {
		x = GetKeypadKey();

		switch (x) {
			case '\0': PORTB = 0x1F; break;
			case '1':  PORTB = 0x01; break;
			case '2':  PORTB = 0x02; break;
			case '3':  PORTB = 0x03; break;
			case '4':  PORTB = 0x04; break;
			case '5':  PORTB = 0x05; break;
			case '6':  PORTB = 0x06; break;
			case '7':  PORTB = 0x07; break;
			case '8':  PORTB = 0x08; break;
			case '9':  PORTB = 0x09; break;
			case 'A':  PORTB = 0x0A; break;
			case 'B':  PORTB = 0x0B; break;
			case 'C':  PORTB = 0x0C; break;
			case 'D':  PORTB = 0x0D; break;
			case '*':  PORTB = 0x0E; break;
			case '0':  PORTB = 0x00; break;
			case '#':  PORTB = 0x0F; break;

			default:   PORTB = 0x1B; break;
		}

		//while(!TimerFlag) {}
		//TimerFlag = 0;
	}

	return 0;
}

void TimerOn() {
	TCCR1B = 0x0B;

	OCR1A = 125;

	TIMSK1 = 0x02;

	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;

}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}


void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
