#ifndef __game_h__
#define __game_h__
#include "piece.h"

class game{
	public:
		game();
		~game();
		void start();
		void turn(int);

		void pawn_promotion(int,int,int);
		void enpassant(piece*,int,int,int);
		void castlespoiled(piece*,int);
		void parity_verify(piece**,piece**,int,int);
		void threefold();
	protected:
		static const int n_=8;
		piece ***chessboard_;
		board *BD, *BD_old1, *BD_old2;	// board plus two old boards to check for repetition
		bool passant_, castle_spoiler[4];
		int passant[2], N_50draw, N_3rep;
	private:
		bool checkmate_, stalmate_, draw_;
};

#endif
