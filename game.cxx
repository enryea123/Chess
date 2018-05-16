#include <iostream>
#include <limits>
#include <stdlib.h>
#include "game.h"
#include "piece.h"
using namespace std;

int bigger(int a,int b){
if(abs(a)>=abs(b))
	return abs(a);
else
	return abs(b);
}

//--------------------------------------------------------------------------//
game::game(){

	checkmate_=false;
	stalmate_=false;
	draw_=false;

	passant_=false;
	N_50draw=0;	// counts the 50 moves if the king is the only piece left

	for(int i=0;i<4;i++)
		castle_spoiler[i]=false;

	BD = new board();

	BD_old1 = new board();	// boards of previous moves to keep track of the repetition
	BD_old2 = new board();
	N_3rep=0;	// counts the threefold repetition

	chessboard_ = new piece**[n_];
	for(int i=0;i<n_;i++)
		chessboard_[i] = new piece*[n_];
	for(int i=0;i<n_;i++)
		for(int j=0;j<n_;j++)
			chessboard_[i][j]=new empty(i,j,BD);

	//----------------------------------------------------------------------------------//
	//------------------------------- Loading the pieces -------------------------------//
	// white pieces
	chessboard_[0][0]=new rook(0,0,1,BD);
	chessboard_[1][0]=new knight(1,0,1,BD);
	chessboard_[2][0]=new bishop(2,0,1,BD);
	chessboard_[3][0]=new queen(3,0,1,BD);
	chessboard_[4][0]=new king(4,0,1,BD);
	chessboard_[5][0]=new bishop(5,0,1,BD);
	chessboard_[6][0]=new knight(6,0,1,BD);
	chessboard_[7][0]=new rook(7,0,1,BD);

	chessboard_[0][1]=new pawn(0,1,1,BD);
	chessboard_[1][1]=new pawn(1,1,1,BD);
	chessboard_[2][1]=new pawn(2,1,1,BD);
	chessboard_[3][1]=new pawn(3,1,1,BD);
	chessboard_[4][1]=new pawn(4,1,1,BD);
	chessboard_[5][1]=new pawn(5,1,1,BD);
	chessboard_[6][1]=new pawn(6,1,1,BD);
	chessboard_[7][1]=new pawn(7,1,1,BD);

	// black pieces
	chessboard_[0][7]=new rook(0,7,-1,BD);
	chessboard_[1][7]=new knight(1,7,-1,BD);
	chessboard_[2][7]=new bishop(2,7,-1,BD);
	chessboard_[3][7]=new queen(3,7,-1,BD);
	chessboard_[4][7]=new king(4,7,-1,BD);
	chessboard_[5][7]=new bishop(5,7,-1,BD);
	chessboard_[6][7]=new knight(6,7,-1,BD);
	chessboard_[7][7]=new rook(7,7,-1,BD);

	chessboard_[0][6]=new pawn(0,6,-1,BD);
	chessboard_[1][6]=new pawn(1,6,-1,BD);
	chessboard_[2][6]=new pawn(2,6,-1,BD);
	chessboard_[3][6]=new pawn(3,6,-1,BD);
	chessboard_[4][6]=new pawn(4,6,-1,BD);
	chessboard_[5][6]=new pawn(5,6,-1,BD);
	chessboard_[6][6]=new pawn(6,6,-1,BD);
	chessboard_[7][6]=new pawn(7,6,-1,BD);
	//----------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------//

	BD->printboard();
}
game::~game(){
	delete[] chessboard_, BD, passant, castle_spoiler;
}
//--------------------------------------------------------------------------//
void game::start(){
	while(true){
		turn(1);
		turn(-1);

		threefold();	// control threefold repetition

		if(checkmate_==true)
			cout<<endl<<"Checkmate!"<<endl<<endl;
		if(stalmate_==true)
			cout<<endl<<"Stalmate!"<<endl<<endl;
		if(draw_==true)
			cout<<endl<<"Draw!"<<endl<<endl;
		if(checkmate_==true || stalmate_==true || draw_==true)
			break;
	}
}

//--------------------------------------------------------------------------//
void game::turn(int color){

	int N_pieces=0, N_enemies=0, N_checks=0, N_defs=0, N_locked=0;
	piece **pieces, **enemies, **checkenemies, **defender, **locked;

	pieces = new piece*[2*n_];
	enemies = new piece*[2*n_];
	checkenemies = new piece*[2];

	int n=0, N_moves=0, N_castle=0, xking=-1, yking=-1;

	//----------------------------------------------------------------------------------//
	//------------------- Find the pieces, the king and the enemies --------------------//
	//----------------------------------------------------------------------------------//
	for(int i=0;i<n_;i++)
		for(int j=0;j<n_;j++)
			if(chessboard_[i][j]->getcolor()==color){
				pieces[N_pieces]=chessboard_[i][j];
				N_pieces++;
				if(chessboard_[i][j]->getid()==6){
					xking=i;
					yking=j;
				}
			}else if(chessboard_[i][j]->getcolor()==-color){
				enemies[N_enemies]=chessboard_[i][j];
				N_enemies++;
			}
	parity_verify(pieces,enemies,N_pieces,N_enemies);	// check if it is a draw

	//----------------------------------------------------------------------------------//
	//-------------- Save the dangerous cells and control if it is check ---------------//
	//----------------------------------------------------------------------------------//
	board *dangermatrix=new board();	// checks matrix
	chessboard_[xking][yking]=new empty(xking,yking,BD);	// temporarely empty the king cell (king can't hide behind itself)

	for(int i=0;i<n_;i++)
		for(int j=0;j<n_;j++)
			if(chessboard_[i][j]->getcolor()==-color){
				if(chessboard_[i][j]->getid()!=1){
					for(int k=0;k<n_*n_;k++){	// range all the moves
						if(chessboard_[i][j]->move()[0][k]==-9)	// in this case stops at -9
							break;
						if(chessboard_[i][j]->move()[0][k]!=-1){	// jump the -1 index
							dangermatrix->fillcell(chessboard_[i][j]->move()[0][k],chessboard_[i][j]->move()[1][k],dangermatrix->verifycell(chessboard_[i][j]->move()[0][k],chessboard_[i][j]->move()[1][k])+1);
							if(chessboard_[i][j]->move()[0][k]==xking && chessboard_[i][j]->move()[1][k]==yking){
								checkenemies[N_checks]=chessboard_[i][j];
								N_checks++;
							}
						}
					}
				}else{	// check and dangermatrix by pawns
					if(j-color>=0 && j-color<n_ && i+1>=0 && i+1<n_)
						dangermatrix->fillcell(i+1,j-color,dangermatrix->verifycell(i+1,j-color)+1);
					if(j-color>=0 && j-color<n_ && i-1>=0 && i-1<n_)
						dangermatrix->fillcell(i-1,j-color,dangermatrix->verifycell(i-1,j-color)+1);

					for(int s=-1;s<2;s=s+2)
						if(i+s==xking && j-color==yking){	// pawns giving check are here
							checkenemies[N_checks]=chessboard_[i][j];
							N_checks++;
						}
				}
			}
	chessboard_[xking][yking]=new king(xking,yking,color,BD);	// fill again the king cell

	if(N_checks>0){
		cout<<"Check by ";
		for(int i=0;i<N_checks;i++){
			cout<<checkenemies[i]->getname()<<" ";
			BD->cellname(checkenemies[i]->getx(),checkenemies[i]->gety());
			if(N_checks>1 && i==0)
				cout<<" and ";
		}
		cout<<endl;
	}

	//----------------------------------------------------------------------------------//
	//---------------------------- Look for locked pieces ------------------------------//
	//----------------------------------------------------------------------------------//


	locked=new piece*[2*n_];	// locked pieces that cover the king from a check
	int ***lockmove=new int**[n_];	// moves of the locked pieces along the covering direction are allowed

	for(int i=0;i<n_;i++)
		lockmove[i]=new int*[2];
	for(int i=0;i<2;i++)
		for(int j=0;j<n_;j++)
			lockmove[i][j]=new int[n_];

	for(int s=-1;s<2;s++){
		for(int t=-1;t<2;t++){
			if(!(s==0 && t==0)){
				bool direction=false;
				n=0;
				for(int d=n_;d>0;d--){
					for(int i=0;i<N_enemies;i++){
						if(enemies[i]->getx()==xking+s*d && enemies[i]->gety()==yking+t*d)
							if(enemies[i]->getid()!=2 && enemies[i]->getid()!=6 && enemies[i]->getid()!=1)
								direction=true;

						if(direction==true){

							if(!(lockmove[N_locked][0][n-1]==xking+s*d && lockmove[N_locked][1][n-1]==yking+t*d)){
								lockmove[N_locked][0][n]=xking+s*d;
								lockmove[N_locked][1][n]=yking+t*d;
								n++;
								lockmove[N_locked][0][n]=-1;	// marks the end
								lockmove[N_locked][1][n]=-1;
							}

							for(int j=0;j<N_pieces;j++){
								if(pieces[j]->getx()==xking+s*d && pieces[j]->gety()==yking+t*d){
									for(int k=0;k<n_*n_;k++){
										if(enemies[i]->move()[0][k]==-1)
											break;

										if(enemies[i]->move()[0][k]==xking+s*d && enemies[i]->move()[1][k]==yking+t*d){

											bool behind=true;	// check that the cells behind the piece are empty
											while(d>1){	// d>1 because we are checking around the king
												d--;

												if(!(lockmove[N_locked][0][n-1]==xking+s*d && lockmove[N_locked][1][n-1]==yking+t*d)){
													lockmove[N_locked][0][n]=xking+s*d;
													lockmove[N_locked][1][n]=yking+t*d;
													n++;
													lockmove[N_locked][0][n]=-1;	// marks the end
													lockmove[N_locked][1][n]=-1;
												}

												if(chessboard_[xking+s*d][yking+t*d]->getid()!=0)
													behind=false;
											}

											if(behind==true){
												locked[N_locked]=pieces[j];
												N_locked++;
												n=0;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//----------------------------------------------------------------------------------//
	//-------------------- If it is check look for defending pieces --------------------//
	//----------------------------------------------------------------------------------//

	defender=new piece*[2*n_];	// pieces who can cover the check or take the piece giving check
	int **mossa=new int*[2*n_];	// moves of the defender pieces
	for(int i=0;i<2*n_;i++)
		mossa[i]=new int[n_*n_];

	n=0;
	if(N_checks>0){
		for(int k=0;k<n_;k++){	// King can run away (it is a defending piece)
			if(chessboard_[xking][yking]->move()[0][k]==-1)
				break;
			if(dangermatrix->verifycell(chessboard_[xking][yking]->move()[0][k],chessboard_[xking][yking]->move()[1][k])==0){
				defender[N_defs]=chessboard_[xking][yking];
				mossa[N_defs][n]=k;
				n++;
			}
		}
		mossa[N_defs][n]=-1;
		if(n>0)
			N_defs++;
		n=0;

		if(N_checks==1){	// other pieces can defend only if the check is given by a single enemy piece
			for(int j=0;j<N_pieces;j++){
				for(int k=0;k<n_*n_;k++){
					if(pieces[j]->move()[0][k]==-1)
						break;
					if(pieces[j]->getid()!=6 && checkenemies[0]->getid()!=2){
						int L=bigger(checkenemies[0]->getx()-xking,checkenemies[0]->gety()-yking);

						for(int s=-1;s<2;s++)
							for(int t=-1;t<2;t++)
								if(!(s==0 && t==0)){
									bool direction=false;
									for(int d=L;d>0;d--){
										if(checkenemies[0]->getx()==xking+s*d && checkenemies[0]->gety()==yking+t*d)
											direction=true;

										if(direction==true){
											if(pieces[j]->move()[0][k]==xking+s*d && pieces[j]->move()[1][k]==yking+t*d){
												if(pieces[j]==defender[N_defs-1])
													N_defs--;
												else
													defender[N_defs]=pieces[j];
												mossa[N_defs][n]=k;
												n++;
												mossa[N_defs][n]=-1;	// prepare the next spot
												N_defs++;
											}
										}
									}
								}

					}else if(pieces[j]->getid()!=6 && checkenemies[0]->getid()==2){
						if(pieces[j]->move()[0][k]==checkenemies[0]->getx() && pieces[j]->move()[1][k]==checkenemies[0]->gety()){
							if(pieces[j]==defender[N_defs-1])
								N_defs--;
							else
								defender[N_defs]=pieces[j];
							mossa[N_defs][n]=k;
							n++;
							mossa[N_defs][n]=-1;	// prepare the next spot
							N_defs++;
						}
					}
				}
				n=0;
			}

		}

		if(N_defs>0){
			delete[] pieces;
			pieces = new piece*[2*n_];

			for(int j=0;j<N_defs;j++)
				pieces[j]=defender[j];

			delete[] defender;
			N_pieces=N_defs;
		}else{
			checkmate_=true;
		}
	}

	//----------------------------------------------------------------------------------//
	//---------------------------------- Make the move ---------------------------------//
	//----------------------------------------------------------------------------------//

	//----------------------------------------------------------------------------------//
	//--------------------------- Search the movable pieces ----------------------------//
	//----------------------------------------------------------------------------------//

	piece **temp = new piece*[2*n_];	// temporary array to store the pieces that can move
	n=0;
	for(int j=0;j<N_pieces;j++){
		while((N_checks==0 && pieces[j]->move()[0][N_moves]!=-1) || (N_checks>0 && mossa[j][N_moves]!=-1))
			N_moves++;

		if(N_locked>0){
			for(int i=0;i<N_locked;i++){
				if(pieces[j]==locked[i]){
					N_moves=0;
					for(int k1=0;k1<n_;k1++){
						if(lockmove[i][0][k1]==-1)
							break;
						for(int k2=0;k2<n_*n_;k2++){
							if(pieces[j]->move()[0][k2]==lockmove[i][0][k1] && pieces[j]->move()[1][k2]==lockmove[i][1][k1])
								N_moves++;
						}
					}
				}
			}
		}

		if(pieces[j]->getid()==6){	// check if the king can move
			N_moves=0;
			for(int k=0;k<n_;k++){
				if(pieces[j]->move()[0][k]==-1)
					break;
				if(dangermatrix->verifycell(pieces[j]->move()[0][k],pieces[j]->move()[1][k])==0)
					if(chessboard_[pieces[j]->move()[0][k]][pieces[j]->move()[1][k]]->getid()==0 || chessboard_[pieces[j]->move()[0][k]][pieces[j]->move()[1][k]]->getcolor()==-color)
						N_moves++;
			}
		}

		if(N_moves!=0){
			temp[n]=pieces[j];
			n++;
		}
		N_moves=0;
	}

	if(n==0 && N_checks==0){	// control if it is stalmate
		stalmate_=true;
	}else{
		delete[] pieces;
		pieces = new piece*[2*n_];
		for(int i=0;i<n;i++)
			pieces[i]=temp[i];
		delete[] temp;
		N_pieces=n;
	}


	while(checkmate_==false && stalmate_==false && draw_==false){	// continue until a move is made

		//----------------------------------------------------------------------------------//
		//------------------------------- Select the piece ---------------------------------//
		//----------------------------------------------------------------------------------//

		cout<<"Select a piece: ";
		for(int i=0;i<N_pieces;i++){
			cout<<pieces[i]->getname()<<" ";
			BD->cellname(pieces[i]->getx(),pieces[i]->gety());
			cout<<" ("<<i+1<<")";
			if(i<N_pieces-1)
				cout<<", ";
			else
				cout<<": ";
		}

		int a;
		while(!(cin>>a) && a<1 || a>N_pieces){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout<<"Select a valid piece: ";
		}

		piece* P=pieces[a-1];	// selected piece
		cout<<P->getname()<<" ";
		BD->cellname(P->getx(),P->gety());
		cout<<" (0) selected: ";

		enpassant(P,-1,-1,color);	// enpassant prepare

		//----------------------------------------------------------------------------------//
		//--------------------------- Search the allowed moves -----------------------------//
		//----------------------------------------------------------------------------------//

		bool castle1now=false, castle2now=false;

		N_moves=0;
		N_castle=0;
		if(P->getid()==6){	// If the piece selected is the king N_moves is different
			for(int k=0;k<n_;k++){
				if(P->move()[0][k]==-1)
					break;
				if(chessboard_[P->move()[0][k]][P->move()[1][k]]->getid()==0 || chessboard_[P->move()[0][k]][P->move()[1][k]]->getcolor()==-color)
					if(dangermatrix->verifycell(P->move()[0][k],P->move()[1][k])==0)
						N_moves++;
			}

			if(N_checks==0){
				if(castle_spoiler[0]==false && color==1 || castle_spoiler[2]==false && color==-1)

					if(dangermatrix->verifycell(xking+1,yking)==0 && dangermatrix->verifycell(xking+2,yking)==0)	// count short castle move
						if(chessboard_[xking+1][yking]->getid()==0 && chessboard_[xking+2][yking]->getid()==0)
							if(chessboard_[7][yking]->getid()==4 && chessboard_[7][yking]->getcolor()==color){
								N_moves++;
								N_castle++;
								castle1now=true;
							}
				if(castle_spoiler[1]==false && color==1 || castle_spoiler[3]==false && color==-1)
					if(dangermatrix->verifycell(xking-1,yking)==0 && dangermatrix->verifycell(xking-2,yking)==0)	// count long castle move
						if(chessboard_[xking-1][yking]->getid()==0 && chessboard_[xking-2][yking]->getid()==0 && chessboard_[xking-3][yking]->getid()==0)
							if(chessboard_[0][yking]->getid()==4 && chessboard_[0][yking]->getcolor()==color){
								N_moves++;
								N_castle++;
								castle2now=true;
							}
			}
		}else{	// calculate the number of moves if the piece is not the king
			while((N_checks==0 && P->move()[0][N_moves]!=-1) || (N_checks>0 && mossa[a-1][N_moves]!=-1))
				N_moves++;
		}

		int **allowed=new int*[2];
		for(int i=0;i<2;i++)
			allowed[i] = new int[N_moves];

		n=0;
		if(P->getid()==6){	// if the piece is the king it cannot go in a dangerous cell
			for(int k=0;k<n_;k++){
				if(P->move()[0][k]==-1)
					break;
				if(chessboard_[P->move()[0][k]][P->move()[1][k]]->getid()==0 || chessboard_[P->move()[0][k]][P->move()[1][k]]->getcolor()==-color)
					if(dangermatrix->verifycell(P->move()[0][k],P->move()[1][k])==0){
						allowed[0][n]=P->move()[0][k];
						allowed[1][n]=P->move()[1][k];
						n++;
					}
			}

			if(castle1now==true){	// short castle
				allowed[0][n]=6;
				allowed[1][n]=yking;
				n++;
			}

			if(castle2now==true){	// long castle
				allowed[0][n]=2;
				allowed[1][n]=yking;
				n++;
			}

		}else{

			if(N_checks>0){

				for(int i=0;i<N_locked;i++)
					if(P==locked[i])
						break;	// check if it works properly

				for(int k=0;k<n_*n_;k++){
					if(mossa[a-1][k]==-1)
						break;
					allowed[0][n]=P->move()[0][mossa[a-1][k]];
					allowed[1][n]=P->move()[1][mossa[a-1][k]];
					n++;
				}

			}else{

				if(N_locked>0){
					for(int i=0;i<N_locked;i++){
						if(P==locked[i]){
							N_moves=0;
							for(int k1=0;k1<n_;k1++){
								if(lockmove[i][0][k1]==-1)
									break;
								for(int k2=0;k2<n_*n_;k2++){
									if(P->move()[0][k2]==lockmove[i][0][k1] && P->move()[1][k2]==lockmove[i][1][k1])
										N_moves++;
								}
							}

							delete[] allowed;
							allowed=new int*[2];
							for(int j=0;j<2;j++)
								allowed[j] = new int[N_moves];

							for(int k1=0;k1<n_;k1++){
								if(lockmove[i][0][k1]==-1)
									break;
								for(int k2=0;k2<n_*n_;k2++){
									if(P->move()[0][k2]==lockmove[i][0][k1] && P->move()[1][k2]==lockmove[i][1][k1]){
										allowed[0][n]=lockmove[i][0][k1];
										allowed[1][n]=lockmove[i][1][k1];
										n++;
									}
								}
							}
						}else{
							for(int i=0;i<N_moves;i++)
								for(int j=0;j<2;j++)
									allowed[j][i]=P->move()[j][i];
						}
					}
				}else{
					for(int i=0;i<N_moves;i++)
						for(int j=0;j<2;j++)
							allowed[j][i]=P->move()[j][i];
				}
			}
		}
		//----------------------------------------------------------------------------------//
		//----------------------------------------------------------------------------------//
		//----------------------------------------------------------------------------------//

		for(int i=0;i<N_moves;i++){
			BD->cellname(allowed[0][i],allowed[1][i]);
			if(chessboard_[allowed[0][i]][allowed[1][i]]->getcolor()==-color)
				cout<<"->";
			cout<<" ("<<i+1<<")";
			if(i<N_moves-1)
				cout<<", ";
			else
				cout<<": ";
		}

		int b;
		while(!(cin>>b) && b<1 || b>N_moves){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout<<"Select a valid move: ";
		}
		int X=allowed[0][b-1], Y=allowed[1][b-1];

		if(b==0){
			cout<<P->getname()<<" not moved"<<endl;
		}else{

			enpassant(P,X,Y,color);	// enpassant reset

			chessboard_[P->getx()][P->gety()]=new empty(P->getx(),P->gety(),BD);
			chessboard_[X][Y]=P;
			chessboard_[X][Y]->setposition(X,Y);

			if(castle1now==true && X==6){	// create new rook in case it is castle
				chessboard_[7][yking]=new empty(7,yking,BD);
				chessboard_[5][yking]=new rook(5,yking,color,BD);
				cout<<"Castled Kingside";
			}else if(castle2now==true && X==2){
				chessboard_[0][yking]=new empty(0,yking,BD);
				chessboard_[3][yking]=new rook(3,yking,color,BD);
				cout<<"Castled Queenside";
			}else{
				cout<<P->getname()<<" moved to ";
				BD->cellname(X,Y);
			}
			if(chessboard_[X][Y]->getid()==1 && (Y==0 || Y==7))
				pawn_promotion(X,Y,color);	// pawn promotion

			cout<<endl;

			castlespoiled(P,color); // spoils the castles if the rooks or king are moved
			break; // if the move is done the turn finishes
		}

	}
	BD->printboard();
	delete[] pieces, enemies, checkenemies, dangermatrix, mossa, locked, lockmove;
}

//----------------------------------------------------------------------------------//
void game::pawn_promotion(int x, int y, int color){
	cout<<endl<<"Promote the pawn to Queen (1), Rook (2), Bishop (3), Knight (4): ";
	int a;
	while(!(cin>>a) && a<1 || a>4){
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');
		cout<<"Select a valid choice: ";
	}

	switch(a){
		case 1:
			chessboard_[x][y]=new queen(x,y,color,BD);
			break;
		case 2:
			chessboard_[x][y]=new rook(x,y,color,BD);
			break;
		case 3:
			chessboard_[x][y]=new bishop(x,y,color,BD);
			break;
		case 4:
			chessboard_[x][y]=new knight(x,y,color,BD);
			break;
	}
	cout<<"Pawn promoted to "<<chessboard_[x][y]->getname();
}
//----------------------------------------------------------------------------------//

void game::enpassant(piece* P, int X, int Y, int color){
	if(X==-1 && Y==-1){	// enpassant prepare
		if(P->getid()==1 && passant_==true){
			if(P->getx()-1>=0 && chessboard_[P->getx()-1][P->gety()]->getid()==1)
				if(passant[0]==chessboard_[P->getx()-1][P->gety()]->getx() && passant[1]==chessboard_[P->getx()-1][P->gety()]->gety())
					chessboard_[P->getx()-1][P->gety()+color]=new pawn(P->getx()-1,P->gety()+color,-color,BD);
			if(P->getx()+1<n_ && chessboard_[P->getx()+1][P->gety()]->getid()==1)
				if(passant[0]==chessboard_[P->getx()+1][P->gety()]->getx() && passant[1]==chessboard_[P->getx()+1][P->gety()]->gety())
					chessboard_[P->getx()+1][P->gety()+color]=new pawn(P->getx()+1,P->gety()+color,-color,BD);
		}
	}else{	// enpassant reset
		if(P->getid()==1 && passant_==true && P->gety()==passant[1]){
			int s=1;
			if(P->getx()==passant[0]+1)
				s=-1;

			if(chessboard_[P->getx()+s][P->gety()]->getid()==1){
				if(X==passant[0])
					chessboard_[P->getx()+s][P->gety()]=new empty(P->getx()+s,P->gety(),BD);
				else
					chessboard_[P->getx()+s][P->gety()+color]=new empty(P->getx()+s,P->gety()+color,BD);
			}
		}
		passant_=false;
		passant[0]=-1;
		passant[1]=-1;

		if(P->getid()==1 && Y==P->gety()+2*color){
			passant_=true;
			passant[0]=X;
			passant[1]=Y;
		}
	}
}

//----------------------------------------------------------------------------------//
void game::castlespoiled(piece* P, int color){
	if(P->getid()==6 || P->getid()==4 && P->getx()==7 || chessboard_[7][0]->getid()!=4 && chessboard_[7][0]->getcolor()!=color)
		if(color==1)
			castle_spoiler[0]=true;
		else
			castle_spoiler[2]=true;
	if(P->getid()==6 || P->getid()==4 && P->getx()==0 || chessboard_[0][0]->getid()!=4 && chessboard_[0][0]->getcolor()!=color)
		if(color==1)
			castle_spoiler[1]=true;
		else
			castle_spoiler[3]=true;
}

//----------------------------------------------------------------------------------//
void game::parity_verify(piece** pieces, piece** enemies, int N_pieces, int N_enemies){
	double value1=0, value2=0;
	for(int i=0;i<N_pieces;i++)
		value1=value1+pieces[i]->getvalue();
	for(int i=0;i<N_enemies;i++)
		value2=value2+enemies[i]->getvalue();

	value1=value1-10000;	// remove the king value of 10000
	value2=value2-10000;

	if(value1<0.1 && value2<0.1)
		draw_=true;	// king vs king

	if((value1<3.025 && value1>3.005 && value2<0.1) || (value2<3.025 && value2>3.005 && value1<0.1))
		draw_=true;	// knight or bishop vs king

	if(value1<3.025 && value1>3.005 && value2<3.025 && value2>3.005)
		draw_=true;	// knight or bishop vs knight or bishop (sometimes it can be mate but can't be forced)

	if((value1>6.015 && value1<6.025 && value2<0.1) || (value2>6.015 && value2<6.025 && value1<0.1))
		draw_=true;	// two knights vs king (mate can't be forced)

	if(value2==0)	// 50 moves repetition
		N_50draw++;
	if(N_50draw==50)
		draw_=true;
}

//----------------------------------------------------------------------------------//
void game::threefold(){	// the threefold repetition rule is, in general, slightly more complicated than this

	if(BD_old2->compare(BD)==false)
		N_3rep=0;
	else
		N_3rep++;

	for(int i=0;i<n_;i++)
		for(int j=0;j<n_;j++)
			BD_old2->fillcell(i,j,BD_old1->getboard(i,j));

	for(int i=0;i<n_;i++)
		for(int j=0;j<n_;j++)
			BD_old1->fillcell(i,j,BD->getboard(i,j));

	if(N_3rep==3)
		draw_=true;

}

//----------------------------------------------------------------------------------//
