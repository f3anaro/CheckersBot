/*
 * Board.cpp
 *
 *  Created on: 21.03.2013
 *      Author: Lucas
 */

#include <assert.h>
#include "Board.h"

void Board::unserialize (char const * const buffer) {
	// validate string length
	int length = strlen(buffer);

	assert (strlen(buffer) == 32);


	for (int i = 0; i < length; i++) {
		Field value = NONE;

		switch (buffer[i]) {
			case 'b' : value = BLACK; break;
			case 'w' : value = WHITE; break;
			case 'B' : value = BLACKKING; break;
			case 'W' : value = WHITEKING; break;
			case '-' : value = NONE; break;
			default : error ("unknown char '%c' in input string\n", buffer[i]); break;
		}

    	// calculate field offset
    	int j = (i / 4) & 1; // (i / 4) % 2

    	// set field value
    	field[2*i-j+1] = value;

    	// set field neighbor
    	field[2*i+j] = NONE;
    	i++;
    }
}

std::string Board::serialize () {
	std::string serialized;

    for (int i=0; i < 32; i++) {
    	// calculate field offset
        int j = (i / 4) & 1;

        switch (field[2*i-j+1]) {
            case BLACK:     serialized += 'b'; break;
            case WHITE:     serialized += 'w'; break;
            case BLACKKING: serialized += 'B'; break;
            case WHITEKING: serialized += 'W'; break;
            case NONE:      serialized += '-'; break;
            default:        assert (0); break;
        }
    }
    return serialized;
}

void Board::draw()
{
    for (int i=0; i<64; i++) {
        if ((i & 7) == 0)
            printf ("+----+----+----+----+----+----+----+----+\n");
        char c = ' ';

        if (field[i] & BLACK)
        	c = 'b';

        if (field[i] & WHITE)
        	c = 'w';

        // capitalize
        if (field[i] & KING)
        	c -= 32;

        if (((i & 8) && (i & 1)) || (!(i & 8) && !(i & 1))) {
            printf ("|    ");
        } else {
            printf("|%c%2d%c",c,(i/2)+1,c);
        }

        // end of line
        if ((i & 7) == 7)
            printf ("|\n");
    }
    printf ("+----+----+----+----+----+----+----+----+\n");
}
