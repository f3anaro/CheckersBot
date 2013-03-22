#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>

#include "src/my_player.h"
#include "src/Board.h"
#include "src/AbstractMove.h"

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


std::vector<AbstractMove> &generatePossibleMoves (Field player, Board &board) {

	std::vector<Jump> *jumps = new std::vector<Jump>();
	std::vector<SimpleMove> *moves = new std::vector<SimpleMove>();

	// iterate through all possible fields
	for (int i = 1; i < 64; i += 2) {

		// calculate left shift of the field
		int pos = i - ((i / 8) & 1);
		Field field = board.field[pos];

		// field does not contain players piece
		if (field & player != player)
			continue;

		// piece is a king
		if (field & KING == KING) {
			// search all possible jumps in the black and white direction
			jump_black(i, board, *jumps);
			jump_white(i, board, *jumps);

			if (jumps->empty()) {
				move_black(i, board, *moves);
				move_white(i, board, *moves);
			}
			continue;
		}

		// TODO possible loop unrolling?

		// fallback for normal pieces
		switch (player) {

			case BLACK:
				// search all possible jumps in black direction
				jump_black(i, board, *jumps);

				// if no jump was found, search also moves
				if (jumps->empty())
					move_black(i, board, *moves);

				break;

			case WHITE:
				jump_white(i, board, *jumps);

				if (jumps->empty())
					move_white(i, board, *moves);

				break;
		}

	}
	// no jumps exist
	if (jumps->empty()) {
		delete jumps;
		return moves;
	}

	// return possible jumps
	delete moves;
	return jumps;

}


struct move_t {
	int left;
	int right;
};


/**
 * Tries to move a white piece forward.
 *
 */
bool move_white_left (Board &board, int position) {
	if (position < 8)
		return false;

	// check if left field exists and is empty
	if (position % 8 != 0 && board.field[position - 9] == NONE) {
		board.field[position - 9] = board.field[position];
		board.field[position] = NONE;
		return true;
	}

	// moving left is not possible
	return false;
}

bool move_white_right (Board &board, int position) {
	if (position < 8)
		false;

	// check if right field exists and is empty
	if ((position + 1) % 8 != 0 && board.field[position - 7] == NONE) {
		board.field[position - 7] = board.field[position];
		board.field[position] = NONE;
		return true;
	}

	// moving right is not possible
	return false;

}

/**
 * @param Board &board
 * @param int position
 */
bool move_black_left (Board &board, int position) {
	if (position > 55)
		return false;

	// move left
	if (position % 8 == 0 && board.field[position + 7] == NONE) {
		board.field[position + 7] = board.field[position];
		board.field[position] = NONE;

		return true;
	}

	return false;
}

bool move_black_right (Board &board, int position) {
	if (position > 55)
		return false;

	// move right
	if ((position + 1) % 8 == 0 && board.field[position + 9] == NONE) {
		board.field[position + 9] = board.field[position];
		board.field[position] = NONE;

		return true;
	}

	// moving right not possible
	return false;
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

	std::vector<AbstractMove> *moves = generatePossibleMoves(player, board);

	std::vector<AbstractMove>::iterator iterator = moves->begin();
	Field opponent = (player == BLACK) ? WHITE : BLACK;

	while (iterator != moves->end()) {
		AbstractMove move = *iterator;

		move.execute(board);

		int value = -negaMax(board, opponent, depth - 1);

		move.revert(board);

		// better move
		if (value > maxValue) {
			maxValue = value;
		}
	}
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
