#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>
#include <bitset>
#include <signal.h>
#include <sys/time.h>

#include "src/my_player.h"
#include "src/defines.h"

using namespace std;


const int FD_IN  = 3;
const int FD_OUT = 4;

const int BUFFERSIZE_IN  = 35;
const int BUFFERSIZE_OUT = 39;
const int BUFFERSIZE = BUFFERSIZE_IN < BUFFERSIZE_OUT ? BUFFERSIZE_OUT : BUFFERSIZE_IN;

void error (const char * fmt, ...) {
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


int leafs = 0;

int makeJump (Player player, CBitBoard &board, Jump jump, int depth, int alpha, int beta) {

	char const *playerName = (player == BLACK) ? "BLACK " : "WHITE";
	cout << "makeJump(" << playerName << ", alpha: " << alpha << ", beta: " << beta <<endl;


	// no crowing jump
	if (!board.executeJump(jump)) {
		cout << "No crowning jump. Try to continue" << endl;

		// compute continuing jumps
		MoveList continuingJumps;

		switch (player) {

			case BLACK:
				board.getJumpListBlack(&continuingJumps, board.BOARD_MASK[jump.target]);
				break;

			case WHITE:
				board.getJumpListWhite(&continuingJumps, board.BOARD_MASK[jump.target]);
				break;

			default:
				error("Invalid player");
				break;
		}

		// continue jumping
		if (!continuingJumps.empty()) {

			board.draw();
			cout << "continue jumping" << endl;

			// TODO alpha beta pruning?
			int maxValue = -INFINTY;

			while (!continuingJumps.empty()) {
				cout << "Continuing jump" << endl;
				// execute continuing jump
				Jump continueJump = continuingJumps.pop();
				int value = makeJump(player, board, continueJump, depth, alpha, beta);

				cout << "Jump " << jump.start+1 << "x" << jump.target+1 << " value: " << value << endl;

				// store always the best jump
				if (value > maxValue) {
					cout << "Better continuing jump: " << maxValue << " < " << value << endl;
					maxValue = value;
				}
			}
			// revert the initial jump and return the maximal value
			board.revertMove(jump);

			return maxValue;


		} else {
			cout << "No continuing jump available" << endl;
		}
	} else {
		cout << "Crowning jump. Do not continue jumping" << endl;
	}

	board.draw();

	// quiescence search
//	if (depth <= 0 && board.getJumpers(player) == 0) {
//	if (depth == 0) {
//		// TODO
//		leafs++;
//		return board.evaluate(player);
//	}

	// continue depth first search
	int value = -miniMax((Player) -player, board, depth-1, -beta, -alpha);

	cout << "Make Jump " << jump.start+1 << "x" << jump.target+1 << " end: value: " << value << endl;

	// revert the initial jump
	board.revertMove(jump);
	return value;
}

int miniMax (Player player, CBitBoard &board, int depth, int alpha, int beta) {

	char const *playerName = (player == BLACK) ? "BLACK " : "WHITE";
	cout << "miniMax(" << playerName << ", depth: " << depth << ", alpha: " << alpha << ", beta: " << beta << endl;

	// depth first search termination condition and quiescence search cancel condition
//	if (depth <= 0 && board.getJumpers(player) == 0) {
	if (depth == 0) {
		leafs++;
		int value = board.evaluate(player);
		cout << "Max. depth reached. Evaluate: " << value << endl;
		return value;
	}

	MoveList possibleMoves;
	board.getJumpList(player, &possibleMoves);

	// no jumps available
	if (possibleMoves.empty()) {

		cout << "No jumps available" << endl;

		board.getMoveList(player, &possibleMoves);

		// no jumps and moves possible
		if (possibleMoves.empty()) {
			cout << "No more moves available" << endl;

			leafs++;
			return board.evaluate(player);
		}

		cout << "Moves available" << endl;

		int maxValue = alpha;

		while (!possibleMoves.empty()) {
			Move move = possibleMoves.pop();
			board.executeMove(move);
			board.draw();
			int value = -miniMax((Player) -player, board, depth-1, -beta, -maxValue);
			board.revertMove(move);

			cout << "Move " << move.start+1 << '-' << move.target+1 << ". value: " << value << endl;

			if (value > maxValue) {
				cout << "Better than current max. value: " << maxValue << ". Replace it" << endl;
				maxValue = value;

				if (maxValue >= beta) {
					cout << "Opponent has a beneficial offensive:" << beta << " >= " << maxValue <<". Cutoff " << possibleMoves.size() << " moves" << endl;
					break;
				}

				// TODO den Zug merken

			}
		}

		// all possible beneficial moves executed
		return maxValue;
	}

	int maxValue = alpha;

	cout << "Jumps available" << endl;

	// execute all possible beneficial jumps
	while (!possibleMoves.empty()) {
		Jump jump = possibleMoves.pop();
		int value = makeJump(player, board, jump, depth, alpha, beta);

		cout << "Jump " << jump.start+1 << "x" << jump.target+1 << " value: " << value << endl;

		// better jump was found
		if (value > maxValue) {

			const char *playerName = (player == BLACK) ? "BLACK" : "WHITE";
			cout << "Better jump for " << playerName << " found. " << jump.start+1 << 'x' << jump.target+1 << ": " << value << " > " <<maxValue << endl;
			maxValue = value;

			// opponent has a beneficial offensive
			if (maxValue >= beta) {
				cout << "Opponent has a beneficial offensive:" << beta << " >= " << maxValue <<". Cutoff " << possibleMoves.size() << " jumps" << endl;
				break;
			}

			// TODO den Zug merken
		} else {
			cout << "Not better than " << maxValue << " >= " << value << endl;
		}
	}

	// all possible beneficial jumps executed
	return maxValue;

}

void signalHandler (int signum) {
	cout << "Iterrupt signal (" << signum << ") received." << endl;
	cout << "Current leaf count: " << leafs << endl;

	exit(signum);
}

int main(int argc, char *argv[]) {
	signal(SIGINT, signalHandler);

	CBitBoard *board = new CBitBoard();
	char buffer[BUFFERSIZE];

//	board->unserialize("----b-B--W--bb--bw--w-----------");
	board->draw();

//	MoveList moveList;
//	board->getMoveListWhite(&moveList);
//
//	cout << endl << "White Moves:" << endl;
//
//	while (!moveList.isEmpty()) {
//		Move move = moveList.pop();
//		printf("\t%d-%d\n", move.start + 1, move.target + 1);
//	}
//
//	board->getMoveListBlack(&moveList);
//	cout << endl << "Black Moves:" << endl;
//
//	while (!moveList.isEmpty()) {
//		Move move = moveList.pop();
//		printf("\t%d-%d\n", move.start + 1, move.target + 1);
//	}

//	bitset<32> jumpers(board->getJumpersWhite());
//	cout << jumpers << endl;
//
//	MoveList jumpList;
//	board->getJumpListWhite(&jumpList);
//	cout << "White jumps:" << endl;
//
//	while (!jumpList.empty()) {
//		Jump jump = jumpList.pop();
//		printf("\t%dx%d\n", jump.start + 1, jump.target + 1);
////		board->executeJump(jump);
////		board->draw();
////		board->revertMove(jump);
////		board->draw();
//	}
//
//	board->getJumpListBlack(&jumpList);
//	cout << "Black jumps:" << endl;
//
//	while (!jumpList.empty()) {
//		Jump jump = jumpList.pop();
//		printf("\t%dx%d\n", jump.start + 1, jump.target + 1);
//	}

	cout << "Start search" << endl;

	timeval stop, start;
	clock_t startSeconds, stopSeconds;

	startSeconds = clock();
	gettimeofday(&start, NULL);
	miniMax(BLACK, *board, 7, -INFINTY, INFINTY);
	stopSeconds = clock();
	gettimeofday(&stop, NULL);

	cout << "Leafs: " << leafs << endl;
	printf("Duration: %d.%lu seconds\n", (int) (stopSeconds - startSeconds), (stop.tv_usec - start.tv_usec));

	return 0;

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
