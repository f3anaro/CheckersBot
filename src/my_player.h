/*
 * main.h
 *
 *  Created on: 21.03.2013
 *      Author: Lucas
 */

#ifndef MY_PLAYER_
#define MY_PLAYER_

#include <stdint.h>
#include "Board.h"

const extern int BUFFERSIZE_IN ;
const extern int BUFFERSIZE_OUT;
const extern int BUFFERSIZE;

uint32_t MASK_LEFT_SHIFT_3, MASK_LEFT_SHIFT_5, MASK_RIGHT_SHIFT_3, MASK_RIGHT_SHIFT_5;
uint32_t MASK_BOARD[32];

MASK_BOARD[0] = 1;

for (int i = 1; i < 32; i++) {
	MASK_BOARD[i] = 2 * MASK_BOARD[i - 1];
}

static void error (const char * fmt, ...);

/**
 * Reads the board state from the input stream into an string
 * and returns it.
 *
 * Example: "B:bbbbbbbbbbbb--------wwwwwwwwwwww\n"
 *
 * @param char *buffer
 * @return void
 */
void input (char *buffer);

/**
 * Writes a given string to the output stream
 *
 * @param char const buffer[]
 * @return void
 */
void output (char const buffer[]);


#endif /* MY_PLAYER_ */
