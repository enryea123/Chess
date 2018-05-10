comp: main.o board.o piece.o game.o
	g++ -o chess main.o board.o piece.o game.o

main.o: main.cxx
	g++ -o main.o -c main.cxx

board.o: board.cxx board.h
	g++ -o board.o -c board.cxx

piece.o: piece.cxx piece.h
	g++ -o piece.o -c piece.cxx

game.o: game.cxx game.h
	g++ -o game.o -c game.cxx
