/*
 * SimpleMove.cpp
 *
 *  Created on: 21.03.2013
 *      Author: Lucas Kahlert <lucas.kahlert@draognwebdesigns.de>
 */

#include "SimpleMove.h"

SimpleMove::SimpleMove(int from, int to, Field formerField): AbstractMove(from, formerField) {
	this->to = to;
}

void SimpleMove::execute(Board &board) {
	// make piece to king on the last lines
	if (to < 8 || to > 55) {
		board.field[to] = board.field[from] | KING;
	} else {
		board.field[to] = board.field[from];
	}
	board.field[from] = NONE;
}

void SimpleMove::revert(Board &board) {
	board.field[from] = formerField;
	board.field[to] = NONE;
}

SimpleMove::~SimpleMove() {
	// TODO Auto-generated destructor stub
}

