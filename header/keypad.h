
#ifndef KEYPAD_H
#define KEYPAD_H

#include <avr/io.h>
#include "bit.h"

// Returns '\0' if no key pressed, else returns char '1', '2', ..., '#', 'D'
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port c
/* Keypad Arrangement
        PC4 PC5 PC6 PC7
   col  1   2   3   4
row
PC0 1   1 | 2 | 3 | A
PC1 2   4 | 5 | 6 | B
PC2 3   7 | 8 | 9 | C
PC3 4   * | 0 | # | D
*/

unsigned char GetKeypadKey() {
    PORTA = 0xEF;   // Enable col 4 with 0, disable others with 1's
    asm("nop");     // add a delay to allow PORTA to stablize before checking
    if (GetBit(PINA, 0) == 0) { return '1'; }
    if (GetBit(PINA, 1) == 0) { return '4'; }
    if (GetBit(PINA, 2) == 0) { return '7'; }
    if (GetBit(PINA, 3) == 0) { return '*'; }

    // Check keys  in col 2
    PORTA = 0xDF;   // Enable col 5 with 0, disable others with 1's
    asm("nop");
    if (GetBit(PINA, 0) == 0) { return '2'; }
    if (GetBit(PINA, 1) == 0) { return '5'; }
    if (GetBit(PINA, 2) == 0) { return '8'; }
    if (GetBit(PINA, 3) == 0) { return '0'; }

    // Check keys in col 3
    PORTA = 0xBF;
    asm("nop");
    if (GetBit(PINA, 0) == 0) { return '3'; }
    if (GetBit(PINA, 1) == 0) { return '6'; }
    if (GetBit(PINA, 2) == 0) { return '9'; }
    if (GetBit(PINA, 3) == 0) { return '#'; }

    // Check keys in col 3
    PORTA = 0x7F;
    asm("nop");
    if (GetBit(PINA, 0) == 0) { return 'A'; }
    if (GetBit(PINA, 1) == 0) { return 'B'; }
    if (GetBit(PINA, 2) == 0) { return 'C'; }
    if (GetBit(PINA, 3) == 0) { return 'D'; }

    return '\0';   // default value
}

#endif /* end of include guard: KEYPAD_H */
