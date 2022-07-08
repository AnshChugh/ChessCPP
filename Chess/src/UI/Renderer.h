#pragma once
#include <SDL.h>
#include "Engine/Engine.h"
class Renderer {
private:
	int m_width;
	SDL_Window* m_window;
	SDL_Surface* m_surface;
	SDL_Surface* m_background;
	SDL_Surface* createBackground();
	SDL_Surface* loadImage(std::string filename);
	SDL_Surface* m_textures[14];
	SDL_Renderer* m_renderer;
	int box_width;
	SDL_Rect r;
	Piece** m_board;
	bool m_flipped;
	void loadAllTextures();
	int getTextureIndex(int, int);
public:

	Renderer(int, Piece**,bool);
	void renderBack();
	void renderBoard();
	void update();
	void redraw();
	void renderPossibleMoves();
	void unrenderPossibleMoves();
	~Renderer();


};