#pragma once

struct Piece {
	int value = 0;
	char color = -1;
	char FLAG = -1;
	inline Piece(int val, char c) {
		value = val;
		color = c;
	}
};

struct Move {
	int pos;
	unsigned FLAG = 0;
};




class Game {
public:
	Game();
	void printPosConsole();
	inline int getSelectedPiece() { return selectedPiece; };
	Piece* board[64];
	int  turn;
	int selectedPiece;
	Move *possibleMoves[60] = {};
	int prevMove = 0;
	struct{
		bool KingMoved = false;
		bool KingRookMoved = false;
		bool QueenRookMoved = false;
		int KingPos = 5;
	} White;
	struct Black {
		bool KingMoved = false;
		bool KingRookMoved = false;
		bool QueenRookMoved = false;
		int KingPos = 7 * 8 + 5;
	} Black;
	struct move {
		int pos;
		int FLAG;
	};
};
class Engine {
private:
	Game g;
public:
	Engine();
	Piece** getBoard() { return g.board; };
	void selectPiece(int pos);
	void  calculatePossibleMoves(Move* [], Piece* [], int, bool, int*);
	void calculateLegalMoves();
	void makeMove(Piece* [], int, int, int, int*);
	void processEvent(int pos);
	inline Piece* copyPiece(Piece* p) {
		
		return (p!= nullptr)?new Piece(p->value, p->color): nullptr;
	};
	void Test();

};

struct pair {
	int x;
	int y;
};

static class Utils {
public:
	static  pair map(int pos, int dim, bool boardFlipped);
	static pair postoXandY(pair , int );
	static int cordToPos(pair, int);
};

