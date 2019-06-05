#define ON  1
#define OFF 0

// Integer types
typedef unsigned long long U64;
typedef   signed long long S64;
typedef unsigned       int U32;
typedef   signed       int S32;
typedef unsigned     short U16;
typedef   signed     short S16;
typedef unsigned       char U8;
typedef   signed       char S8;

// Encodes all the information of a chess move
typedef struct StructMove{

	U8 bit_from;
	// Initial bit

	U8 bit_to;
	// Final bit

	U8 move_type;
	// Integer denotes kind of move (see enum MoveEncoding)

	U8 moving_piece;
	// Index for moving piece

	U8 captured_piece;
	// Index for captured piece

} Move;

// Global memory for static chess related values
typedef struct StructChessGlobal{

	U64 capture_BB[2][6][64];
	// Two for each turn (enum TurnIndex)
	// Six for each piece (enum BBIndex)
	// 64 for each bit_from

	U64 quiet_BB[2][6][64];
	// Two for each turn (enum TurnIndex)
	// Six for each piece (enum BBIndex)
	// 64 for each bit_from

} Global;

// Local memory for board state of given position
typedef struct StructBoardState{

	U64 pieces_BB[8];
	// Six for pieces (enum BBIndex)
	// Two for colors (enum BBIndex)

	U8 castling_flags;
	// First four bits (enum CastlingKeys)
	// White short, long
	// Black short, long

	U8 EP_flags;
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

/* Move codes retrieved from: https://www.chessprogramming.org/Encoding_Moves
 * 
 * Codes set such that
 * Promotion 	-> Bit 3
 * Capture 		-> Bit 2
 * n/a			-> Bit 1
 * n/a			-> Bit 0				
 * Move type can be kept in a 4 bit integer
 * 6 and 7 are placeholders
 */

/*	Setters:
 *		If you want to set by bit index instead of rank and file,
 *		use rank=0 and file=bit
 */ 

/*	Getters:
 *		Return codes:
 *		0: Bit off at given location
 *		1: Bit on at given location
 */

// Setters
static inline void U64SetBitOn(U64*, U8, U8);
static inline void U64SetBitOff(U64*, U8, U8);
static inline void U8SetBitOn(U8*, U8);
static inline void U8SetBitOff(U8*, U8);

// Getters
static inline U8 U64GetBit(U64, U8, U8);
static inline U8 U8GetBit(U8, U8);

// Determine if move is a promotion or capture
static inline U8 isPromo(Move);
static inline U8 isCapture(Move);

// Setters and getters for castling and EP flags
static inline void setCastleFlagOn(Board*, U8);
static inline void setCastleFlagOff(Board*, U8);
static inline void setEPFlagOn(Board*, U8);
static inline void setEPFlagOff(Board*, U8);
static inline U8 getCastlingFlag(Board*, U8);
static inline U8 getEPFlag(Board*, U8);

// Setting all flags at once
static inline void clearAllPiecesBB(Board*);
static inline void lowerAllCastlingFlags(Board*);
static inline void raiseAllCastlingFlags(Board*);
static inline void lowerAllEPFlags(Board*);

// Set piece on board by color and piece type
static inline void setPieceOn(Board*, U8, U8, U8, U8);
static inline void setPieceOff(Board*, U8, U8, U8, U8);

// Print BB
void BBPrint(U64);

// Load the starting position
void BoardStart(Board*);

// Print board
void BoardPrint(Board*);

// Setup board based off of FEN string
void loadFEN(Board*, char*);

// Load capture and quiet BBs
void GlobalLoadBBs(Global*);

// Filenames for captureBB and quietBB
char* filenames_capture[2][6];
char* filenames_quiet[2][6];

// Check if rank and file within board dims
U8 isRankFileInBounds(U8, U8);
/* Return codes:
 * 1: Piece in bounds at (rank, file)
 * 0: Piece not in bounds at (rank, file)
 */

// Return moveList without validating for check
Move* pseudoMoveGenerator(Global*, Board*, U8*, U8);

// Return moveList while validating for check
Move* legalMoveGenerator(Global*, Board*, U8*, U8);

// Basis vectors for scanning sliding moves
S8 delta_rank[8];
S8 delta_file[8];

// Configure all attributes of Move struct
static inline void configureMove(Move*, U8, U8, U8, U8, U8);

// Determine if move is legal
U8 validateMove(Global*, Board*, Move);
/* Return codes:
 * 0: Illegal move
 * 1: Legal move
 */

// Execute move and increment ply, update castle and EP flags
void makeMove(Global*, Board*, Move);

// Undo move, reset ply, castling, and EP flags
void undoMove(Global*, Board*, Move, U8, U8);

// Verify if current-turn player in check
U8 isInCheck(Global*, Board*, U8);
/* Return codes:
 * 0: Not in check
 * 1: Check (can capture checking piece, intercept, or move king)
 * 2: Double check (must move king to get out of check)
 *
 * Input king_bit = 64 if king position unknown
 */

// (PERF)ormance (T)est to test move generation, up to 3-fold and 50-move
void perftResults(Global*, Board*, U64**, U8, U8);

// Benchmark perft() to only count nodes
U64 perftBenchmark(Global*, Board*, U8);

// Initial call for perft() function
void initPerft(Global*, Board*, U8, U8);

// Extensive comparison to existing perft results
void longPerftDebug(Global*, Board*);

// Debugging tool to compare boards
void compareBoards(Board*, Board*, char*, char*);

// Debug current board for many functions
void debugBoard(Global*, Board*, Move, char*);

// Convert algebraic coordinates to bit
U8 bitFromAlgeb(char*);

// Test that bits correspond to proper move types
void bitTesting();

// Unit tests (not 100% coverage)
void unitTests(Global*, Board*);

// Convert move to UCI string
char* moveToUCI(Move);

// Print out complete moveList in full detail
void movePrinter(Global*, Board*);

/***************************************
* TO DO LIST:
*
* - Have a function for tracking castling_flags and EP_flags
*
* - Consider U64GetBit by bit only, not rank and file
*
* - Consider U64SetOn and U64SetOff instead of the toggle
*
* - Resolve static inline issues for U64SetBit() and isRankFileInBounds()
*
* - Look into hex 0x integer notation
*
* - Instead of using bits as integers, only use U64s to make comparisons faster
*
* - Preload sliding bitboards and king space bitboards from all starting bits
*
* - Isolate moveGen() for bishop/rook/queen?????
*
* - Load turn specific things (turn, color, opp_color) into global struct
*		- Increment the active set of values in makeMove() / undoMove()
*
* - Try and remove all raw use of UxxGetBit or UxxSetBit, cover them up in helper functions
*
* - 3 fold repetition
* - 50 move draw
*		- Fix in loadFEN()
* - insufficientMaterial()
* - Determine checkmate and stalemate status
* - Algebraic reader
*
* - Make sure I free() where I malloc() or calloc()
*		- Particularly for moveGenerators()
*
* - Change variables to use_this_type
* - Change functions to functionLikeThis()
*
* - Reorder functions in chess_framework files
*
* - Tidy up Makefile and files
*
* - Make sure all declared variables in pseudoMoveGenerator() are used
* 
* - Compress moveToUCI by using the ascii values of a-h and 1-8
*
* - Compress memory in Board and Move structs
* - Implement U8 and S8 data types
*
* - Potentially keep track of king bits
* - Consider setting Global* and Board* as global variables
*
* - Make sure all used data types are appropriate
*
* - Inside makeMove(), consider a more efficient piece movement by
* 	having a bitmask that has bit_from and bit_to as the only active
*	bits, and XOR that mask with the corresponding colorBB and pieceBB
*	for the moving piece to makeMove()
*
* - Add .bmp output
*
****************************************/



/******************************************\
*
*	ERRORS IN MOVE GENERATION FUNCTIONS:
*
*	(List of all chess logic errors that
*	have been since fixed. This excludes
*	all comp-sci specific bugs)
*
*	- Long castle would use a phantom rook from
*		B-file instead of A-file
*
*	- makeMove and undoMove would update moving
*		and captured pieces in wrong order,
*		generating phantom pieces when undoing
*		en passant captures
*
*	- isInCheck would not look for knight checks
*		in certain cases, would allow for king
*		to stay in check
*
*
*
\******************************************/
