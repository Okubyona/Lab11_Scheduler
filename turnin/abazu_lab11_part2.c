
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "keypad.h"
#include "tasks.h"
#include "timer.h"
#include "io.c"

//typedef enum keypad_states {wait, buttonPress};
typedef enum scroll_states {init, cycle};

//int keypadTick(int state);
int displayMessage( int state);

#define SCREEN_WIDTH 16

int main(void) {
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	unsigned char i;
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	task1.state = init;
	task1.period = 350;
	task1.elapsedTime = task1.period;
	task1.TickFct = &displayMessage;

	unsigned long GCD = tasks[0]->period;
	for (i = 1; i < numTasks; ++i) {
		GCD = findGCD(GCD, tasks[i]->period);
	}

	LCD_init();

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


/*int keypadTick(int state) {
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
}*/


int displayMessage(int state) {
	char message[] = "CS120B is Legend... wait for it DARY!";
	static unsigned char cursor, startStr, stopStr;

	switch (state) {
		case init:
			state = cycle;
			cursor = 16;
			startStr = 0;
			stopStr = 1;
			break;

		case cycle:
			state = (startStr == strlen(message)) ? init: cycle;
			break;
	}

	switch (state) {
		case init:
			break;

		case cycle:
			LCD_ClearScreen();

			LCD_Cursor(cursor);
			LCD_Cursor(cursor);
			for (unsigned char i = startStr; i < stopStr; ++i) {
				if (i < strlen(message)) {
					LCD_WriteData(message[i]);
				}
			}

			if(cursor > 1 && startStr == 0) {
				++stopStr;
				--cursor;
			}
			else if (startStr == stopStr) {
				startStr = stopStr = 0;
				cursor = SCREEN_WIDTH;
			}
			else if (stopStr == strlen(message) && cursor == 0) {
				++startStr;
			}
			else {
				++startStr;
				++stopStr;
			}
			break;
	}
	return state;
}
