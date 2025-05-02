#pragma once

#include "types.h"
#include <ostream>
#include <string>
#include "tables.h"
#include <utility>

//psuedorandom number generator
//source stockfish
class PRNG {
    uint64_t s;

    uint64_t rand64() {
        s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
        return s * 2685821657736338717LL;
    }

    public:
    PRNG(uint64_t seed) : s(seed) {}

    //gen rand number
    template<typename T> T rand() { return T(rand64()); }

    //gen rand number w only a few set bits
    template<typename T> 
	T sparse_rand() {
		return T(rand64() & rand64() & rand64());
	}
};

//Zobrist is a type of hashing for transition tables
//Used to avoid analyzing the same position more than once
namespace zobrist {
	extern uint64_t zobrist_table[NPIECES][NSQUARES];
	extern void initialise_zobrist_keys();
}

//Stores position information which cannot be recovered on undo-ing a move
struct UndoInfo {
	//The bitboard of squares on which pieces have either moved from, or have been moved to. Used for castling
	//legality checks
	Bitboard entry;
	
	//The piece that was captured on the last move
	Piece captured;
	
	//The en passant square. This is the square which pawns can move to in order to en passant capture an enemy pawn that has 
	//double pushed on the previous move
	Square epsq;

	UndoInfo() : entry(0), captured(NO_PIECE), epsq(NO_SQUARE) {}
	
	//This preserves the entry bitboard across moves
	UndoInfo(const UndoInfo& prev) : 
		entry(prev.entry), captured(NO_PIECE), epsq(NO_SQUARE) {}

	UndoInfo& operator=(const UndoInfo& other) = default;
};

class Position {
    private:
        //A bitboard of the locations of each piece
        Bitboard piece_bb[NPIECES];
        
        //A mailbox representation of the board. Stores the piece occupying each square on the board
        Piece board[NSQUARES];
        
        //The side whose turn it is to play next
        Color side_to_play;
        
        //The current game ply (depth), incremented after each move 
        int game_ply;
        
        //The zobrist hash of the position, which can be incrementally updated and rolled back after each
        //make/unmake
        uint64_t hash;
    public:
        //The history of non-recoverable information
        UndoInfo history[256];
        
        //The bitboard of enemy pieces that are currently attacking the king, updated whenever generate_moves()
        //is called
        Bitboard checkers;
        
        //The bitboard of pieces that are currently pinned to the king by enemy sliders, updated whenever 
        //generate_moves() is called
        Bitboard pinned;
        
        
        Position() : piece_bb{ 0 }, side_to_play(WHITE), game_ply(0), board{}, 
            hash(0), pinned(0), checkers(0) {
            
            //Sets all squares on the board as empty
            for (int i = 0; i < 64; i++) board[i] = NO_PIECE;
            history[0] = UndoInfo();
        }
        
        //Places a piece on a particular square and updates the hash. Placing a piece on a square that is 
        //already occupied is an error
        inline void put_piece(Piece pc, Square s) {
            board[s] = pc;
            piece_bb[pc] |= SQUARE_BB[s];
            hash ^= zobrist::zobrist_table[pc][s];
        }
    
        //Removes a piece from a particular square and updates the hash. 
        inline void remove_piece(Square s) {
            hash ^= zobrist::zobrist_table[board[s]][s];
            piece_bb[board[s]] &= ~SQUARE_BB[s];
            board[s] = NO_PIECE;
        }
    
        void move_piece(Square from, Square to);
        void move_piece_quiet(Square from, Square to);
    
    
        friend std::ostream& operator<<(std::ostream& os, const Position& p);
        static void set(const std::string& fen, Position& p);
        std::string fen() const;
    
        Position& operator=(const Position&) = delete;
        inline bool operator==(const Position& other) const { return hash == other.hash; }
    
        inline Bitboard bitboard_of(Piece pc) const { return piece_bb[pc]; }
        inline Bitboard bitboard_of(Color c, PieceType pt) const { return piece_bb[make_piece(c, pt)]; }
        inline Piece at(Square sq) const { return board[sq]; }
        inline Color turn() const { return side_to_play; }
        inline int ply() const { return game_ply; }
        inline uint64_t get_hash() const { return hash; }
		inline uint64_t zobrist_key() const { return hash; }

        template<Color C> inline Bitboard diagonal_sliders() const;
        template<Color C> inline Bitboard orthogonal_sliders() const;
        template<Color C> inline Bitboard all_pieces() const;
        template<Color C> inline Bitboard attackers_from(Square s, Bitboard occ) const;
    
        template<Color C> inline bool in_check() const {
            return attackers_from<~C>(bsf(bitboard_of(C, KING)), all_pieces<WHITE>() | all_pieces<BLACK>());
        }
    
        template<Color C> void play(Move m);
        template<Color C> void undo(Move m);
    
        template<Color Us>
        Move * generate_legals(Move* list);
};
    

//Returns the bitboard of all bishops and queens of a given color
template<Color C> 
inline Bitboard Position::diagonal_sliders() const {
	return C == WHITE ? piece_bb[WHITE_BISHOP] | piece_bb[WHITE_QUEEN] :
		piece_bb[BLACK_BISHOP] | piece_bb[BLACK_QUEEN];
}

//Returns the bitboard of all rooks and queens of a given color
template<Color C> 
inline Bitboard Position::orthogonal_sliders() const {
	return C == WHITE ? piece_bb[WHITE_ROOK] | piece_bb[WHITE_QUEEN] :
		piece_bb[BLACK_ROOK] | piece_bb[BLACK_QUEEN];
}

//Returns a bitboard containing all the pieces of a given color
template<Color C> 
inline Bitboard Position::all_pieces() const {
	return C == WHITE ? piece_bb[WHITE_PAWN] | piece_bb[WHITE_KNIGHT] | piece_bb[WHITE_BISHOP] |
		piece_bb[WHITE_ROOK] | piece_bb[WHITE_QUEEN] | piece_bb[WHITE_KING] :

		piece_bb[BLACK_PAWN] | piece_bb[BLACK_KNIGHT] | piece_bb[BLACK_BISHOP] |
		piece_bb[BLACK_ROOK] | piece_bb[BLACK_QUEEN] | piece_bb[BLACK_KING];
}

//Returns a bitboard containing all pieces of a given color attacking a particluar square
template<Color C> 
inline Bitboard Position::attackers_from(Square s, Bitboard occ) const {
	return C == WHITE ? (pawn_attacks<BLACK>(s) & piece_bb[WHITE_PAWN]) |
		(attacks<KNIGHT>(s, occ) & piece_bb[WHITE_KNIGHT]) |
		(attacks<BISHOP>(s, occ) & (piece_bb[WHITE_BISHOP] | piece_bb[WHITE_QUEEN])) |
		(attacks<ROOK>(s, occ) & (piece_bb[WHITE_ROOK] | piece_bb[WHITE_QUEEN])) :
		(pawn_attacks<WHITE>(s) & piece_bb[BLACK_PAWN]) |
		(attacks<KNIGHT>(s, occ) & piece_bb[BLACK_KNIGHT]) |
		(attacks<BISHOP>(s, occ) & (piece_bb[BLACK_BISHOP] | piece_bb[BLACK_QUEEN])) |
		(attacks<ROOK>(s, occ) & (piece_bb[BLACK_ROOK] | piece_bb[BLACK_QUEEN]));
}


/*template<Color C>
Bitboard Position::pinned(Square s, Bitboard us, Bitboard occ) const {
	Bitboard pinned = 0;

	Bitboard pinners = get_xray_rook_attacks(s, occ, us) & orthogonal_sliders<~C>();
	while (pinners) pinned |= SQUARES_BETWEEN_BB[s][pop_lsb(&pinners)] & us;

	pinners = get_xray_bishop_attacks(s, occ, us) & diagonal_sliders<~C>();
	while (pinners) pinned |= SQUARES_BETWEEN_BB[s][pop_lsb(&pinners)] & us;

	return pinned;
}

template<Color C>
Bitboard Position::blockers_to(Square s, Bitboard occ) const {
	Bitboard blockers = 0;
	Bitboard candidates = get_rook_attacks(s, occ) & occ;
	Bitboard attackers = get_rook_attacks(s, occ ^ candidates) & orthogonal_sliders<~C>();

	candidates = get_bishop_attacks(s, occ) & occ;
	attackers |= get_bishop_attacks(s, occ ^ candidates) & diagonal_sliders<~C>();

	while (attackers) blockers |= SQUARES_BETWEEN_BB[s][pop_lsb(&attackers)];
	return blockers;
}*/

//Plays a move in the position
template<Color C>
void Position::play(const Move m) {
	side_to_play = ~side_to_play;
	++game_ply;
	history[game_ply] = UndoInfo(history[game_ply - 1]);

	MoveFlags type = m.flags();
	history[game_ply].entry |= SQUARE_BB[m.to()] | SQUARE_BB[m.from()];

	switch (type) {
	case QUIET:
		//The to square is guaranteed to be empty here
		move_piece_quiet(m.from(), m.to());
		break;
	case DOUBLE_PUSH:
		//The to square is guaranteed to be empty here
		move_piece_quiet(m.from(), m.to());
			
		//This is the square behind the pawn that was double-pushed
		history[game_ply].epsq = m.from() + relative_dir<C>(NORTH);
		break;
	case OO:
		if (C == WHITE) {
			move_piece_quiet(e1, g1);
			move_piece_quiet(h1, f1);
		} else {
			move_piece_quiet(e8, g8);
			move_piece_quiet(h8, f8);
		}			
		break;
	case OOO:
		if (C == WHITE) {
			move_piece_quiet(e1, c1); 
			move_piece_quiet(a1, d1);
		} else {
			move_piece_quiet(e8, c8);
			move_piece_quiet(a8, d8);
		}
		break;
	case EN_PASSANT:
		move_piece_quiet(m.from(), m.to());
		remove_piece(m.to() + relative_dir<C>(SOUTH));
		break;
	case PR_KNIGHT:
		remove_piece(m.from());
		put_piece(make_piece(C, KNIGHT), m.to());
		break;
	case PR_BISHOP:
		remove_piece(m.from());
		put_piece(make_piece(C, BISHOP), m.to());
		break;
	case PR_ROOK:
		remove_piece(m.from());
		put_piece(make_piece(C, ROOK), m.to());
		break;
	case PR_QUEEN:
		remove_piece(m.from());
		put_piece(make_piece(C, QUEEN), m.to());
		break;
	case PC_KNIGHT:
		remove_piece(m.from());
		history[game_ply].captured = board[m.to()];
		remove_piece(m.to());
		
		put_piece(make_piece(C, KNIGHT), m.to());
		break;
	case PC_BISHOP:
		remove_piece(m.from());
		history[game_ply].captured = board[m.to()];
		remove_piece(m.to());

		put_piece(make_piece(C, BISHOP), m.to());
		break;
	case PC_ROOK:
		remove_piece(m.from());
		history[game_ply].captured = board[m.to()];
		remove_piece(m.to());

		put_piece(make_piece(C, ROOK), m.to());
		break;
	case PC_QUEEN:
		remove_piece(m.from());
		history[game_ply].captured = board[m.to()];
		remove_piece(m.to());

		put_piece(make_piece(C, QUEEN), m.to());
		break;
	case CAPTURE:
		history[game_ply].captured = board[m.to()];
		move_piece(m.from(), m.to());
		
		break;
	}
}

//Undos a move in the current position, rolling it back to the previous position
template<Color C>
void Position::undo(const Move m) {
	MoveFlags type = m.flags();
	switch (type) {
	case QUIET:
		move_piece_quiet(m.to(), m.from());
		break;
	case DOUBLE_PUSH:
		move_piece_quiet(m.to(), m.from());
		break;
	case OO:
		if (C == WHITE) {
			move_piece_quiet(g1, e1);
			move_piece_quiet(f1, h1);
		} else {
			move_piece_quiet(g8, e8);
			move_piece_quiet(f8, h8);
		}
		break;
	case OOO:
		if (C == WHITE) {
			move_piece_quiet(c1, e1);
			move_piece_quiet(d1, a1);
		} else {
			move_piece_quiet(c8, e8);
			move_piece_quiet(d8, a8);
		}
		break;
	case EN_PASSANT:
		move_piece_quiet(m.to(), m.from());
		put_piece(make_piece(~C, PAWN), m.to() + relative_dir<C>(SOUTH));
		break;
	case PR_KNIGHT:
	case PR_BISHOP:
	case PR_ROOK:
	case PR_QUEEN:
		remove_piece(m.to());
		put_piece(make_piece(C, PAWN), m.from());
		break;
	case PC_KNIGHT:
	case PC_BISHOP:
	case PC_ROOK:
	case PC_QUEEN:
		remove_piece(m.to());
		put_piece(make_piece(C, PAWN), m.from());
		put_piece(history[game_ply].captured, m.to());
		break;
	case CAPTURE:
		move_piece_quiet(m.to(), m.from());
		put_piece(history[game_ply].captured, m.to());
		break;
	}

	side_to_play = ~side_to_play;
	--game_ply;
}

//Generates all legal moves in a position for the given side. Advances the move pointer and returns it.
template<Color Us>
Move* Position::generate_legals(Move* list) {
    constexpr Color Them = ~Us;

    const Bitboard us_bb = all_pieces<Us>();
    const Bitboard them_bb = all_pieces<Them>();
    const Bitboard all = us_bb | them_bb;

    const Square our_king = bsf(bitboard_of(Us, KING));
    const Square their_king = bsf(bitboard_of(Them, KING));

    //const Bitboard our_diag_sliders = diagonal_sliders<Us>();
    const Bitboard their_diag_sliders = diagonal_sliders<Them>();
    //const Bitboard our_orth_sliders = orthogonal_sliders<Us>();
    const Bitboard their_orth_sliders = orthogonal_sliders<Them>();

    // General purpose bitboards
    Bitboard b1;

    // Build danger squares
    Bitboard danger = 0;
    danger |= pawn_attacks<Them>(bitboard_of(Them, PAWN)) | attacks<KING>(their_king, all);

    b1 = bitboard_of(Them, KNIGHT);
    while (b1) danger |= attacks<KNIGHT>(pop_lsb(&b1), all);

    b1 = their_diag_sliders;
    while (b1) danger |= attacks<BISHOP>(pop_lsb(&b1), all ^ SQUARE_BB[our_king]);

    b1 = their_orth_sliders;
    while (b1) danger |= attacks<ROOK>(pop_lsb(&b1), all ^ SQUARE_BB[our_king]);

    // King moves first
    b1 = attacks<KING>(our_king, all) & ~(us_bb | danger);
    list = make<QUIET>(our_king, b1 & ~them_bb, list);
    list = make<CAPTURE>(our_king, b1 & them_bb, list);

    // Compute pinned pieces and checkers
    checkers = (attacks<KNIGHT>(our_king, all) & bitboard_of(Them, KNIGHT))
             | (pawn_attacks<Us>(our_king) & bitboard_of(Them, PAWN));

    Bitboard candidates = (attacks<ROOK>(our_king, them_bb) & their_orth_sliders)
                         | (attacks<BISHOP>(our_king, them_bb) & their_diag_sliders);

    pinned = 0;
    while (candidates) {
        Square s = pop_lsb(&candidates);
        b1 = SQUARES_BETWEEN_BB[our_king][s] & us_bb;
        if (b1 == 0) {
            checkers ^= SQUARE_BB[s];
        } else if ((b1 & (b1 - 1)) == 0) {
            pinned ^= b1;
        }
    }

    const Bitboard non_pinned = us_bb & ~pinned;
    const Bitboard pinned_pieces = us_bb & pinned;
    const Bitboard all_pieces_bb = all;

    // Switch based on number of checkers
    switch (sparse_pop_count(checkers)) {
    case 2:
        // Double check: Only king moves allowed
        return list;

    case 1: {
        // Single check: Must capture checker or block
        Square checker_square = bsf(checkers);
        PieceType checker_type = type_of(at(checker_square));

        Bitboard quiet_mask = 0;
        if (checker_type == ROOK || checker_type == BISHOP || checker_type == QUEEN) {
            quiet_mask = SQUARES_BETWEEN_BB[our_king][checker_square];
        }

        Bitboard capture_mask = SQUARE_BB[checker_square];

        // Unpinned pieces
        {
            // Knights
            Bitboard knights = bitboard_of(Us, KNIGHT) & non_pinned;
            while (knights) {
                Square from = pop_lsb(&knights);
                Bitboard moves = attacks<KNIGHT>(from, all_pieces_bb) & capture_mask;
                list = make<CAPTURE>(from, moves & them_bb, list);
            }

            // Bishops
            Bitboard bishops = bitboard_of(Us, BISHOP) & non_pinned;
            while (bishops) {
                Square from = pop_lsb(&bishops);
                Bitboard moves = attacks<BISHOP>(from, all_pieces_bb) & (capture_mask | quiet_mask);
                list = make<CAPTURE>(from, moves & them_bb, list);
                list = make<QUIET>(from, moves & quiet_mask & ~them_bb, list);
            }

            // Rooks
            Bitboard rooks = bitboard_of(Us, ROOK) & non_pinned;
            while (rooks) {
                Square from = pop_lsb(&rooks);
                Bitboard moves = attacks<ROOK>(from, all_pieces_bb) & (capture_mask | quiet_mask);
                list = make<CAPTURE>(from, moves & them_bb, list);
                list = make<QUIET>(from, moves & quiet_mask & ~them_bb, list);
            }

            // Queens
            Bitboard queens = bitboard_of(Us, QUEEN) & non_pinned;
            while (queens) {
                Square from = pop_lsb(&queens);
                Bitboard moves = attacks<QUEEN>(from, all_pieces_bb) & (capture_mask | quiet_mask);
                list = make<CAPTURE>(from, moves & them_bb, list);
                list = make<QUIET>(from, moves & quiet_mask & ~them_bb, list);
            }
        }

        // Pinned pieces
        {
            Bitboard pinned_rooks = bitboard_of(Us, ROOK) & pinned_pieces;
            while (pinned_rooks) {
                Square from = pop_lsb(&pinned_rooks);
                if (LINE[our_king][from] & SQUARE_BB[checker_square]) {
                    Bitboard moves = attacks<ROOK>(from, all_pieces_bb) & capture_mask;
                    list = make<CAPTURE>(from, moves & them_bb, list);
                }
            }

            Bitboard pinned_bishops = bitboard_of(Us, BISHOP) & pinned_pieces;
            while (pinned_bishops) {
                Square from = pop_lsb(&pinned_bishops);
                if (LINE[our_king][from] & SQUARE_BB[checker_square]) {
                    Bitboard moves = attacks<BISHOP>(from, all_pieces_bb) & capture_mask;
                    list = make<CAPTURE>(from, moves & them_bb, list);
                }
            }

            Bitboard pinned_queens = bitboard_of(Us, QUEEN) & pinned_pieces;
            while (pinned_queens) {
                Square from = pop_lsb(&pinned_queens);
                if (LINE[our_king][from] & SQUARE_BB[checker_square]) {
                    Bitboard moves = attacks<QUEEN>(from, all_pieces_bb) & capture_mask;
                    list = make<CAPTURE>(from, moves & them_bb, list);
                }
            }
        }

        return list;
    }

    default:
        // No check: generate all normal moves
        break;
    }

    // ---------------------------
    // Normal move generation
    // ---------------------------

    {
        // Knights
        Bitboard knights = bitboard_of(Us, KNIGHT) & non_pinned;
        while (knights) {
            Square from = pop_lsb(&knights);
            Bitboard moves = attacks<KNIGHT>(from, all_pieces_bb) & ~us_bb;
            list = make<QUIET>(from, moves & ~them_bb, list);
            list = make<CAPTURE>(from, moves & them_bb, list);
        }

        // Bishops
        Bitboard bishops = bitboard_of(Us, BISHOP);
        while (bishops) {
            Square from = pop_lsb(&bishops);
            Bitboard mask = (pinned & SQUARE_BB[from]) ? LINE[our_king][from] : ~Bitboard(0);
            Bitboard moves = attacks<BISHOP>(from, all_pieces_bb) & mask & ~us_bb;
            list = make<QUIET>(from, moves & ~them_bb, list);
            list = make<CAPTURE>(from, moves & them_bb, list);
        }

        // Rooks
        Bitboard rooks = bitboard_of(Us, ROOK);
        while (rooks) {
            Square from = pop_lsb(&rooks);
            Bitboard mask = (pinned & SQUARE_BB[from]) ? LINE[our_king][from] : ~Bitboard(0);
            Bitboard moves = attacks<ROOK>(from, all_pieces_bb) & mask & ~us_bb;
            list = make<QUIET>(from, moves & ~them_bb, list);
            list = make<CAPTURE>(from, moves & them_bb, list);
        }

        // Queens
        Bitboard queens = bitboard_of(Us, QUEEN);
        while (queens) {
            Square from = pop_lsb(&queens);
            Bitboard mask = (pinned & SQUARE_BB[from]) ? LINE[our_king][from] : ~Bitboard(0);
            Bitboard moves = attacks<QUEEN>(from, all_pieces_bb) & mask & ~us_bb;
            list = make<QUIET>(from, moves & ~them_bb, list);
            list = make<CAPTURE>(from, moves & them_bb, list);
        }
    }

    return list;
}

//A convenience class for interfacing with legal moves, rather than using the low-level
//generate_legals() function directly. It can be iterated over.
template<Color Us>
class MoveList {
public:
	explicit MoveList(Position& p) : last(p.generate_legals<Us>(list)) {}

	const Move* begin() const { return list; }
	const Move* end() const { return last; }
	size_t size() const { return last - list; }
private:
	Move list[218];
	Move *last;
};