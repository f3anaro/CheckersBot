/*
 * CBitBoard.cpp
 *
 *  Created on: 22.03.2013
 *      Author: Lucas
 */

#include <assert.h>
#include <stdio.h>
#include <bitset>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "CBitBoard.h"
#include "my_player.h"
#include "defines.h"

using namespace std;


/* -----------------------------------------------------------------------
 * Bitboard Bitmasks
 * ----------------------------------------------------------------------- */

const uint32_t CBitBoard::BOARD_MASK[] = {
	1 <<  0, 1 <<  1, 1 <<  2, 1 <<  3,
	1 <<  4, 1 <<  5, 1 <<  6, 1 <<  7,
	1 <<  8, 1 <<  9, 1 << 10, 1 << 11,
	1 << 12, 1 << 13, 1 << 14, 1 << 15,
	1 << 16, 1 << 17, 1 << 18, 1 << 19,
	1 << 20, 1 << 21, 1 << 22, 1 << 23,
	1 << 24, 1 << 25, 1 << 26, 1 << 27,
	1 << 28, 1 << 29, 1 << 30, 1 << 31
};

const uint32_t CBitBoard::MASK_LEFT_SHIFT_3 = 	BOARD_MASK[1]  | BOARD_MASK[2]  | BOARD_MASK[3]  |
												BOARD_MASK[9]  | BOARD_MASK[10] | BOARD_MASK[11] |
												BOARD_MASK[17] | BOARD_MASK[18] | BOARD_MASK[19] |
												BOARD_MASK[25] | BOARD_MASK[26] | BOARD_MASK[27];

const uint32_t CBitBoard::MASK_LEFT_SHIFT_5  = 	BOARD_MASK[4]  | BOARD_MASK[5]  | BOARD_MASK[6]  |
												BOARD_MASK[12] | BOARD_MASK[13] | BOARD_MASK[14] |
												BOARD_MASK[20] | BOARD_MASK[21] | BOARD_MASK[22];

const uint32_t CBitBoard::MASK_RIGHT_SHIFT_3 = 	BOARD_MASK[4]  | BOARD_MASK[5]  | BOARD_MASK[6]  |
												BOARD_MASK[12] | BOARD_MASK[13] | BOARD_MASK[14] |
												BOARD_MASK[20] | BOARD_MASK[21] | BOARD_MASK[22] |
												BOARD_MASK[28] | BOARD_MASK[29] | BOARD_MASK[30];

const uint32_t CBitBoard::MASK_RIGHT_SHIFT_5 =	BOARD_MASK[9]  | BOARD_MASK[10] | BOARD_MASK[11] |
												BOARD_MASK[17] | BOARD_MASK[18] | BOARD_MASK[19] |
												BOARD_MASK[25] | BOARD_MASK[26] | BOARD_MASK[27];

const uint32_t CBitBoard::MASK_KINGS_ROW_BLACK = BOARD_MASK[0]  | BOARD_MASK[1]  | BOARD_MASK[2]  | BOARD_MASK[3];
const uint32_t CBitBoard::MASK_KINGS_ROW_WHITE = BOARD_MASK[28] | BOARD_MASK[29] | BOARD_MASK[30] | BOARD_MASK[31];

/* -----------------------------------------------------------------------
 * Class Methods
 * ----------------------------------------------------------------------- */

CBitBoard::CBitBoard() {
	unserialize("bbbbbbbbbbbb--------wwwwwwwwwwww");
}

CBitBoard::CBitBoard(const char * const string) {
	unserialize(string);
}

void CBitBoard::unserialize(char const * const buffer) {
	int length = strlen(buffer);

	// validate string length
	assert (strlen(buffer) == 32);

	// reset all fields
	blackPieces = 0;
	whitePieces = 0;
	kings = 0;

	for (int i = 0; i < length; i++) {
		switch (buffer[i]) {
			case 'b' :
				blackPieces |= BOARD_MASK[i];
				break;

			case 'w' :
				whitePieces |= BOARD_MASK[i];
				break;

			case 'B' :
				blackPieces |= BOARD_MASK[i];
				kings |= BOARD_MASK[i];
				break;

			case 'W' :
				whitePieces |= BOARD_MASK[i];
				kings |= BOARD_MASK[i];
				break;

			case '-' :
				break;

			default :
				error ("unknplayer char '%c' in input string\n", buffer[i]);
				break;
		}
	}
}

string CBitBoard::serialize() {
	string serialized;

	for (int i=0; i < 32; i++) {

		// piece is set on the white bitboard
		if ((whitePieces & BOARD_MASK[i]) == BOARD_MASK[i]) {

			// piece is white and on kings bitboard
			if ((kings & BOARD_MASK[i]) == BOARD_MASK[i]) {
				serialized += 'W';
			}
			// white piece is not on kings board
			else {
				serialized += 'w';
			}

		}
		// piece is on black bitboard
		else if ((blackPieces & BOARD_MASK[i]) == BOARD_MASK[i]) {

			// piece is black and on kings board
			if ((kings & BOARD_MASK[i]) == BOARD_MASK[i]) {
				serialized += 'B';
			}
			// normal black piece
			else {
				serialized += 'b';
			}
		}
		// field is either on black or white bitboard. field is empty.
		else {
			serialized += '-';
		}
	}
	return serialized;
}

void CBitBoard::draw(void) {
//	bitset<32> whiteBits(whitePieces);
//	bitset<32> blackBits(blackPieces);
//	bitset<32> kingBits(kings);
//
//	cout << "White Bitboard: " << whiteBits << endl
//		<< "Black Bitboard: " << blackBits << endl
//		<< "Kings Bitboard: " << kingBits  << endl << endl;

	cout << endl;

	for (int r=7; r >= 0; r--) {
		printf ("+----+----+----+----+----+----+----+----+\n");

		// even row
		// r == 0 (mod 2)
		if ((r & 1) != 0) {
			cout << "|    ";
		}

		for (int j = 0; j < 4; j++) {
			int i = 4 * r + j;

			char c = ' ';

			if (BOARD_MASK[i] & blackPieces)
				c = 'b';

			if (BOARD_MASK[i] & whitePieces)
				c = 'w';

			// capitalize
			if (BOARD_MASK[i] & kings)
				c -= 32;

			printf("|%c%2d%c", c, i+1, c);

			// no end of line or even row
			if ((i & 7) != 7 || (r & 1) == 0) {
				cout << "|    ";
			}

		}

		// odd row
		cout << "|\n";


	}
	cout << "+----+----+----+----+----+----+----+----+" << endl << endl;
}


uint32_t CBitBoard::getMoversWhite (void) {
	const uint32_t freeSquares = ~(whitePieces | blackPieces);
	const uint32_t whiteKings = whitePieces & kings;

//	bitset<32> myset (freeSquares);
//	myset.count();

	// move all free squares +4 and check if this square contains a white piece
	uint32_t movers = (freeSquares << 4) & whitePieces;
	movers |= ((freeSquares & MASK_LEFT_SHIFT_3) << 3) & whitePieces;
	movers |= ((freeSquares & MASK_LEFT_SHIFT_5) << 5) & whitePieces;

	// if white kings exist, take also the movements backwards
	if (whiteKings) {
		movers |= (freeSquares >> 4) & whiteKings;
		movers |= ((freeSquares & MASK_RIGHT_SHIFT_3) >> 3) & whiteKings;
		movers |= ((freeSquares & MASK_RIGHT_SHIFT_5) >> 5) & whiteKings;
	}
	return movers;
}

uint32_t CBitBoard::getMoversBlack (void) {
	// for proper comments take a look at CBitBoard::getMoversWhite()
	// this function is the mirror-symmetric to it.

	const uint32_t freeSquares = ~(whitePieces | blackPieces);
	const uint32_t blackKings = whitePieces & kings;

	uint32_t movers = (freeSquares << 4) & blackPieces;
	movers |= ((freeSquares & MASK_RIGHT_SHIFT_3) >> 3) & blackPieces;
	movers |= ((freeSquares & MASK_RIGHT_SHIFT_5) >> 5) & blackPieces;

	if (blackKings) {
		movers |= (freeSquares << 4) & blackKings;
		movers |= ((freeSquares & MASK_LEFT_SHIFT_3) << 3) & blackKings;
		movers |= ((freeSquares & MASK_LEFT_SHIFT_5) << 5) & blackKings;
	}

	return movers;
}

void CBitBoard::getMoveList (Player player, MoveList *moveList) {
	switch (player) {

		case BLACK:
			getMoveListBlack(moveList);
			break;

		case WHITE:
			getMoveListWhite(moveList);
			break;

		default:
			error("Invalid player");
			break;
	}
}

void CBitBoard::getMoveListWhite(MoveList *moveList) {
	const uint32_t freeSquares = ~(whitePieces | blackPieces);
	const uint32_t whiteKings = whitePieces & kings;

	// bitmask of all white moves for "square + 4"
	moveList->addMoves((freeSquares << 4) & whitePieces, -4);

	// bitmask of all white moves for "square + 3"
	moveList->addMoves(((freeSquares & MASK_LEFT_SHIFT_3) << 3) & whitePieces, -3);

	// bitmask of all white moves for "square + 5"
	moveList->addMoves(((freeSquares & MASK_LEFT_SHIFT_5) << 5) & whitePieces, -5);

	// all possible moves for kings
	if (whiteKings) {
		moveList->addMoves((freeSquares >> 4) & whiteKings, 4);
		moveList->addMoves(((freeSquares & MASK_RIGHT_SHIFT_3) >> 3) & whiteKings, 3);
		moveList->addMoves(((freeSquares & MASK_RIGHT_SHIFT_5) >> 5) & whiteKings, 5);
	}
}

void CBitBoard::getMoveListBlack(MoveList *moveList) {
	const uint32_t freeSquares = ~(whitePieces | blackPieces);
	const uint32_t blackKings = blackPieces & kings;

	// bitmask of all white moves for "square - 4"
	moveList->addMoves((freeSquares >> 4) & blackPieces, 4);

	// bitmask of all white moves for "square - 3"
	moveList->addMoves(((freeSquares & MASK_RIGHT_SHIFT_3) >> 3) & blackPieces, 3);

	// bitmask of all white moves for "square - 5"
	moveList->addMoves(((freeSquares & MASK_RIGHT_SHIFT_5) >> 5) & blackPieces, 5);

	// all possible moves for kings
	if (blackKings) {
		moveList->addMoves((freeSquares << 4) & blackKings, -4);
		moveList->addMoves(((freeSquares & MASK_LEFT_SHIFT_3) << 3) & blackKings, -3);
		moveList->addMoves(((freeSquares & MASK_LEFT_SHIFT_5) << 5) & blackKings, -5);
	}
}

uint32_t CBitBoard::getJumpers (Player player) {
	return (player == WHITE) ? getJumpersWhite() : getJumpersBlack();
}

uint32_t CBitBoard::getJumpersWhite () {
	const uint32_t freeSquares = ~(whitePieces | blackPieces);
	const uint32_t whiteKings = whitePieces & kings;

	uint32_t jumpers = 0;

	// get all capturable black pieces in "square + 4" direction
	uint32_t capturableSqaures = (freeSquares << 4) & blackPieces;

	// if capturable black pieces exist, mark every white piece able to move in this direction as jumper.
	// This "if" case is optional, but for speed up because the jumper bit operations in this case are expensive
	if (capturableSqaures)
		jumpers |= (((capturableSqaures & MASK_LEFT_SHIFT_3) << 3) | ((capturableSqaures & MASK_LEFT_SHIFT_5) << 5)) & whitePieces;

	// get all capturable black pieces in "square + 3|5" direction
	capturableSqaures = (((freeSquares & MASK_LEFT_SHIFT_3) << 3) | (freeSquares & MASK_LEFT_SHIFT_5) << 5) & blackPieces;
	jumpers |= (capturableSqaures << 4) & whitePieces;

	// make the backward jumps for kings only if kings exist
	if (whiteKings) {
		capturableSqaures = (freeSquares >> 4) & blackPieces;

		// if capturable black pieces exists in the "square - 4" diretion, search for all kings in this directions
		if (capturableSqaures)
			jumpers |= (((capturableSqaures & MASK_RIGHT_SHIFT_3) >> 3) | (capturableSqaures & MASK_RIGHT_SHIFT_5) >> 5) & whiteKings;

		// search for capturable black pieces in "square - 3|5" direction
		capturableSqaures = (((freeSquares & MASK_RIGHT_SHIFT_3 >> 3) | (freeSquares & MASK_RIGHT_SHIFT_5) >> 5)) & blackPieces;
		jumpers |= (capturableSqaures >> 4) & whiteKings;
	}
	return jumpers;
}

uint32_t CBitBoard::getJumpersBlack () {
	const uint32_t freeSquares = ~(whitePieces | blackPieces);
	const uint32_t blackKings = blackPieces & kings;

	uint32_t jumpers = 0;

	// get all capturable white pieces in "square - 4" direction
	uint32_t capturableSqaures = (freeSquares >> 4) & whitePieces;

	// if capturable white pieces exist, mark every black piece able to move in this direction as jumper.
	// This "if" case is optional, but for speed up because the jumper bit operations in this case are expensive
	if (capturableSqaures)
		jumpers |= (((capturableSqaures & MASK_RIGHT_SHIFT_3) >> 3) | ((capturableSqaures & MASK_RIGHT_SHIFT_5) >> 5)) & blackPieces;

	// get all capturable white pieces in "square - 3|5" direction
	capturableSqaures = (((freeSquares & MASK_RIGHT_SHIFT_3) >> 3) | (freeSquares & MASK_RIGHT_SHIFT_5) >> 5) & whitePieces;
	jumpers |= (capturableSqaures >> 4) & blackPieces;

	// make the backward jumps for kings only if kings exist
	if (blackKings) {
		capturableSqaures = (freeSquares << 4) & whitePieces;

		// if capturable white pieces exists in the "square + 4" diretion, search for all kings in this directions
		if (capturableSqaures)
			jumpers |= (((capturableSqaures & MASK_LEFT_SHIFT_3) << 3) | (capturableSqaures & MASK_LEFT_SHIFT_5) << 5) & blackKings;

		// search for capturable black pieces in "square + 3|5" direction
		capturableSqaures = (((freeSquares & MASK_LEFT_SHIFT_3 << 3) | (freeSquares & MASK_LEFT_SHIFT_5) << 5)) & whitePieces;
		jumpers |= (capturableSqaures >> 4) & blackKings;
	}
	return jumpers;
}

void CBitBoard::getJumpList (Player player, MoveList *jumpList, uint32_t filter) {

	switch (player) {

			case BLACK:
				getJumpListBlack(jumpList, filter);
				break;

			case WHITE:
				getJumpListWhite(jumpList, filter);
				break;

			default:
				error("Invalid player");
				break;
		}

}

void CBitBoard::getJumpListWhite(MoveList *jumpList, uint32_t filter) {
	const uint32_t whitePieces = this->whitePieces & filter;
	const uint32_t freeSquares = ~(whitePieces | blackPieces);
	const uint32_t whiteKings = whitePieces & kings;

//	bitset<32> filterBits(filter);
//	cout << "Filter: " << filterBits << endl;


	// capturable squares in "square + 4" direction
	uint32_t capturableSqaures = (freeSquares << 4) & blackPieces;

	// if capturable squares in "square + 4" direction exist
	if (capturableSqaures) {
		jumpList->addMoves(((capturableSqaures & MASK_LEFT_SHIFT_3) << 3) & whitePieces, -3 - 4);
		jumpList->addMoves(((capturableSqaures & MASK_LEFT_SHIFT_5) << 5) & whitePieces, -5 - 4);
	}

	// capturable squares in "square + 3" direction
	capturableSqaures = ((freeSquares & MASK_LEFT_SHIFT_3) << 3) & blackPieces;
	jumpList->addMoves((capturableSqaures << 4) & whitePieces, -4 - 3);

	// capturable squares in "square + 5" direction
	capturableSqaures = ((freeSquares & MASK_LEFT_SHIFT_5) << 5) & blackPieces;
	jumpList->addMoves((capturableSqaures << 4) & whitePieces, -4 - 5);

	if (whiteKings) {
		// capturable squares in "square - 4" direction
		capturableSqaures = (freeSquares >> 4) & blackPieces;

		if (capturableSqaures) {
			jumpList->addMoves(((capturableSqaures & MASK_RIGHT_SHIFT_3) >> 3) & whiteKings, 3 + 4);
			jumpList->addMoves(((capturableSqaures & MASK_RIGHT_SHIFT_5) >> 5) & whiteKings, 5 + 4);
		}

		// capturable squares in "square - 3" direction
		capturableSqaures = ((freeSquares & MASK_RIGHT_SHIFT_3) >> 3) & blackPieces;
		jumpList->addMoves((capturableSqaures >> 4) & whiteKings, 4 + 3);

		// capturable squares in "square - 5" direction
		capturableSqaures = ((freeSquares & MASK_RIGHT_SHIFT_5) >> 5) & blackPieces;
		jumpList->addMoves((capturableSqaures >> 4) & whiteKings, 4 + 5);
	}
}

void CBitBoard::getJumpListBlack(MoveList *jumpList, uint32_t filter) {
	const uint32_t blackPieces = this->blackPieces & filter;
	const uint32_t freeSquares = ~(whitePieces | blackPieces);
	const uint32_t blackKings = blackPieces & kings;

	// capturable squares in "square - 4" direction
	uint32_t capturableSqaures = (freeSquares >> 4) & whitePieces;

	// if capturable squares in "square - 4" direction exist
	if (capturableSqaures) {
		jumpList->addMoves(((capturableSqaures & MASK_RIGHT_SHIFT_3) >> 3) & blackPieces, 3 + 4);
		jumpList->addMoves(((capturableSqaures & MASK_RIGHT_SHIFT_5) >> 5) & blackPieces, 5 + 4);
	}

	// capturable squares in "square - 3" direction
	capturableSqaures = ((freeSquares & MASK_RIGHT_SHIFT_3) >> 3) & whitePieces;
	jumpList->addMoves((capturableSqaures >> 4) & blackPieces, 4 + 3);

	// capturable squares in "square - 5" direction
	capturableSqaures = ((freeSquares & MASK_RIGHT_SHIFT_5) >> 5) & whitePieces;
	jumpList->addMoves((capturableSqaures >> 4) & blackPieces, 4 + 5);

	if (blackKings) {
		// capturable squares in "square + 4" direction
		capturableSqaures = (freeSquares << 4) & whitePieces;

		if (capturableSqaures) {
			jumpList->addMoves(((capturableSqaures & MASK_LEFT_SHIFT_3) << 3) & blackKings, -3 - 4);
			jumpList->addMoves(((capturableSqaures & MASK_LEFT_SHIFT_5) << 5) & blackKings, -5 - 4);
		}

		// capturable squares in "square + 3" direction
		capturableSqaures = ((freeSquares & MASK_LEFT_SHIFT_3) << 3) & whitePieces;
		jumpList->addMoves((capturableSqaures << 4) & blackPieces, -4 - 3);

		// capturable squares in "square - 5" direction
		capturableSqaures = ((freeSquares & MASK_RIGHT_SHIFT_5) << 5) & whitePieces;
		jumpList->addMoves((capturableSqaures << 4) & blackKings, -4 - 5);
	}
}

void CBitBoard::executeMove (Move &move) {
	printf("Execute move: %d-%d\n", move.start + 1, move.target + 1);
	uint32_t startMask = BOARD_MASK[move.start];
	uint32_t targetMask = BOARD_MASK[move.target];

	// store current board state in the move struct
	move.blackPiecesBefore = blackPieces;
	move.whitePiecesBefore = whitePieces;
	move.kingsBefore = kings;

	// set king to target position if start square was a king
	if (kings & startMask) {
		// remove king from start position
		kings &= ~(startMask);
		kings |= targetMask;
	}


	// white turn
	if (whitePieces & startMask) {
		whitePieces &= ~(startMask);
		whitePieces |= targetMask;

		// white piece will be crplayered
		if (targetMask & MASK_KINGS_ROW_BLACK)
			kings |= targetMask;

	}
	// black turn
	else {
		blackPieces &= ~(startMask);
		blackPieces |= targetMask;

		// black piece will be crplayered
		if (targetMask & MASK_KINGS_ROW_WHITE)
			kings |= targetMask;
	}
}

bool CBitBoard::executeJump (Jump &jump) {
	printf("Execute jump: %dx%d\n", jump.start + 1, jump.target + 1);

	// store board state in the jump struct
	jump.blackPiecesBefore = blackPieces;
	jump.whitePiecesBefore = whitePieces;
	jump.kingsBefore = kings;


	bool isKing = false;

	// define masks
	uint32_t startMask = BOARD_MASK[jump.start];

	// average value of start and target + (if the row is odd 1 | 0 otherwise)
	uint32_t capturedSquare = ((jump.start + jump.target) >> 1) + ((jump.start >> 2) & 1);

	uint32_t captureMask = ~(BOARD_MASK[capturedSquare]);
	uint32_t targetMask = BOARD_MASK[jump.target];
	bool isWhiteToken = ((startMask & whitePieces) != 0);

	if (isWhiteToken) {
		// moving white token to target
		whitePieces &= ~startMask;
		whitePieces |= targetMask;

		// remove captured black token
		blackPieces &= captureMask;
		cout << "Remove black token on " << capturedSquare + 1 << endl;
	} else {
		// moving black token to target
		blackPieces &= ~startMask;
		blackPieces |= targetMask;

		// remove captured white token
		whitePieces &= captureMask;
		cout << "Remove white token on " << capturedSquare + 1 << endl;
	}


	// check if jumping token is a king
	if (kings & startMask) {
		isKing = true;

		cout << "Start position " << jump.start + 1 << " is a king" << endl;

		// remove king from start position
		kings &= ~(startMask);

		// set king to target
		kings |= targetMask;

	}

	// remove captured king
	kings &= captureMask;


	// crplayering
	if (!isKing) {

		cout << "Jumping token is no king" << endl;

		if (isWhiteToken) {
			if (targetMask & MASK_KINGS_ROW_BLACK) {
				kings |= targetMask;
				cout << "White token was crplayered" << endl;
				return true;
			}
		} else {
			if (targetMask & MASK_KINGS_ROW_BLACK) {
				kings |= targetMask;
				cout << "Black token was crplayered" << endl;
				return true;
			}
		}
	}

	return false;

//	// white jumps. remove captured piece from black board
//	if (whitePieces & startMask) {
//		whitePieces |= setMask;
//		whitePieces &= ~startMask;
//		blackPieces &= captureMask;
//
//		// white piece will be crplayered
//		if (!isKing && (jump.target & MASK_KINGS_ROW_BLACK)) {
//			kings |= setMask;
//			return true;
//		}
//	}
//	// black jumps
//	else {
//		blackPieces |= setMask;
//		blackPieces &= ~startMask;
//		whitePieces &= captureMask;
//
//		// black piece will be crplayered
//		if (!isKing && (jump.target & MASK_KINGS_ROW_WHITE)) {
//			kings |= setMask;
//			return true;
//		}
//	}
//	return false;
}

void CBitBoard::revertMove (Move &move) {
//	printf("Revert Move: %d(-|x)%d\n", move.start + 1, move.target + 1);

	blackPieces = move.blackPiecesBefore;
	whitePieces = move.whitePiecesBefore;
	kings = move.kingsBefore;

	// TODO optional
	move.blackPiecesBefore = 0;
	move.whitePiecesBefore = 0;
	move.kingsBefore = 0;
}

uint32_t CBitBoard::getPossibleKingsBlack (void) {
	uint32_t freeKingSquares = ~(blackPieces | whitePieces) & MASK_KINGS_ROW_WHITE;
	uint32_t blackMen = (blackPieces & ~kings);

	uint32_t possibleKings = ((freeKingSquares >> 4) | ((freeKingSquares & MASK_RIGHT_SHIFT_3) >> 3)) & blackMen;

	return possibleKings;
}

uint32_t CBitBoard::getPossibleKingsWhite (void) {
	uint32_t freeKingSquares = ~(blackPieces | whitePieces) & MASK_KINGS_ROW_BLACK;
	uint32_t whiteMen = (whitePieces & ~kings);

	uint32_t possibleKings = ((freeKingSquares << 4) | ((freeKingSquares & MASK_LEFT_SHIFT_3) << 3)) & whiteMen;

	return possibleKings;
}

int CBitBoard::evaluate (Player player) {
	uint32_t playerBitboard, opponentBitboard;

	bitset<32> *playerMen, *playerKings, *playerMovers, *opponentMen, *opponentKings, *opponentMovers;
	bitset<32> *playerPossibleKings, *opponentPossibleKings;

	if (player == WHITE) {
		// white player
		playerBitboard = whitePieces;
		opponentBitboard = blackPieces;

		// get movers and possible kings for white
		playerMovers = new bitset<32>(getMoversWhite());
		playerPossibleKings = new bitset<32>(getPossibleKingsWhite());

		// get movers and possible kings for black
		opponentMovers = new bitset<32>(getMoversBlack());
		opponentPossibleKings = new bitset<32>(getPossibleKingsBlack());

	} else {
		// black player
		playerBitboard = blackPieces;
		opponentBitboard = whitePieces;

		// get movers and possible kings for black
		playerMovers = new bitset<32>(getMoversBlack());
		playerPossibleKings = new bitset<32>(getPossibleKingsBlack());

		// get movers and possible kings for white
		opponentMovers = new bitset<32>(getMoversWhite());
		opponentPossibleKings = new bitset<32>(getPossibleKingsWhite());
	}

	if (!playerBitboard || playerMovers == 0)
		return -INFINTY;

	if (!opponentBitboard || opponentMovers == 0)
		return INFINTY;

	// create player bitsets
	playerMen = new bitset<32>(playerBitboard & ~kings);
	playerKings = new bitset<32>(playerBitboard & kings);

	// create opponent bitsets
	opponentMen = new bitset<32>(opponentBitboard & ~kings);
	opponentKings = new bitset<32>(opponentBitboard & kings);

	int value = 4 * playerKings->count()  + 2 * playerPossibleKings->count()   + playerMen->count();
	value -=  4 * opponentKings->count()  + 2 * opponentPossibleKings->count() + opponentMen->count();

	// free allocated memory
	delete playerMen;
	delete playerKings;
	delete playerMovers;
	delete opponentMen;
	delete opponentKings;
	delete opponentMovers;
	delete playerPossibleKings;
	delete opponentPossibleKings;

	return value;
}

CBitBoard::~CBitBoard() {
	// TODO Auto-generated destructor stub
}

