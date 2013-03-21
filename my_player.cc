#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>

const int FD_IN  = 3;
const int FD_OUT = 4;

const int BUFFERSIZE_IN  = 35;
const int BUFFERSIZE_OUT = 39;
const int BUFFERSIZE = BUFFERSIZE_IN < BUFFERSIZE_OUT ? BUFFERSIZE_OUT : BUFFERSIZE_IN;

static void error (const char * fmt, ...)
{
    va_list vl;
    va_start (vl, fmt);
    vfprintf (stderr, fmt, vl);
    va_end (vl);
    exit (-1);
}

enum Field {
    NONE  = 0,
    BLACK = 1,
    WHITE = 2,
    KING  = 4,
    BLACKKING = BLACK | KING,
    WHITEKING = WHITE | KING,
};

class Board
{
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
         */
        Board (std::string const string) {
        	unserialize(string);
        }

        // Copy method
        Board (Board const &b) {
        	memcpy (&field, &b.field, sizeof(field));
        }

        /**
         * Unserialize a string to the board state.
         *
         * @param std::string const string	Input string representing the board state
         * @return void
         */
        void unserialize (std::string const string);

        /**
         * Serialize board state to string
         *
         * @return std::string
         */
        std::string serialize ();

        /**
         * Prints the current board state to standard output
         *
         * @return void
         */
        void draw();
};

void Board::unserialize (std::string const string) {
	// validate string length
    assert (string.length() == 32);

    int i = 0;

    for (std::string::iterator iterator = string.begin(); iterator != string.end(); ++iterator) {
    	Field value = NONE;

    	switch (*iterator) {
			case 'b' : value = BLACK; break;
			case 'w' : value = WHITE; break;
			case 'B' : value = BLACKKING; break;
			case 'W' : value = WHITEKING; break;
			case '-' : value = NONE; break;
			default : error ("unknown char '%c' in input string\n", *iterator); break;
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
    return std::string;
}

void Board::draw()
{
    for (int i=0; i<64; i++) {
        if ((i & 7) == 0)
            printf ("+----+----+----+----+----+----+----+----+\n");
        char c = ' ';
        if (field[i] & BLACK) c = 'b';
        if (field[i] & WHITE) c = 'w';
        if (field[i] & KING)  c -= 32;

        if (((i & 8) && (i & 1)) || (!(i & 8) && !(i & 1))) {
            printf ("|    ");
        } else {
            printf("|%c%2d%c",c,(i/2)+1,c);
        }

        if ((i & 7) == 7)
            printf ("|\n");
    }
    printf ("+----+----+----+----+----+----+----+----+\n");
}

/**
 * Reads the board state from the input stream into an string
 * and returns it.
 *
 * @return std::string
 */
std::string input () {
	char* buffer[BUFFERSIZE];
    char* end = buffer + BUFFERSIZE_IN;

    while (buffer < end) {
        int bytes_read = read (FD_IN, buffer, end - buffer);
        if (bytes_read < 0) error ("error reading FD_IN\n");
        buffer += bytes_read;
    }
    end--;

    // validate last character
    if (*end != '\n')
    	error ("line does not end with newline\n");

    *end = 0;
    return std::string(buffer);
}

/**
 * Writes a given string to the output stream
 *
 * @param
 * @return void
 */
void output (std::string const *buffer) {
    int length = buffer->length();

    // write to output stream and validate count of written chars
    if (write (FD_OUT, buffer, length) != length)
    	error ("error writing FD_OUT\n");
}


int main() {
	Board board = new Board();

    while (1) {
    	std::string input = input();

    	board.unserialize(input);
    	board.draw();

    	printf ("[ %c ] board '%s'\n", input[0], input + 2);

		printf ("[ %c ] your move : ", input[0]);

		char buffer[BUFFERSIZE];

		// request user input, read move into buffer
		if (!fgets(buffer, BUFFERSIZE_OUT, stdin))
			error ("fgets() failed");

		// send move back to MCP
		output (buffer);
    }
}
