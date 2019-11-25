
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "keypad.h"
#include "tasks.h"
#include "timer.h"

typedef enum keypad_states {wait, buttonPress};

int keypadTick(int state);

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	unsigned char i;
	static Task task1;
	Task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*)

	task1.state = wait;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadTick;

	unsigned long GCD = tasks[0]->period;
	for (i = 1; i < numTasks; ++i) {
		GCD = findGCD(GCD, tasks[i]->period);
	}

	TimerSet(GCD);
	TimerOn();

	while(1) {


		for (i = 0; i < numTasks; i++) {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += GCD;
		}

		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}


int keypadTick(int state) {
	unsigned char x = GetKeypadKey();

	switch (state) {
		case wait:
		if (x == '\0') { state = wait; }
		else { state = buttonPress; }
		break;

		case buttonPress:
		if (x == '\0') { state = wait; }
		else { state = buttonPress; }
		break;

		default:
		state = wait;
		break;
	}

	switch (state) {
		case wait:  PORTB = 0x1F; break;

		case buttonPress:
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
		break;
	}


	return state;
}
