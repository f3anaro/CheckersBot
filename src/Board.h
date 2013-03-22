/*
 * Board.h
 *
 *  Created on: 21.03.2013
 *      Author: Lucas Kahlert <lucas.kahlert@dragonwebdesigns.de>
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <vector>

enum Field {
    NONE  = 0,
    BLACK = 1,
    WHITE = 2,
    KING  = 4,
    BLACKKING = BLACK | KING,
    WHITEKING = WHITE | KING,
};

class Board {
    public:
		Field field[64];

        /**
         * Creates a new checker board with the standard starting
         * positions
         */
        Board() {
        	unserialize("bbbbbbbbbbbb--------wwwwwwwwwwww");
        }

        /**
         * Initialize board from given situation
         *
         * @param char const * const string
         */
        Board (char const * const string) {
        	unserialize(string);
        }

        /**
         * Copy operator
         *
         * @param Board const &b
         * @return void
         */
        Board (Board const &board) {
        	memcpy(&field, &(board.field), sizeof(field));
        }

        /**
         * Unserialize a string to the board state.
         *
         * @param std::string const string	Input string representing the board state
         * @return void
         */
        void unserialize (char const * const buffer);

        /**
         * Serialize board state to string
         *
         * @return std::string
         */
        std::string serialize (void);

        /**
         * Prints the current board state to standard output
         *
         * @return void
         */
        void draw();
};


#endif /* BOARD_H_ */
