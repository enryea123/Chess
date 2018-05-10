#ifndef __board_h__
#define __board_h__

class board{
	public:
		board();
		~board();

		void printboard();
		void printboard_small();
		void printnumbers();

		void cellname(int,int);
		void fillcell(int,int,int);
		int verifycell(int,int);
		int verifypiece(int,int);
		int getboard(int,int);

		bool compare(board*);
		board* operator=(board*);	// overload assignment operator

	protected:
		static const int n_=8; 
		int **matrixboard_;
};

//fare metodo in scacchiera per vedere se è occupata da avversario

#endif
