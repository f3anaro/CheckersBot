/*
 * AbstractMove.h
 *
 *  Created on: 21.03.2013
 *      Author: Lucas Kahlert <lucas.kahlert@dragonwebdesigns.de>
 */

#ifndef ABSTRACTMOVE_H_
#define ABSTRACTMOVE_H_

#include "Board.h"

class AbstractMove {
	protected:
		int from;
		Field formerField;

	public:
		/**
		 * @param int from
		 * @param Field formerField
		 */
		AbstractMove (int from, Field formerField) {
			this->from = from;
			this->formerField = formerField;
		}

		/**
		 * @param Board &board
		 * @return void
		 */
		virtual void execute (Board &board);

		/**
		 * Reverts the move on the committed board object.
		 *
		 * @param Board &board
		 * @return void
		 */
		virtual void revert (Board &board);

		/**
		 * Returns the string representation of the move. This
		 * string could be passed to the master control program.
		 *
		 * @return std::string
		 */
		virtual std::string toString (void);

		/**
		 * Destructor
		 *
		 * @return void
		 */
		virtual ~AbstractMove ();

};

#endif /* ABSTRACTMOVE_H_ */
