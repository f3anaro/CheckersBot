/*
 * SimpleMove.h
 *
 *  Created on: 21.03.2013
 *      Author: Lucas Kahlert <lucas.kahlert@dragonwebdesigns.de>
 */

#ifndef SIMPLEMOVE_H_
#define SIMPLEMOVE_H_

#include "AbstractMove.h"

class SimpleMove: public AbstractMove {
	protected:
		// target field number
		int to;

	public:
		/**
		 * Creates a new simple move from one field to another.
		 *
		 * @param int from
		 * @param int to	Target field
		 * @param Field formerField
		 */
		SimpleMove(int from, int to, Field formerField);

		/**
		 * Destructor for the SimpleMove objects
		 */
		virtual ~SimpleMove();

		/**
		 * Moves a piece from its current position to the target position.
		 *
		 * @param Board &board
		 * @return void
		 */
		void execute (Board &board);

		/**
		 * Restores the former position of the piece.
		 *
		 * @param Board &board
		 * @return void
		 */
		void revert (Board &board);
};

#endif /* SIMPLEMOVE_H_ */
