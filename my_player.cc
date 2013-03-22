#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>

#include "my_player.h"

// TODO int => short

const int FD_IN  = 3;
const int FD_OUT = 4;

const int BUFFERSIZE_IN  = 35;
const int BUFFERSIZE_OUT = 39;
const int BUFFERSIZE = BUFFERSIZE_IN < BUFFERSIZE_OUT ? BUFFERSIZE_OUT : BUFFERSIZE_IN;

static void error (const char * fmt, ...) {
    va_list vl;
    va_start (vl, fmt);
    vfprintf (stderr, fmt, vl);
    va_end (vl);
    exit (-1);
}

void input (char *buffer) {
	// pointer to last+1 char
	char* end = buffer + BUFFERSIZE_IN;

	while (buffer < end) {
		int bytes_read = read (FD_IN, buffer, end - buffer);
		if (bytes_read < 0) error ("error reading FD_IN\n");
		buffer += bytes_read;
	}
	end--;

	if (*end != '\n')
		error ("line does not end with newline\n");

	*end = 0;
}

void output (char const buffer[]) {
    int length = strlen(buffer);

    // write to output stream and validate count of written chars
    if (write (FD_OUT, buffer, length) != length)
    	error ("error writing FD_OUT\n");
}


int evaluate (Field player, Board &board) {
	int value = 0;

	// iterate through all fileds
	for (int i = 1; i < 64; i += 2) {
		Field field = board.field[i - ((i / 8) & 1)];

		// select players piece
		if (field & player == player) {

			// king
			if (field & KING == KING)
				value += 3;
			// normal pieces
			else
				value += 1;
		}
	}
	return value;
}

int negaMax (Board &board, Field player, int depth) {
	if (depth == 0)
		return evaluate(player, board);

	// initialize max. value
	int maxValue = -1000000;

//	std::vector<AbstractMove> *moves = generatePossibleMoves(player, board);
//
//	std::vector<AbstractMove>::iterator iterator = moves->begin();
//	Field opponent = (player == BLACK) ? WHITE : BLACK;
//
//	while (iterator != moves->end()) {
//		AbstractMove move = *iterator;
//
//		move.execute(board);
//
//		int value = -negaMax(board, opponent, depth - 1);
//
//		move.revert(board);
//
//		// better move
//		if (value > maxValue) {
//			maxValue = value;
//		}
//	}
	return maxValue;
}


int main() {
	Board* board = new Board();
	char buffer[BUFFERSIZE];

    while (1) {
    	input(buffer);

    	board->unserialize(buffer + 2);
    	board->draw();

    	printf ("[ %c ] board '%s'\n", buffer[0], buffer + 2);

		printf ("[ %c ] your move : ", buffer[0]);


		// request user input, read move into buffer
		if (!fgets(buffer, BUFFERSIZE_OUT, stdin))
			error ("fgets() failed");

		// send move back to MCP
		output (buffer);
    }
    // destruct board
    delete board;
}
