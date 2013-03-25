#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <string>
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

void output (const string &move) {
    int length = move.length();

    // write to output stream and validate count of written chars
    if (write (FD_OUT, move.c_str(), length) != length)
    	error ("error writing FD_OUT\n");
}


int leafs = 0;

int makeJump (Player player, CBitBoard &board, Jump jump, int depth, int alpha, int beta, string &bestJump) {

	char const *playerName = (player == BLACK) ? "BLACK " : "WHITE";
	cout << "makeJump(" << playerName << ", alpha: " << alpha << ", beta: " << beta <<endl;

	// append target
	stringstream strStream;
	strStream << bestJump << 'x' << jump.target+1;
	bestJump = strStream.str();

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

			while (!continuingJumps.empty() && !timeout) {
				cout << "Continuing jump" << endl;
				// execute continuing jump
				Jump continueJump = continuingJumps.pop();

				string localCopy(bestJump);

				int value = makeJump(player, board, continueJump, depth, alpha, beta, localCopy);

				cout << "Jump " << localCopy << " value: " << value << endl;

				// store always the best jump
				if (value > maxValue) {
					maxValue = value;

					cout << "Better continuing jump: " << maxValue << " < " << value << endl;
					cout << "Former best jump: " << bestJump << " -> better jump: " << localCopy << endl;
					bestJump = localCopy;
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

	// continue depth first search
	int value = -miniMax((Player) -player, board, depth-1, -beta, -alpha, NULL);

	cout << "Make Jump " << jump.start+1 << "x" << jump.target+1 << " end: value: " << value << endl;

	// revert the initial jump
	board.revertMove(jump);
	return value;
}

int miniMax (Player player, CBitBoard &board, int depth, int alpha, int beta, string *bestMove) {
	char const *playerName = (player == BLACK) ? "BLACK " : "WHITE";
	cout << "miniMax(" << playerName << ", depth: " << depth << ", alpha: " << alpha << ", beta: " << beta << endl;

	// TODO quiescence search

	// depth first search termination condition and quiescence search cancel condition
	if (depth <= 0 && board.getJumpers(player) == 0) {
//	if (depth == 0 || timeout) {
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

		while (!possibleMoves.empty() && !timeout) {
			Move move = possibleMoves.pop();
			board.executeMove(move);
			board.draw();
			int value = -miniMax((Player) -player, board, depth-1, -beta, -maxValue, NULL);
			board.revertMove(move);

			cout << "Move " << move.start+1 << '-' << move.target+1 << ". value: " << value << endl;

			if (value > maxValue) {
				cout << "Better than current max. value: " << maxValue << ". Replace it" << endl;
				maxValue = value;

				if (maxValue >= beta) {
					cout << "Opponent has a beneficial offensive:" << beta << " >= " << maxValue <<". Cutoff " << possibleMoves.size() << " moves" << endl;
					break;
				}


				if (bestMove != NULL) {
					stringstream strStream;
					strStream <<  move.start+1 << '-' << move.target+1;
					*bestMove = strStream.str();
				}
			}
		}

		// all possible beneficial moves executed
		return maxValue;
	}

	int maxValue = alpha;

	cout << "Jumps available" << endl;

	// execute all possible beneficial jumps
	while (!possibleMoves.empty() && !timeout) {
		Jump jump = possibleMoves.pop();

		stringstream strStream;
		strStream <<  jump.start+1;
		string seriaizedJump = strStream.str();

		int value = makeJump(player, board, jump, depth, alpha, beta, seriaizedJump);

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

			if (bestMove != NULL) {
				cout << "Better jump: " << *bestMove << " -> " << seriaizedJump << endl;
				*bestMove = seriaizedJump;
			}
		} else {
			cout << "Not better than " << maxValue << " >= " << value << endl;
		}
	}

	// all possible beneficial jumps executed
	return maxValue;

}

bool timeout = false;

void signalHandlerSIGXCPU (int signum) {
	timeout = true;

	timeval signal;
	gettimeofday(&signal, NULL);

	cout << endl << "-----------------------------------" << endl;
	cout << "Timeout Signal on " << signal.tv_sec << '.' << signal.tv_usec << endl;
	cout << "-----------------------------------" << endl << endl;
}

void signalHandlerSIGINT (int signum) {
	cout << "Iterrupt signal (" << signum << ") received." << endl;
	cout << "Current leaf count: " << leafs << endl;

	exit(signum);
}

int main(int argc, char *argv[]) {

	// initialize SignalHandler
	signal(SIGINT,  signalHandlerSIGXCPU);
	signal(SIGXCPU, signalHandlerSIGXCPU);
//	signal(SIGINT,  signalHandlerSIGINT);

	CBitBoard board = CBitBoard();
	char buffer[BUFFERSIZE];

//	board.unserialize("----b-B--W--bb--bw--w-----------");
//	board.draw();
//
//	cout << "Start search" << endl;
//
//	timeval stop, start;
//
//	string bestMove;
//
//	gettimeofday(&start, NULL);
//	miniMax(BLACK, board, 12, -INFINTY, INFINTY, &bestMove);
//	gettimeofday(&stop, NULL);
//
//	board.draw();
//
//	cout << "Best Move: " << bestMove << endl;
//	cout << "Leafs: " << leafs << endl;
//	cout << "Duration: " << stop.tv_sec - start.tv_sec << "." << stop.tv_usec - start.tv_usec <<endl;
//	cout << "End: " << stop.tv_sec << '.' << stop.tv_usec << endl;
//
//	return 0;

	// TODO openings

    while (1) {
    	input(buffer);

    	board.unserialize(buffer + 2);
    	board.draw();

    	// define current player
    	Player player = (buffer[0] == 'B') ? BLACK : WHITE;
    	int depth = DEPTH_INIT;

    	string bestMove;

    	int maxValue = -INFINTY;

    	while (!timeout) {
    		cout << "Search with depth: " << depth << endl;

    		string move;
    		int value = miniMax(player, board, depth, -INFINTY, INFINTY, &move);

    		// storing the best move
    		if (value > maxValue) {
    			maxValue = value;
    			bestMove = move;
    		}
    		depth++;
    	}

		// send move back to MCP
		output (buffer);
    }
}
