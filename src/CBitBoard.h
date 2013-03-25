/*
 * CBitBoard.h
 *
 *  Created on: 22.03.2013
 *      Author: Lucas Kahlert <lucas.kahlert@dragonwebdesigns.de>
 */

#ifndef CBITBOARD_H_
#define CBITBOARD_H_

#include <stdint.h>
#include <string>
#include <string.h>
#include "Movements.h"

/**
 * Type definition for the player parameter. The signed representation is used
 * for simplicity. You can switch to opponent by multiplying with -1
 */
enum Player: signed short {
	BLACK = 1,
	WHITE = -1,
};

class CBitBoard {
	protected:
		/**
		 * Bitmask of all avaiable white pieces on the board.
		 * This includes normal pieces and kings.
		 */
		uint32_t whitePieces;

		/**
		 * Bitmask of all available black pieces on the board.
		 * This includes normal pieces and kings.
		 */
		uint32_t blackPieces;

		/**
		 * Bitmask of all available kings on the board. Both colors
		 * are included.
		 */
		uint32_t kings;

	public:
		/**
		 * Array of bitmasks for each field on the bitboard
		 */
		static const uint32_t BOARD_MASK[32];

		/**
		 * Bitmask identifying the fileds on the bitboard available for left shifting
		 * by 3 (that calculates "fieldNumber + 3")
		 */
		static const uint32_t MASK_LEFT_SHIFT_3;

		/**
		 * Bitmask identifying the fileds on the bitboard available for left shifting
		 * by 5 (that calculates "fieldNumber + 5")
		 */
		static const uint32_t MASK_LEFT_SHIFT_5;

		/**
		 * Bitmask identifying the fileds on the bitboard available for right shifting
		 * by 3 (that calculates "fieldNumber - 3")
		 */
		static const uint32_t MASK_RIGHT_SHIFT_3;

		/**
		 * Bitmask identifying the fields on the bitboard available for right shifting
		 * by 5 (that calculates "fieldNumber - 5")
		 */
		static const uint32_t MASK_RIGHT_SHIFT_5;

		/**
		 * Bitmask identifying all squares in the kings row of the black side
		 */
		static const uint32_t MASK_KINGS_ROW_BLACK;

		/**
		 * Bitmask identifying all squares in the kings row of the white side
		 */
		static const uint32_t MASK_KINGS_ROW_WHITE;

		/**
		 * Creates a new checker board with the standard starting
		 * positions
		 */
		CBitBoard();

		/**
		 * Initialize board from given situation. The string is given in form of
		 * a 32 char long string. For example:
		 *
		 * 		"bbbbbbbbbbbb--------wwwwwwwwwwww" (start position)
		 *
		 * An "b" means a black piece and "w" a white one. A capital char identifies
		 * a king.
		 *
		 * @param char const * const string
		 */
		CBitBoard (char const * const string);

		/**
		 * Copy operator
		 *
		 * @param Board const &b
		 * @return void
		 */
		CBitBoard (CBitBoard const &board) {
			// copy all bitboards for white, black and king pieces.
			memcpy(&whitePieces, &(board.whitePieces), sizeof(whitePieces));
			memcpy(&blackPieces, &(board.blackPieces), sizeof(blackPieces));
			memcpy(&kings, &(board.kings), sizeof(kings));
		}

		/**
		 * Unserializes a string to the board state.
		 *
		 * @param std::string const string	Input string representing the board state
		 * @return void
		 */
		void unserialize (char const * const buffer);

		/**
		 * Serializes the board state to string
		 *
		 * @return std::string
		 */
		std::string serialize (void);

		/**
		 * Prints the current board state to standard output
		 *
		 * @return void
		 */
		void draw(void);

		/**
		 * Returns a bitmask of all white pieces that could be moved.
		 *
		 * @return uint_32_t
		 */
		uint32_t getMoversWhite (void);

		/**
		 * Returns a bitmask of all black pieces could be moved. That includes
		 * normal pieces and kings.
		 *
		 * @return uint_32_t
		 */
		uint32_t getMoversBlack (void);

		/**
		 * Wrapper for getJumpersWhite() and getJumpersBlack()
		 *
		 * @param Player player
		 * @return uint32_t
		 */
		uint32_t getJumpers(Player player);

		/**
		 * Returns a bitmask of all white pieces that could execute a jump
		 *
		 * @return uint_32_t
		 */
		uint32_t getJumpersWhite (void);

		/**
		 * Return a bitmask of all black pieces that could execute a jump.
		 *
		 * @return uint32_t
		 */
		uint32_t getJumpersBlack (void);

		/**
		 * Returns a bitmask of all black tokens that could be crowned.
		 *
		 * @return uint32_t
		 */
		uint32_t getPossibleKingsBlack (void);

		/**
		 * Returns a bitmask of all white tokens that could be crowned.
		 *
		 * @return uint32_t
		 */
		uint32_t getPossibleKingsWhite (void);

		/**
		 * Wrapper method for getMoveListWhite() and getMoveListBlack()
		 *
		 * @param Player player
		 * @param MoveList *moveList
		 * @return void
		 */
		void getMoveList (Player player, MoveList *moveList);

		/**
		 * Stores all available moves for white pieces on the committed
		 * list of moves. This list contains normal and crowned pieces.
		 *
		 * @param MoveList<Move> *moveList
		 * @return void
		 */
		void getMoveListWhite (MoveList *moveList);

		/**
		 * Stores all available moves for black pieces in the committed
		 * list of moves. This list contains normal and crowned pieces.
		 *
		 * @param MoveList<Move> *moveList
		 * @return void
		 */
		void getMoveListBlack (MoveList *moveList);

		/**
		 * Wrapper method for getJumpListWhite() and getJumpListBlack()
		 *
		 * @param Player player
		 * @param MoveList *jumpList
		 * @param uint32_t filter = 0xffffffff
		 * @return void
		 */
		void getJumpList (Player player, MoveList *jumpList, uint32_t filter = 0xffffffff);

		/**
		 * Stores all available jumps for white pieces in the committed
		 * list. This list contains normal and crowned pieces. The filter
		 * is used to mask only selected squares. By default this mask is
		 * 0xffffffff, that means every square is selected.
		 *
		 * @param JumpList *jumpList
		 * @param uint32_t filter = 0xffffffff
		 * @return void
		 */
		void getJumpListWhite (MoveList *jumpList, uint32_t filter = 0xffffffff);

		/**
		 * Stores all available jumps for black pieces in the committed
		 * list. This list contains normal and crowned pieces. The filter
		 * is used to mask only selected squares. By default this mask is
		 * 0xffffffff, that means every square is selected.
		 *
		 * @param JumpList *jumpList
		 * @param uint32_t filter = 0xffffffff
		 * @return void
		 */
		void getJumpListBlack (MoveList *jumpList, uint32_t filter = 0xffffffff);

		/**
		 * Makes a move.
		 * jump struct.
		 *
		 * @param Move &move
		 * @return void
		 */
		void executeMove (Move &move);

		/**
		 *
		 * @param uint32_t position
		 * @param MoveList *jumpList
		 * @return void
		 */
		void getContinuingJumps (uint32_t position, MoveList *jumpList);

		/**
		 * Makes a jump. The board state before the jump will be stored in the
		 * jump struct.
		 * The method returns true if the piece was crowned.
		 *
		 * @param Jump &jump
		 * @return bool
		 */
		bool executeJump (Jump &jump);

		void revertMove (Move &move);

		/**
		 * Evaluates the current situation on the board from point of view of the
		 * committed player. A higher value means a better situation for the player.
		 *
		 * @return int
		 */
		int evaluate (Player player);

		/**
		 * Destructor
		 */
		virtual ~CBitBoard();
};

#endif /* CBITBOARD_H_ */
