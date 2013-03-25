/*
 * main.h
 *
 *  Created on: 21.03.2013
 *      Author: Lucas
 */

#ifndef MY_PLAYER_
#define MY_PLAYER_

#include <stdint.h>
#include <string>
#include "CBitBoard.h"

const extern int BUFFERSIZE_IN ;
const extern int BUFFERSIZE_OUT;
const extern int BUFFERSIZE;

/**
 * This variable is true if the maximal computation time is exceeded.
 */
extern bool timeout;

/**
 * Prints an error message and stops the execution immediately.
 *
 * @param const char *fmt
 * @return void
 */
void error (const char *fmt, ...);

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
 * Writes a given string to the output stream. The output
 * must implement the following encoding:
 *
 * Moves from square A to square B:
 * 		"A-B\n"
 *
 * Jumps from square A over B to C:
 * 		"AxC\n"
 *
 * Multiple Jumps:
 * 		"AxCxE...\n"
 *
 * Regular expression for encoding: [0-9][0-9]?([-x][0-9][0-9]?)+
 *
 * @param char const buffer[]
 * @return void
 */
void output (std::string const &move);

int makeJump (uint32_t startField, uint32_t target, Player player, uint32_t depth, std::string &bestJump);

/**
 * NegaMax variation of the MiniMax algorithm.
 *
 * @return int
 */
int miniMax (Player player, CBitBoard &board, int depth, int alpha, int beta, std::string *bestMove);

#endif /* MY_PLAYER_ */
