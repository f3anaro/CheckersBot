/*
 * Jump.h
 *
 *  Created on: 21.03.2013
 *      Author: Lucas Kahlert <lucas.kahlert@dragonwebdesigns.de>
 */

#ifndef JUMP_H_
#define JUMP_H_

#include <vector>
#include "AbstractMove.h"

class Jump: public AbstractMove {
	protected:
		// stack of all fields that are passed by the jump
		std::vector<int> targets;

		// stack of captured pieces. This stack is used to restore the former
		// board situation
		std::vector<Field> captured;

	public:

		/**
		 * @param int from
		 * @param std::vector<int> targets
		 * @param Board board
		 */
		Jump(int from, std::vector<int> targets, Field formerField): AbstractMove(from, formerField) {
			this->targets = targets;

			// initializes a new list of captured pieces
			captured = new std::vector<Field>();
		}

		virtual ~Jump();
};

#endif /* JUMP_H_ */
