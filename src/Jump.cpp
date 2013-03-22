/*
 * Jump.cpp
 *
 *  Created on: 21.03.2013
 *      Author: Lucas Kahlert <lucas.kahlert@dragonwebdesigns.de>
 */

#include "Jump.h"

Jump::execute(Board &board) {
	int from = this->from;

	// remove all pieces on the target route
	for (std::vector<int>::iterator iterator = targets.begin(); iterator != targets.end(); ++iterator) {
		int target = *iterator;

		// captured position
		int capPos = (target + from) / 2;

		// store captured field
		captured.push_back(board.field[capPos]);

		// remove captured piece from board
		board.field[capPos] = NONE;

		// go to next field
		from = target;
	}

	int lastTarget = *(targets.back());

	// removes piece from start field
	board.field[this->from] = NONE;

	// check if last move converts the piece into a king
	if (lastTarget < 8 || lastTarget > 55) {
		board.field[lastTarget] = formerField | KING;
	} else {
		board.field[lastTarget] = formerField;
	}
}

Jump::revert(Board &board) {
	int from = this->from;

	std::vector<int>::iterator targetIterator;
	std::vector<Field>::iterator capturedFieldIterator;

	// restore all captured pieces on the target route
	for (targetIterator = targets.begin(); targetIterator != targets.end(); ++targetIterator, ++capturedFieldIterator) {
		int target = *targetIterator;

		// calculate captured position
		int capPos = (target + from) / 2;

		// restore captured field from stack
		board.field[capPos] = *capturedFieldIterator;

		// go to next field
		from = target;
	}

	// removes all pieces from captured stack
	captured.clear();

	// remove piece on final target field
	board.field[*(targets.back())] = NONE;

	// restore old start value
	board.field[this->from] = this->formerField;
}

Jump::~Jump() {
	// TODO Auto-generated destructor stub
	delete captured;
	delete targets;
}

