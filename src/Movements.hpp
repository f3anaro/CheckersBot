/*
 * Movements.h
 *
 *  Created on: 23.03.2013
 *      Author: Lucas Kahlert <lucas.kahlert@dragonwebdesigns.de>
 */

#ifndef MOVEMENTS_H_
#define MOVEMENTS_H_

#include <vector>
#include <stdint.h>

typedef struct Move Move, Jump;

/**
 * Basic data structure of a simple move from start square to
 * target square
 */
struct Move {
	/**
	 * Copy of the bitboard for all black pieces
	 * before the execution of the jump.
	 */
	uint32_t blackPiecesBefore;

	/**
	 * Copy of the bitboard for all white pieces
	 * before the execution of the jump.
	 */
	uint32_t whitePiecesBefore;

	/**
	 * Copy of the bitboard for all kings before
	 * the execution of the jump
	 */
	uint32_t kingsBefore;

	/**
	 * Number of the start square
	 */
	uint32_t start;

	/**
	 * Number of the target square
	 */
	uint32_t target;

	/**
	 * Struct constructor
	 */
	Move (uint32_t start, uint32_t target) {
		this->start = start;
		this->target = target;
		blackPiecesBefore = 0;
		whitePiecesBefore = 0;
		kingsBefore = 0;
	};
};

class MoveList: public std::vector<Move> {
	public:
		/**
		 * Adds each bit in the bitmask to the move list. The start
		 * position of the move is the position in the bitmask and
		 * the target if the move is calculated by "position + offset"
		 *
		 * @param uint32_t bitmask
		 * @param int offset
		 * @return void
		 */
		void addMoves (uint32_t bitmask, int offset) {
			for (int i = 0; bitmask != 0; i++) {

				// square is set. Store the move
				if (bitmask & 1) {
					push_back(Move(i, i + offset));
				}

				// go to next bit (square flag)
				bitmask >>= 1;
			}
		}

		/**
		 * Returns the last element of the move list and removes it from
		 * the list.
		 *
		 * @return Move
		 */
		inline Move pop () {
			Move move = back();
			pop_back();
			return move;
		}
};

#endif /* MOVEMENTS_H_ */
