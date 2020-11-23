/*	Author: abazu001
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [ Modify the keypad code to be in an SM task and add
    an LED on PB7 then will light up while any button is pressed, consider this
    immediate feedback to the user. Then, modify the keypad SM to utilize the
    simple task scheduler format. All code from here on out should use the task
    scheduler.]
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "keypad.h"
#include "tasks.h" // GCD() and Task struct
#include "timer.h" // all defs for timer stuff

enum keypad_states {wait, buttonPress};

int keypadTick(int state);

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;

	unsigned char i;
	static task task1, task2;
	task *tasks[] = { &task1, };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	task1.state = wait;
	task1.period = 50;
	task1.elapsedTime = task2.period;
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

//------------------------ SHARED VARIABLES ------------------------------------
static unsigned char x;

//------------------------------------------------------------------------------

int keypadTick(int state) {
	x = GetKeypadKey();
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

		case buttonPress: // Just to make sure each individual button is working
		switch (x) {      // properly.
			case '\0': PORTB = 0x00; break;
			case '1':  PORTB = 0x80; break;
			case '2':  PORTB = 0x80; break;
			case '3':  PORTB = 0x80; break;
			case '4':  PORTB = 0x80; break;
			case '5':  PORTB = 0x80; break;
			case '6':  PORTB = 0x80; break;
			case '7':  PORTB = 0x80; break;
			case '8':  PORTB = 0x80; break;
			case '9':  PORTB = 0x80; break;
			case 'A':  PORTB = 0x80; break;
			case 'B':  PORTB = 0x80; break;
			case 'C':  PORTB = 0x80; break;
			case 'D':  PORTB = 0x80; break;
			case '*':  PORTB = 0x80; break;
			case '0':  PORTB = 0x80; break;
			case '#':  PORTB = 0x80; break;

			default:   PORTB = 0x00; break;
		}
		break;
	}


	return state;
}
