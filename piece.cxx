#include <iostream>
#include <limits>
#include "board.h"
#include "piece.h"
using namespace std;

//--------------------------------------------------------------------------//
int piece::getx(){
	return x_;
}

int piece::gety(){
	return y_;
}

int piece::getcolor(){
	return color_;
}

int piece::getid(){
	return id_;
}

double piece::getvalue(){
	return value_;
}

string piece::getname(){
	return name_;
}

void piece::setposition(int x, int y){
	BOO->fillcell(x_,y_,0);
	x_=x;
	y_=y;
	BOO->fillcell(x,y,color_*id_);
}

void piece::getinfo(){
	cout<<"-----------"<<endl;
	cout<<" "<<name_<<endl;
	cout<<" x:\t"<<x_<<endl;
	cout<<" y:\t"<<y_<<endl;
	cout<<" color:\t"<<color_<<endl;
	cout<<" id:\t"<<id_<<endl;
	cout<<" id*c:\t"<<id_*color_<<endl;
	cout<<"-----------"<<endl;
}

//--------------------------------------------------------------------------//
empty::empty(int x, int y, board* BD){
	x_=x;
	y_=y;
	color_=0;
	value_=0;
	id_=0;
	name_="Empty cell";
	BOO=BD;
	BOO->fillcell(x_,y_,0);
}
empty::~empty(){}

int** empty::move(){}

//--------------------------------------------------------------------------//
pawn::pawn(int x, int y, int color, board* BD){
	x_=x;
	y_=y;
	color_=color;
	value_=1;
	id_=1;
	name_="Pawn";
	BOO=BD;
	BOO->fillcell(x_,y_,color_*id_);
}
pawn::~pawn(){}

int** pawn::move(){

	int N=0, P=0, x0, y0, **allowed, **protect;
	allowed = new int*[2];
	protect = new int*[2];
	for(int i=0;i<2;i++){
		allowed[i] = new int[n_*n_];
		protect[i] = new int[n_];
	}

	if(y_+color_<n_ && y_+color_>=0){
		if(BOO->verifycell(x_,y_+color_)==0){
				allowed[0][N]=x_;
				allowed[1][N]=y_+color_;
				N++;
		}
		if(x_+1<n_)
			if(BOO->verifycell(x_+1,y_+color_)==-color_){
					allowed[0][N]=x_+1;
					allowed[1][N]=y_+color_;
					N++;
			}else{
				protect[0][P]=x_+1;
				protect[1][P]=y_+color_;
				P++;
			}
		if(x_-1>=0)
			if(BOO->verifycell(x_-1,y_+color_)==-color_){
					allowed[0][N]=x_-1;
					allowed[1][N]=y_+color_;
					N++;
			}else{
				protect[0][P]=x_-1;
				protect[1][P]=y_+color_;
				P++;
			}
	}
	if((y_==1 && color_==1) || (y_==6 && color_==-1))
		if(BOO->verifycell(x_,y_+color_)==0 && BOO->verifycell(x_,y_+2*color_)==0){
				allowed[0][N]=x_;
				allowed[1][N]=y_+2*color_;
				N++;
		}

	allowed[0][N]=-1;
	allowed[1][N]=-1;
	N++;
	if(P>0)
		for(int i=0;i<P;i++){
			allowed[0][N+i]=protect[0][i];
			allowed[1][N+i]=protect[1][i];
		}
	allowed[0][N+P]=-9;
	allowed[1][N+P]=-9;
	return allowed;
}

//--------------------------------------------------------------------------//
knight::knight(int x, int y, int color, board* BD){
	x_=x;
	y_=y;
	color_=color;
	value_=3.01;
	id_=2;
	name_="Knight";
	BOO=BD;
	BOO->fillcell(x_,y_,color_*id_);
}
knight::~knight(){}

int** knight::move(){

	int N=0, P=0, x0, y0, **allowed, **protect;
	allowed = new int*[2];
	protect = new int*[2];
	for(int i=0;i<2;i++){
		allowed[i] = new int[n_*n_];
		protect[i] = new int[n_];
	}

	for(int i=-2;i<3;i++){
		for(int j=-2;j<3;j++){
			if(i!=0 && j!=0 && i!=j && i!=-j){
				x0=x_+i;
				y0=y_+j;
				if(x0<n_ && x0>=0 && y0<n_ && y0>=0){
					if(BOO->verifycell(x0,y0)!=color_){
						allowed[0][N]=x0;
						allowed[1][N]=y0;
						N++;
					}else{
						protect[0][P]=x0;
						protect[1][P]=y0;
						P++;
					}
				}
			}
		}
	}

	allowed[0][N]=-1;
	allowed[1][N]=-1;
	N++;
	if(P>0)
		for(int i=0;i<P;i++){
			allowed[0][N+i]=protect[0][i];
			allowed[1][N+i]=protect[1][i];
		}
	allowed[0][N+P]=-9;
	allowed[1][N+P]=-9;
	return allowed;
}
//--------------------------------------------------------------------------//

bishop::bishop(int x, int y, int color, board* BD){
	x_=x;
	y_=y;
	color_=color;
	value_=3.02;
	id_=3;
	name_="Bishop";
	BOO=BD;
	BOO->fillcell(x_,y_,color_*id_);
}
bishop::~bishop(){}

int** bishop::move(){

	int N=0, P=0, d=0, x0, y0, **allowed, **protect;
	allowed = new int*[2];
	protect = new int*[2];
	for(int i=0;i<2;i++){
		allowed[i] = new int[n_*n_];
		protect[i] = new int[n_];
	}

	bool direction[n_];
	for(int i=0;i<n_;i++)
		direction[i]=false;

	for(int s=-1;s<2;s++){
		for(int t=-1;t<2;t++){
			if(s!=0 && t!=0){	// bishop condition
				for(int i=1;i<n_;i++){

					x0=x_+i*s;
					y0=y_+i*t;
					if(x0<n_ && x0>=0 && y0<n_ && y0>=0 && direction[d]==false){
						if(BOO->verifycell(x0,y0)!=color_){
							allowed[0][N]=x0;
							allowed[1][N]=y0;
							N++;
							if(BOO->verifycell(x0,y0)==-color_)
								direction[d]=true;
						}else{
							protect[0][P]=x0;
							protect[1][P]=y0;
							P++;
							direction[d]=true;
						}
					}
				}
				d++;
			}
		}
	}

	allowed[0][N]=-1;
	allowed[1][N]=-1;
	N++;
	if(P>0)
		for(int i=0;i<P;i++){
			allowed[0][N+i]=protect[0][i];
			allowed[1][N+i]=protect[1][i];
		}
	allowed[0][N+P]=-9;
	allowed[1][N+P]=-9;
	return allowed;
}

//--------------------------------------------------------------------------//

rook::rook(int x, int y, int color, board* BD){
	x_=x;
	y_=y;
	color_=color;
	value_=5;
	id_=4;
	name_="Rook";
	BOO=BD;
	BOO->fillcell(x,y,color*id_);
}
rook::~rook(){}

int** rook::move(){

	int N=0, P=0, d=0, x0, y0, **allowed, **protect;
	allowed = new int*[2];
	protect = new int*[2];
	for(int i=0;i<2;i++){
		allowed[i] = new int[n_*n_];
		protect[i] = new int[n_];
	}

	bool direction[n_];
	for(int i=0;i<n_;i++)
		direction[i]=false;

	for(int s=-1;s<2;s++){
		for(int t=-1;t<2;t++){
			if(!(s==0 && t==0) && (s==0 || t==0)){	// rook condition
				for(int i=1;i<n_;i++){

					x0=x_+i*s;
					y0=y_+i*t;
					if(x0<n_ && x0>=0 && y0<n_ && y0>=0 && direction[d]==false){
						if(BOO->verifycell(x0,y0)!=color_){
							allowed[0][N]=x0;
							allowed[1][N]=y0;
							N++;
							if(BOO->verifycell(x0,y0)==-color_)
								direction[d]=true;
						}else{
							protect[0][P]=x0;
							protect[1][P]=y0;
							P++;
							direction[d]=true;
						}
					}
				}
				d++;
			}
		}
	}

	allowed[0][N]=-1;
	allowed[1][N]=-1;
	N++;
	if(P>0)
		for(int i=0;i<P;i++){
			allowed[0][N+i]=protect[0][i];
			allowed[1][N+i]=protect[1][i];
		}
	allowed[0][N+P]=-9;
	allowed[1][N+P]=-9;
	return allowed;
}

//--------------------------------------------------------------------------//

queen::queen(int x, int y, int color, board* BD){
	x_=x;
	y_=y;
	color_=color;
	value_=9;
	id_=5;
	name_="Queen";
	BOO=BD;
	BOO->fillcell(x,y,color*id_);
}
queen::~queen(){}

int** queen::move(){

	int N=0, P=0, d=0, x0, y0, **allowed, **protect;
	allowed = new int*[2];
	protect = new int*[2];
	for(int i=0;i<2;i++){
		allowed[i] = new int[n_*n_];
		protect[i] = new int[n_];
	}

	bool direction[n_];
	for(int i=0;i<n_;i++)
		direction[i]=false;

	for(int s=-1;s<2;s++){
		for(int t=-1;t<2;t++){
			if(!(s==0 && t==0)){	// queen condition
				for(int i=1;i<n_;i++){

					x0=x_+i*s;
					y0=y_+i*t;
					if(x0<n_ && x0>=0 && y0<n_ && y0>=0 && direction[d]==false){
						if(BOO->verifycell(x0,y0)!=color_){
							allowed[0][N]=x0;
							allowed[1][N]=y0;
							N++;
							if(BOO->verifycell(x0,y0)==-color_)
								direction[d]=true;
						}else{
							protect[0][P]=x0;
							protect[1][P]=y0;
							P++;
							direction[d]=true;
						}
					}
				}
				d++;
			}
		}
	}

	allowed[0][N]=-1;
	allowed[1][N]=-1;
	N++;
	if(P>0)
		for(int i=0;i<P;i++){
			allowed[0][N+i]=protect[0][i];
			allowed[1][N+i]=protect[1][i];
		}
	allowed[0][N+P]=-9;
	allowed[1][N+P]=-9;
	return allowed;
}

//--------------------------------------------------------------------------//

king::king(int x, int y, int color, board* BD){
	x_=x;
	y_=y;
	color_=color;
	value_=10000;
	id_=6;
	name_="King";
	BOO=BD;
	BOO->fillcell(x_,y_,color_*id_);
}
king::~king(){}

int** king::move(){

	int N=0, P=0, d=0, x0, y0, **allowed, **protect;
	allowed = new int*[2];
	protect = new int*[2];
	for(int i=0;i<2;i++){
		allowed[i] = new int[n_*n_];
		protect[i] = new int[n_];
	}

	bool direction[n_];
	for(int i=0;i<n_;i++)
		direction[i]=false;

	for(int s=-1;s<2;s++){
		for(int t=-1;t<2;t++){
			if(!(s==0 && t==0)){	// king condition
				for(int i=1;i<2;i++){

					x0=x_+i*s;
					y0=y_+i*t;
					if(x0<n_ && x0>=0 && y0<n_ && y0>=0 && direction[d]==false){
						if(BOO->verifycell(x0,y0)!=color_){
							allowed[0][N]=x0;
							allowed[1][N]=y0;
							N++;
							if(BOO->verifycell(x0,y0)==-color_)
								direction[d]=true;
						}else{
							protect[0][P]=x0;
							protect[1][P]=y0;
							P++;
							direction[d]=true;
						}
					}
				}
				d++;
			}
		}
	}

	allowed[0][N]=-1;
	allowed[1][N]=-1;
	N++;
	if(P>0)
		for(int i=0;i<P;i++){
			allowed[0][N+i]=protect[0][i];
			allowed[1][N+i]=protect[1][i];
		}
	allowed[0][N+P]=-9;
	allowed[1][N+P]=-9;
	return allowed;
}

//--------------------------------------------------------------------------//
