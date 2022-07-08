#include <iostream>
#include "Engine.h";
#include <cmath>


/* Define standard values for pieces and there color*/
const enum PIECES {
	PAWN = 1, KNIGHT, BISHOP, ROOK, QUEEN, KING
};
const enum COLOR {
	WHITE = 0, BLACK
};

const enum moveType {
	NORMAL = 0 , CAPTURE,CHECK , CAPTURE_WITH_CHECK,  CASTLE, ENPASSANT
};

const int OUTOFINDEX = 65;
const char code[20] = { 'P','N','B','R','Q','K' ,0,0,0,0,0,0,0,0,'p','n','b','r','q','k' };

const int MAX = 60;



Engine::Engine() {
	g = Game();
	g.printPosConsole();
}

Game::Game() {

	// initialize turn as white
	turn = WHITE;

	// add nullptrs at all postions
	for (int i = 0; i < 64; i++) {
		board[i] = nullptr;
	}

	/*initialize a temprory pointer*/
	Piece* p;

	/* adding white pawns*/
	for (int i = 1 * 8 + 1; i < 2 * 8 + 1; i++) {
		p = new Piece(PAWN, WHITE);
		board[i - 1] = p;
	}
	/*adding black pawns*/
	for (int i = 6 * 8 + 1; i < 7 * 8 + 1; i++) {
		p = new Piece(PAWN, BLACK);
		board[i - 1] = p;
	}

	/*Adding Both the rooks for White*/
	p = new Piece(ROOK, WHITE);
	board[0] = p;
	p = new Piece(ROOK, WHITE);
	board[7] = p;

	/*Adding both the rooks for black*/
	p = new Piece(ROOK, BLACK);
	board[56] = p;
	p = new Piece(ROOK, BLACK);
	board[63] = p;

	/* Adding Both the Knights for WHITE*/
	p = new Piece(KNIGHT, WHITE);
	board[1] = p;
	p = new Piece(KNIGHT, WHITE);
	board[6] = p;
	/*Adding Both the Knights for Black*/
	p = new Piece(KNIGHT, BLACK);
	board[7 * 8 + 2 - 1] = p;
	p = new Piece(KNIGHT, BLACK);
	board[7 * 8 + 7 - 1] = p;

	/*Adding Both the Bishops for White*/
	p = new Piece(BISHOP, WHITE);
	board[3 - 1] = p;
	p = new Piece(BISHOP, WHITE);
	board[6 - 1] = p;
	/* Adding Both the bishops for black*/
	p = new Piece(BISHOP, BLACK);
	board[7 * 8 + 3 - 1] = p;
	p = new Piece(BISHOP, BLACK);
	board[7 * 8 + 6 - 1] = p;

	// Adding Both the Queens for White and Black
	p = new Piece(QUEEN, WHITE);
	board[4 - 1] = p;
	p = new Piece(QUEEN, BLACK);
	board[7 * 8 + 4 - 1] = p;

	//Adding Both the Kings
	p = new Piece(KING, WHITE);
	board[5 - 1] = p;
	p = new Piece(KING, BLACK);
	board[7 * 8 + 5 - 1] = p;


	//assign selected piece as 64 (out of index)
	selectedPiece = OUTOFINDEX;


}

void Engine::selectPiece(int p) {
	if (g.selectedPiece == p + 1)
		return;
	g.selectedPiece = p + 1;
}


// This Code is gonna be tough
void Engine::calculateLegalMoves() {
	int i = 0;
	int n = 0;
	Move* possibleMoves[MAX];
	for (int z = 0; z < MAX; z++) {
		g.possibleMoves[z] = new Move();
		g.possibleMoves[z]->pos = OUTOFINDEX;
	}
	calculatePossibleMoves(g.possibleMoves,g.board, g.selectedPiece, g.turn, &n);
	/*
	int placeholder = 0;
	Move* LegalMoves[MAX];

	Piece *fake[64];

	for (int j = 0; j < n+1; j++) {
		for (int k = 0; k < 64; k++) {
			fake[k] = copyPiece(g.board[k]);
		}
		bool moveValid = true;
		int king_pos = (g.turn == WHITE) ?  g.White.KingPos : g.Black.KingPos;
		makeMove(fake, g.selectedPiece, possibleMoves[j]->pos, possibleMoves[j]->FLAG , &king_pos);
		for (int l = 0; l < 64; l++) {
			if (!(fake[l] == nullptr)) {
				if (fake[l]->color != g.turn) {
					
					Move* opponentMoves[MAX];
					for (int n = 0; n < MAX; n++) {
						opponentMoves[n] = new Move();
						};
					calculatePossibleMoves(opponentMoves,fake, l + 1, !g.turn, &placeholder);
					for (int m = 0; m < placeholder + 1; m++) {
						if (opponentMoves[m]->pos == king_pos) {
							moveValid = false;
							delete opponentMoves;
							break;

						}
					}
				}
			}
		}
		if (moveValid) {
			LegalMoves[i++] = possibleMoves[j];
		}
		for (int k = 0; k < 64; i++) {
			if (fake[k] != nullptr)
				delete fake[k];
		}
		for (int k = 0; k < MAX; k++) {
			g.possibleMoves[i] = LegalMoves[i];
		}
		
	}

	*/
	
	
	
}

void  Engine::calculatePossibleMoves(Move *possibleMoves[], Piece* fakeBoard[], int selectedPiece, bool turn, int* x) {

	int i = 0;
	Piece* p = fakeBoard[selectedPiece - 1];
	if (!(turn == p->color))
		return;

	int pos = selectedPiece;


	switch (p->value) {
	case PAWN:
		if (p->color == WHITE) {

			//if pawn at last row 
			if (pos > 56)
				break;

			// check for front moves
			if (fakeBoard[pos + 8 - 1] == nullptr)
				possibleMoves[i++]->pos = pos + 8;

			// double pawnMove
			if (p->FLAG == -1) {
				if (fakeBoard[pos + 8 - 1] == nullptr
					&& fakeBoard[pos + 8 * 2 - 1] == nullptr)
					possibleMoves[i++]->pos = pos + 8 * 2;
			}

			// side move capture
			//right side capture 
			if (fakeBoard[pos + 9 - 1] != nullptr
				&& pos % 8 != 0) {
				if (fakeBoard[pos + 9 - 1]->color == BLACK)
					possibleMoves[i++]->pos = pos + 9;
			}
			// left side capture
			if (fakeBoard[pos + 7 - 1] != nullptr
				&& (pos - 1) % 8 != 0) {
				if (fakeBoard[pos + 7 - 1]->color == BLACK)
					possibleMoves[i++]->pos = pos + 7;
			}
		}
		else {

			//check if pawn at last row
			if (pos < 9)
				break;

			// check for front moves
			if (fakeBoard[pos - 8 - 1] == nullptr)
				possibleMoves[i++]->pos = pos - 8;

			// double pawnMove
			if (p->FLAG == -1) {
				if (fakeBoard[pos - 8 - 1] == nullptr
					&& fakeBoard[pos - 8 * 2 - 1] == nullptr)
					possibleMoves[i++]->pos = pos - 8 * 2;
			}

			// side move capture
			//right side capture
			if (fakeBoard[pos - 7 - 1] != nullptr
				&& pos % 8 != 0) {
				if (fakeBoard[pos - 7 - 1]->color == WHITE)
					possibleMoves[i++]->pos = pos - 7;
			}
			// left side capture
			if (fakeBoard[pos - 9 - 1] != nullptr
				&& (pos - 1) % 8 != 0) {
				if (fakeBoard[pos - 9 - 1]->color == WHITE)
					possibleMoves[i++]->pos = pos - 9;
			}
		}
		break;
	case ROOK:

		// horizontal lookup right
		if (!(pos % 8 == 0)) {
			for (int k = pos + 1;; k++) {
				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k % 8 == 0) {
					possibleMoves[i++]->pos = k;
					break;
				}
				possibleMoves[i++]->pos = k;
			}
		}
		//horizontal lookup leftward
		if (!((pos - 1) % 8 == 0)) {
			for (int k = pos - 1;; k--) {
				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
					}
				}

				if ((k - 1) % 8 == 0) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;

			}
		}

		//vertical lookup downward
		if (!(pos <= 8)) {
			for (int k = pos - 8;; k -= 8) {
				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k < 9) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;

			}
		}

		//vertical lookup upward
		if (!(pos > 7*8)) {
			for (int k = pos + 8;; k += 8) {
				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k > 7*8) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;

			}
		}

		break;
	case KNIGHT:
		// this is gonna be tricky
		//or it is just 8 manual cases
		//top two
		if (!(pos > 6 * 8)) {
			//top right
			if (!(pos % 8 == 0)) {
				if (fakeBoard[pos + 8 * 2 + 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos + 8 * 2 + 1;
				}
				else {
					if (fakeBoard[pos + 8 * 2 + 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos + 8 * 2 + 1;
				}
			}
			//top left
			if (!((pos - 1) % 8 == 0)) {
				if (fakeBoard[pos + 8 * 2 - 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos + 8 * 2 - 1;
				}
				else {
					if (fakeBoard[pos + 8 * 2 - 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos + 8 * 2 - 1;
				}
			}
		}
		// bottom two
		if (!(pos < 2 * 8 + 1)) {
			//bottom  right
			if (!(pos % 8 == 0)) {
				if (fakeBoard[pos - 8 * 2 + 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos - 8 * 2 + 1;
				}
				else {
					if (fakeBoard[pos - 8 * 2 + 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos - 8 * 2 + 1;
				}
			}
			//top left
			if (!((pos - 1) % 8 == 0)) {
				if (fakeBoard[pos - 8 * 2 - 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos - 8 * 2 - 1;
				}
				else {
					if (fakeBoard[pos - 8 * 2 - 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos - 8 * 2 - 1;
				}
			}
		}

		// far right (Hail Hitler lol)
		if (!((pos - 2) % 8 == 0)) {
			//upper
			if (!(pos > 7 * 8)) {
				if (fakeBoard[pos + 8 + 2 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos + 8 + 2;
				}
				else {
					if (fakeBoard[pos + 8 + 2 - 1]->color != turn)
						possibleMoves[i++]->pos = pos + 8 + 2;
				}
			}
			//lower
			if (!(pos < 8 + 1)) {
				if (fakeBoard[pos - 8 + 2 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos - 8 + 2;
				}
				else {
					if (fakeBoard[pos - 8 + 2 - 1]->color != turn)
						possibleMoves[i++]->pos = pos - 8 + 2;
				}
			}
		}

		//far left (Hail Stalin lol)
		if (!((pos + 2 - 1) % 8 == 0)) {
			//upper
			if (!(pos > 7 * 8)) {
				if (fakeBoard[pos + 8 - 2 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos + 8 - 2;
				}
				else {
					if (fakeBoard[pos + 8 - 2 - 1]->color != turn)
						possibleMoves[i++]->pos = pos + 8 - 2;
				}
			}
			//lower
			if (!(pos < 8 + 1)) {
				if (fakeBoard[pos - 8 - 2 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos - 8 - 2;
				}
				else {
					if (fakeBoard[pos - 8 - 2 - 1]->color != turn)
						possibleMoves[i++]->pos = pos - 8 - 2;
				}
			}
		};
		break;

	case BISHOP:
		// it might be simple
		// top right search
		if (!(pos > 7 * 8) && !(pos % 8 == 0)) {
			for (int k = pos + 9;; k += 9) {

				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k > 7 * 8 || (k % 8 == 0)) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;
			}
		}
		//top left
		if (!(pos > 7 * 8) && !((pos-1) % 8 == 0)) {
			for (int k = pos + 7;; k += 7) {

				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k > 7 * 8 || ((k-1) % 8 == 0)) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;
			}
		}
		//bottom left search
		if (!(pos < 8) && !((pos - 1) % 8 == 0)) {
			for (int k = pos - 9;; k -= 9) {

				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k < 8 || (pos - 1) % 8 == 0) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;
			}
		}
		// bottom right
		if (!(pos < 8) && !(pos % 8 == 0)) {
			for (int k = pos - 7;; k -= 7) {

				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k < 8 || pos % 8 == 0) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;
			}
		}


		break;
	case KING:
		
		//one move
		//front
		if (!(pos > 7 * 8)) {
			if (fakeBoard[pos + 8 - 1] == nullptr) {
				possibleMoves[i++]->pos = pos + 8;
			}
			else {
				if (fakeBoard[pos + 8 - 1]->color != turn)
					possibleMoves[i++]->pos = pos + 8;
			}
		}
		// right 3 moves
		if (!(pos % 8 == 0)) {
			//right
			if (fakeBoard[pos + 1 - 1] == nullptr) {
				possibleMoves[i++]->pos = pos + 1;
			}
			else {
				if (fakeBoard[pos + 1 - 1]->color != turn)
					possibleMoves[i++]->pos = pos + 1;
			}
			//top right
			if (!(pos > 7 * 8)) {
				if (fakeBoard[pos + 8 + 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos + 8 + 1;
				}
				else {
					if (fakeBoard[pos + 8 + 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos + 8 + 1;
				}
			}
			//bottom right
			if (!(pos < 8 + 1)) {
				if (fakeBoard[pos - 8 + 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos - 8 + 1;
				}
				else {
					if (fakeBoard[pos - 8 + 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos - 8 + 1;
				}
			}
		}
		// left three moves
		if (!((pos-1) % 8 == 0)) {
			//right
			if (fakeBoard[pos - 1 - 1] == nullptr) {
				possibleMoves[i++]->pos = pos - 1;
			}
			else {
				if (fakeBoard[pos  - 1 - 1]->color != turn)
					possibleMoves[i++]->pos = pos - 1;
			}
			//top right
			if (!(pos > 7 * 8)) {
				if (fakeBoard[pos + 8 - 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos + 8 - 1;
				}
				else {
					if (fakeBoard[pos + 8 - 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos + 8 - 1;
				}
			}
			//bottom right
			if (!(pos < 8 + 1)) {
				if (fakeBoard[pos - 8 - 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos - 8 - 1;
				}
				else {
					if (fakeBoard[pos - 8 - 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos - 8 - 1;
				}
			}
		}



		//down
		if (!(pos < 8 + 1)) {
			if (fakeBoard[pos - 8 - 1] == nullptr) {
				possibleMoves[i++]->pos = pos - 8;
			}
			else {
				if (fakeBoard[pos - 8 - 1]->color != turn)
					possibleMoves[i++]->pos = pos - 8;
			}
		}
	


		// castling code
		if ((turn == WHITE && g.White.KingMoved == false)
			|| turn == BLACK && g.Black.KingMoved == false) {

		}

		break;

	case QUEEN:

		// copy paste rook bishop king(one move)
		// bishop
		// it might be simple
		// top right search
		if (!(pos > 7 * 8) && !(pos % 8 == 0)) {
			for (int k = pos + 9;; k += 9) {

				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k > 7 * 8 || (k % 8 == 0)) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;
			}
		}
		//top left
		if (!(pos > 7 * 8) && !((pos - 1) % 8 == 0)) {
			for (int k = pos + 7;; k += 7) {

				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k > 7 * 8 || ((k - 1) % 8 == 0)) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;
			}
		}
		//bottom left search
		if (!(pos < 8) && !((pos - 1) % 8 == 0)) {
			for (int k = pos - 9;; k -= 9) {

				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k < 8 || (pos - 1) % 8 == 0) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;
			}
		}
		// bottom right
		if (!(pos < 8) && !(pos % 8 == 0)) {
			for (int k = pos - 7;; k -= 7) {

				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k < 8 || pos % 8 == 0) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;
			}
		}
		// --
		// rook
		// horizontal lookup right
		if (!(pos % 8 == 0)) {
			for (int k = pos + 1;; k++) {
				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k % 8 == 0) {
					possibleMoves[i++]->pos = k;
					break;
				}
				possibleMoves[i++]->pos = k;
			}
		}
		//horizontal lookup leftward
		if (!((pos - 1) % 8 == 0)) {
			for (int k = pos - 1;; k--) {
				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
					}
				}

				if ((k - 1) % 8 == 0) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;

			}
		}

		//vertical lookup downward
		if (!(pos <= 8)) {
			for (int k = pos - 8;; k -= 8) {
				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k < 9) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;

			}
		}

		//vertical lookup upward
		if (!(pos > 7 * 8)) {
			for (int k = pos + 8;; k += 8) {
				if (fakeBoard[k - 1] != nullptr) {
					if (fakeBoard[k - 1]->color == fakeBoard[selectedPiece - 1]->color) {
						break;
					}
					else {
						possibleMoves[i++]->pos = k;
						break;
					}
				}

				if (k > 7 * 8) {
					possibleMoves[i++]->pos = k;
					break;
				}

				possibleMoves[i++]->pos = k;

			}
		}

		//--
		//one move
		//front
		if (!(pos > 7 * 8)) {
			if (fakeBoard[pos + 8 - 1] == nullptr) {
				possibleMoves[i++]->pos = pos + 8;
			}
			else {
				if (fakeBoard[pos + 8 - 1]->color != turn)
					possibleMoves[i++]->pos = pos + 8;
			}
		}
		// right 3 moves
		if (!(pos % 8 == 0)) {
			//right
			if (fakeBoard[pos + 1 - 1] == nullptr) {
				possibleMoves[i++]->pos = pos + 1;
			}
			else {
				if (fakeBoard[pos + 1 - 1]->color != turn)
					possibleMoves[i++]->pos = pos + 1;
			}
			//top right
			if (!(pos > 7 * 8)) {
				if (fakeBoard[pos + 8 + 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos + 8 + 1;
				}
				else {
					if (fakeBoard[pos + 8 + 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos + 8 + 1;
				}
			}
			//bottom right
			if (!(pos < 8 + 1)) {
				if (fakeBoard[pos - 8 + 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos - 8 + 1;
				}
				else {
					if (fakeBoard[pos - 8 + 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos - 8 + 1;
				}
			}
		}
		// left three moves
		if (!((pos - 1) % 8 == 0)) {
			//right
			if (fakeBoard[pos - 1 - 1] == nullptr) {
				possibleMoves[i++]->pos = pos - 1;
			}
			else {
				if (fakeBoard[pos - 1 - 1]->color != turn)
					possibleMoves[i++]->pos = pos - 1;
			}
			//top right
			if (!(pos > 7 * 8)) {
				if (fakeBoard[pos + 8 - 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos + 8 - 1;
				}
				else {
					if (fakeBoard[pos + 8 - 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos + 8 - 1;
				}
			}
			//bottom right
			if (!(pos < 8 + 1)) {
				if (fakeBoard[pos - 8 - 1 - 1] == nullptr) {
					possibleMoves[i++]->pos = pos - 8 - 1;
				}
				else {
					if (fakeBoard[pos - 8 - 1 - 1]->color != turn)
						possibleMoves[i++]->pos = pos - 8 - 1;
				}
			}
		}



		//down
		if (!(pos < 8 + 1)) {
			if (fakeBoard[pos - 8 - 1] == nullptr) {
				possibleMoves[i++]->pos = pos - 8;
			}
			else {
				if (fakeBoard[pos - 8 - 1]->color != turn)
					possibleMoves[i++]->pos = pos - 8;
			}
		}

		
	}
	*x = i;
}


void Engine::makeMove(Piece* p[], int from, int to, int FLAG , int *king_pos) {

	switch (FLAG) {
	case NORMAL:{
			
			p[to - 1] = p[from - 1];
			p[from - 1] = nullptr;
			if (p[to - 1]->value == PAWN)
				p[to - 1]->FLAG = NORMAL;
			break;
		}
	case CAPTURE: {
		Piece* temp = p[to - 1];
		p[to - 1] = p[from - 1];
		p[from - 1] = nullptr;
		delete temp;
		if (p[to - 1]->value == PAWN)
			p[to - 1]->FLAG = NORMAL;
		break;
	}
	case ENPASSANT:
		break;
	case CASTLE:
		break;
	}
	if (FLAG != ENPASSANT)
		if (p[to - 1]->value == KING)
			*king_pos = to;

}


void Engine::processEvent(int pos){

	if (pos >= OUTOFINDEX)
		return;
	if (g.selectedPiece == OUTOFINDEX) {

		if (g.board[pos-1] != nullptr ) {
			if (g.board[pos-1]->color == g.turn) {
				g.selectedPiece = pos;
				calculateLegalMoves();
			}
		}

	}
	else if (g.selectedPiece == pos) {
		return;
	}
	// make a move
	else {
		bool valid = false;
		for (int i = 0; i < MAX; i++) {
			if (g.possibleMoves[i] != nullptr) {
				if (g.possibleMoves[i]->pos == pos) {
					valid = true;
				}
			}
		}

		if (valid) {

			int* Kingpos = &((g.turn == WHITE) ? g.White.KingPos : g.Black.KingPos);
			
			makeMove(g.board, g.selectedPiece, pos, g.possibleMoves[pos]->FLAG, Kingpos);
			g.prevMove = pos;
			g.turn = !(g.turn);
			g.selectedPiece = OUTOFINDEX;

		}
		else {
			g.selectedPiece = OUTOFINDEX;
		}
	}
		

}




#ifdef _DEBUG
void Game::printPosConsole() {
	int j = 1;
	for (int i = 63; i >= 0; i--) {
		Piece* p = board[i];
		if (board[i] == nullptr) {
			std::cout << " , ";
		}
		else {
			std::cout << " " << code[board[i]->value + board[i]->color * 14 - 1] << ",";
		}
		if ((j++) % 8 == 0)
			std::cout << std::endl;
	}
}

pair Utils::map(int pos, int dim, bool boardFlipped) {

	if (boardFlipped) {
		pos = 65 - pos;
	}

	// map pos to coordinates to draw
	// offset of drawing :origin at  top right corner +x = right +y = down
	// convert 1d into 2d
	int x = (pos % 8 == 0) ? 8 : pos % 8;
	int y = (pos %8 ==0)? pos/8 : (pos/8 + 1);

	pair p;
	p.x = x;
	p.y = y;

	return p;

};

pair Utils::postoXandY(pair pos, int dim) {
	pair p;
	p.x = pos.x;
	p.y = pos.y;
	p.x = p.x - 1;

	p.y = abs(8 - p.y);
	p.x = p.x * dim;
	p.y = dim * p.y;

	return p;
}

int Utils::cordToPos(pair cord, int dim) {
	int x = cord.x / dim + 1;
	int y = ((dim * 8) - cord.y) / dim + 1;
	return (y - 1) * 8 + x;
}

void Engine::Test() {
	selectPiece(int(19));
}



#endif