#define ON  1
#define OFF 0

#define YES 1
#define NO  0

/*	Bitboard chessboard layout (feat. original chess coords)
 *	
 *	56	57	58	59	60	61	62	63		8
 *	48	49	50	51	52	53	54	55		7
 *	40	41	42	43	44	45	46	47		6
 *	32	33	34	35	36	37	38	39		5
 *	24	25	26	27	28	29	30	31		4
 *	16	17	18	19	20	21	22	23		3
 *	8	9	10	11	12	13	14	15		2
 *	0	1	2	3	4	5	6	7		1
 *
 *	A	B 	C 	D 	E 	F 	G 	H
 *
 * Access bit k by: 1ULL << k
 */

/* When moving a piece, bit shift amount
 * simply depends on direction of shift
 *
 *			(+7)  (+8)	(+9)
 *				  \	| / 
 *			(-1) <- 0 -> (+1)
 *				  /	| \
 *			(-9)  (-8)	(-7)
 *
 * The compass rose of bitwise chess
 */

// 64 bits correspond to 64 squares on CBoard
typedef unsigned long long U64;

// Other integer types
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;

// Encodes all the information of a chess move
typedef struct StructMove{

	U16 bit_from;
	// Initial bit (original bit)

	U16 bit_to;
	// Final bit (destination bit)

	U16 move_type;
	// Integer denotes kind of move (enum MoveEncoding)

	U16 moving_piece;
	// Index for moving piece

	U16 captured_piece;
	// Index for captured piece

} Move;

// Global memory for static chess related values
typedef struct StructChessGlobal{

	U64 captureBB[2][6][64];
	// Two for each turn (enum TurnIndex)
	// Six for each piece (enum BBIndex)
	// 64 for each bit_from

	U64 quietBB[2][6][64];
	// Two for each turn (enum TurnIndex)
	// Six for each piece (enum BBIndex)
	// 64 for each bit_from

} Global;

// Local memory for board state of given position
typedef struct StructBoardState{

	U64 piecesBB[8];
	// Six for pieces (enum BBIndex)
	// Two for colors (enum BBIndex)

	U16 castlingRights;
	// First four bits (enum CastlingKeys)
	// White short, long
	// Black short, long

	U16 EPFiles;
	// First 8 bits for 8 files

	U16 ply;
	// Keeps track of number of halfmoves

} Board;

// Different types of BBs
enum BBIndex{
	Pawn,		// 0
	Rook,		// 1
	Knight,		// 2
	Bishop,		// 3
	Queen,		// 4
	King, 		// 5
	White, 		// 6
	Black,		// 7
	Null_piece 	// 8
};

// Turn indicator
enum TurnIndex{
	BlackTurn,	// 0
	WhiteTurn	// 1
};

// Note BlackTurn is 0, as turn is computed as (ply % 2)

// Castling keys
enum CastlingKeys{
	ShortW,		// 0
	LongW,		// 1
	ShortB,		// 2
	LongB 		// 3
};

// Move encoding
enum MoveEncoding{
	Quiet,			// 0
	DoubleStep,		// 1
	ShortCastle,	// 2
	LongCastle,		// 3
	Capture,		// 4
	EPCapture,		// 5
	n_a_6,
	n_a_7,
	RPromo,			// 8
	NPromo,			// 9
	BPromo,			// 10
	QPromo,			// 11
	RPromoCapture,	// 12
	NPromoCapture,	// 13
	BPromoCapture,	// 14
	QPromoCapture	// 15
};

// Move codes retrieved from https://www.chessprogramming.org/Encoding_Moves
/* Codes set such that
 * Promotion 	-> Bit 3
 * Capture 		-> Bit 2
 * n/a			-> Bit 1
 * n/a			-> Bit 0				
 * Move type can be kept in a 4 bit integer
 * 6 and 7 are placeholders
 */

// Set bit on U64
void U64SetBit(U64*, U16, U16, U16);
/* If you want to set by bit index
 * instead of row and col, set 
 * row = 0 and col = bit
 */

// Get bit on U64
U16 U64GetBit(U64, U16, U16);
/* Return codes:
 * 1: Piece in BB at (rank, file)
 * 0: No piece in BB at (rank, file)
 */

// Setter and getter for U16
void U16SetBit(U16*, U16, U16, U16);
U16 U16GetBit(U16, U16, U16);

// Set piece on board by color and piece type
void setPiece(Board*, U16, U16, U16, U16, U16);

// Print BB
void BBPrint(U64);

// Place pieces in initial positions, reset castling and EP flags
void BoardReset(Board*);

// Print board
void BoardPrint(Board*);

// Setup board based off of FEN string
void loadFEN(Board*, U16, char**);

// Load capture and quiet BBs
void GlobalLoadBBs(Global*);

// Filenames for captureBB and quietBB
char* filenames_capture[2][6];
char* filenames_quiet[2][6];

// Check if rank and file within board dims
U16 isRankFileInBounds(U16, U16);
/* Return codes:
 * 1: Piece in bounds at (rank, file)
 * 0: Piece not in bounds at (rank, file)
 */

// Return moveList without validating for check
Move* pseudoMoveGenerator(Global*, Board*, U16*, U16);

// Return moveList while validating for check
Move* legalMoveGenerator(Global*, Board*, U16*, U16);

// Basis vectors for scanning sliding moves
S16 delta_rank[8];
S16 delta_file[8];

// Configure all attributes of Move struct
void configureMove(Move*, U16, U16, U16, U16, U16);

// Determine if move is legal
U16 validateMove(Global*, Board*, Move);
/* Return codes:
 * 0: Illegal move
 * 1: Legal move
 */

// Execute move and increment ply, update castle and EP flags
U16 makeMove(Global*, Board*, Move, U16);
/* Return codes:
 * 1: Move successful
 * 0: Move illegal (only when do_validate == 1)
 */

// Undo move, reset ply, castling, and EP flags
void undoMove(Global*, Board*, Move, U16, U16);

// Verify if current-turn player in check
U16 isInCheck(Global*, Board*, U16, U16);
/* Return codes:
 * 0: Not in check
 * 1: Check (can capture checking piece, intercept, or move king)
 * 2: Double check (must move king to get out of check)
 *
 * Input king_bit = 64 if king position unknown
 */

// (PERF)ormance (T)est to test move generation, up to 3-fold and 50-move
U64 perft(Global*, Board*, U64**, U16, U16);

// Initial call for perft() function
void initPerft(Global*, Board*, U16);

// Debugging tool to compare boards
void compareBoards(Board*, Board*, char*, char*);

// Debug current board for many functions
void debugBoard(Global*, Board*, Move, char*);

// Convert algebraic coordinates to bit
U16 bitFromAlgeb(char*);

// Convert move to UCI string
char* moveToUCI(Move);

// Print out complete moveList in full detail
void movePrinter(Global*, Board*);

/***************************************
* TO DO LIST:
*
* - Make perft() work
*
* - 3 fold repetition
* - 50 move draw
*		- Fix in loadFEN()
* - insufficientMaterial()
* - Determine checkmate and stalemate status
* - FEN reader
* - Algebraic reader
*
* - Make sure I free() where I malloc() or calloc()
*		- Particularly for moveGenerators()
* - Change variables to use_this_type
* - Change functions to functionLikeThis()
* - Reorder functions in chess_framework files
*
* - Tidy up Makefile
* - Add standardized perft() benchmark
*
* - Make sure all declared variables in pseudoMoveGenerator() are used
* 
* - Manage #define collection
* - Compress moveToUCI by using the ascii values of a-h and 1-8
*
* - Consider compressing memory in Board and Move structs
*
****************************************/

// Inside makeMove():

/*
CONSIDER A MORE EFFICIENT PIECE MOVEMENT, i.e. HAVE
A BITMASK THAT HAS bit_from AND bit_to AS THE ONLY
1 BITS, AND XOR THAT MASK WITH THE CORRESPONDING
colorBB and pieceBB FOR THE MOVING PIECE
*/

/*
CONSIDER EMBEDDING turn, color, opp_color INTO BOARD
STATE. THEN, ONE WOULD UPDATE THEM IN makeMove
*/
