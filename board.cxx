#include <iostream>
#include <stdlib.h>
#include "board.h"
using namespace std;

//--------------------------------------------------------------------------//
board::board(){
	matrixboard_ = new int*[n_];

	for(int i=0;i<n_;i++)
		matrixboard_[i] = new int[n_];

	for(int i=0;i<n_;i++)
		for(int j=0;j<n_;j++)
			matrixboard_[i][j]=0;
}
board::~board(){
	for(int i=0;i<n_;i++)
		delete[] matrixboard_[i];
	delete[] matrixboard_;
}

//--------------------------------------------------------------------------//
void board::cellname(int x,int y){
	if(x>7 || x<0 || y>7 || y<0)
		cout<<"Error 0"<<endl;
	else
		switch(x){
			case 0:
				cout<<"A"<<y+1;
				break;

			case 1:
				cout<<"B"<<y+1;
				break;

			case 2:
				cout<<"C"<<y+1;
				break;

			case 3:
				cout<<"D"<<y+1;
				break;

			case 4:
				cout<<"E"<<y+1;
				break;

			case 5:
				cout<<"F"<<y+1;
				break;

			case 6:
				cout<<"G"<<y+1;
				break;

			case 7:
				cout<<"H"<<y+1;
				break;
		}
}

//--------------------------------------------------------------------------//
void board::printboard(){
	int count=0;
	cout<<" _ _______________________________________________ "<<endl;
	cout<<"| |     |     |     |     |     |     |     |     |"<<endl<<"|8|";
	for(int i=n_-1;i>=0;i--){
		for(int j=0;j<n_;j++){

			if(count>=n_){
				cout<<endl;
				cout<<"|_|_____|_____|_____|_____|_____|_____|_____|_____|"<<endl;
				cout<<"| |     |     |     |     |     |     |     |     |"<<endl<<"|"<<i+1<<"|";
//				cout<<"| |---------------------------------|"<<endl<<"|"<<i+1<<"|";
				count=0;
			}

			count++;

			if(matrixboard_[j][i]<0)
				cout<<" -";
			else
				cout<<"  ";
			switch(abs(matrixboard_[j][i])){
				case 0:
					cout<<"   |";
					break;
				case 1:
					cout<<"p  |";
					break;
				case 2:
					cout<<"k  |";
					break;
				case 3:
					cout<<"B  |";
					break;
				case 4:
					cout<<"R  |";
					break;
				case 5:
					cout<<"Q  |";
					break;
				case 6:
					cout<<"Ki |";
					break;
				default:
					cout<<"Board Error";
					break;
			}
		}
	}
	cout<<endl;
	cout<<"|_|_____|_____|_____|_____|_____|_____|_____|_____|"<<endl;
	cout<<"  |__A__|__B__|__C__|__D__|__E__|__F__|__G__|__H__|"<<endl;
	cout<<endl;
}
//--------------------------------------------------------------------------//
void board::printboard_small(){
	int count=0;
	cout<<" _ _________________________________ "<<endl;
	cout<<"| |                                 |"<<endl<<"|8|";
	for(int i=n_-1;i>=0;i--){
		for(int j=0;j<n_;j++){

			if(count>=n_){
				cout<<" |"<<endl;
				cout<<"| |                                 |"<<endl<<"|"<<i+1<<"|";
				count=0;
			}

			count++;

			if(matrixboard_[j][i]<0)
				cout<<" -";
			else
				cout<<"  ";
			switch(abs(matrixboard_[j][i])){
				case 0:
					cout<<". ";
					break;
				case 1:
					cout<<"p ";
					break;
				case 2:
					cout<<"k ";
					break;
				case 3:
					cout<<"B ";
					break;
				case 4:
					cout<<"R ";
					break;
				case 5:
					cout<<"Q ";
					break;
				case 6:
					cout<<"KI";
					break;
				default:
					cout<<"Board Error";
					break;
			}
		}
	}
	cout<<" |"<<endl;
	cout<<"|_|_________________________________|"<<endl;
	cout<<"  |__A___B___C___D___E___F___G___H__|"<<endl;
	cout<<endl;
}

//--------------------------------------------------------------------------//
void board::printnumbers(){
	int count=0;
	cout<<" _ _________________________________ "<<endl;
	cout<<"| |                                 |"<<endl<<"|8|";
	for(int i=n_-1;i>=0;i--){
		for(int j=0;j<n_;j++){

			if(count>=n_){
				cout<<" |"<<endl;
				cout<<"| |                                 |"<<endl<<"|"<<i+1<<"|";
				count=0;
			}
			count++;

			if(matrixboard_[j][i]>=0)
				cout<<" ";
			if(matrixboard_[j][i]==0)
				cout<<" . ";
			else
				cout<<" "<<matrixboard_[j][i]<<" ";

		}
	}
	cout<<" |"<<endl;
	cout<<"|_|_________________________________|"<<endl;
	cout<<"  |__A___B___C___D___E___F___G___H__|"<<endl;
	cout<<endl;
}

//--------------------------------------------------------------------------//
void board::fillcell(int x, int y, int Z){
	if(x<n_ && x>=0 && y<n_ && y>=0)
		matrixboard_[x][y]=Z;
}

//--------------------------------------------------------------------------//
int board::verifycell(int x, int y){
	if(x<n_ && x>=0 && y<n_ && y>=0){
		if(matrixboard_[x][y]>0)
			return 1;
		else if(matrixboard_[x][y]<0)
			return -1;
		else
			return 0;
	}
}
//--------------------------------------------------------------------------//
int board::verifypiece(int x, int y){
	if(x<n_ && x>=0 && y<n_ && y>=0)
		return abs(matrixboard_[x][y]);
}

//--------------------------------------------------------------------------//
int board::getboard(int x, int y){
	if(x<n_ && x>=0 && y<n_ && y>=0)
		return matrixboard_[x][y];
}

//--------------------------------------------------------------------------//
board* board::operator=(board* A){
	board* B=new board();
	for(int i=0;i<n_;i++)
		for(int j=0;j<n_;j++)
			B->fillcell(i,j,A->getboard(i,j));
	return B;
}

//--------------------------------------------------------------------------//
bool board::compare(board* B){
	bool equality=true;
	for(int i=0;i<n_;i++)
		for(int j=0;j<n_;j++)
			if(matrixboard_[i][j]!=B->getboard(i,j))
				equality=false;

	return equality;
}

//--------------------------------------------------------------------------//
