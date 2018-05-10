#ifndef __piece_h__
#define __piece_h__
#include "board.h"
using namespace std;

//--------------------------------------------------------------------------//
class piece{
	public:
		int getx();
		int gety();
		int getcolor();
		int getid();
		double getvalue();
		string getname();

		void setposition(int,int);
		void getinfo();
		virtual int** move()=0;
	protected:
		static const int n_=8;
		int x_, y_, color_, id_;
		double value_;
		string name_;
		board* BOO;
};
//--------------------------------------------------------------------------//

class empty: public piece{
	public:
		empty(int,int,board*);
		~empty();
		virtual int** move();
};

class knight: public piece{
	public:
		knight(int,int,int,board*);
		~knight();
		virtual int** move();
};

class bishop: public piece{
	public:
		bishop(int,int,int,board*);
		~bishop();
		virtual int** move();
};

class rook: public piece{
	public:
		rook(int,int,int,board*);
		~rook();
		virtual int** move();
};

class queen: public piece{
	public:
		queen(int,int,int,board*);
		~queen();
		virtual int** move();
};

class king: public piece{
	public:
		king(int,int,int,board*);
		~king();
		virtual int** move();
};

class pawn: public piece{
	public:
		pawn(int,int,int,board*);
		~pawn();
		virtual int** move();
};

//--------------------------------------------------------------------------//
#endif
