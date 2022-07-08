#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Renderer.h"
#include <string>



enum TEXTURES {
    W_PAWN = 0, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING, B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING, COLOR1, COLOR2
};


Renderer::Renderer(int width , Piece** board, bool flipped) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return;
    }
   
    m_width = width;
    m_board = board;
    m_flipped = flipped;
     m_window = SDL_CreateWindow("SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width,m_width ,
        0);

     if (!m_window) {
         std::cout << "Failed to create Window";
     }



     m_surface = SDL_GetWindowSurface(m_window);
     if (!m_surface) {
         std::cout << "Failed to get surface";
     }


     box_width = width / 8;

     loadAllTextures();
     m_background = createBackground();
     renderBack();
     SDL_UpdateWindowSurface(m_window);
}


void Renderer::loadAllTextures() {


    std::string start = "img\\";
    std::string end = ".png";
    std::string filename;

    for (int i = 0; i < 14; i++) {
        filename = start + std::to_string(i) + end;
        std::string p = SDL_GetBasePath();

        m_textures[i] = loadImage(p + filename);

    }

}


void Renderer::renderBack() {

    r.x = 0;
    r.y = 0;
    r.h = m_width;
    r.w = m_width;


    SDL_BlitSurface(m_background, NULL, m_surface, &r);


}   

// create a background surface

SDL_Surface *Renderer::createBackground() {
    SDL_Surface* back = SDL_CreateRGBSurfaceWithFormat(0, m_width, m_width, 8, SDL_PIXELFORMAT_RGBA8888);
    SDL_Rect rect;
    rect.w = box_width;
    rect.h = box_width;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            rect.x = i*box_width;
            rect.y = j* box_width;
            if ((i+j) % 2 == 0) {
                SDL_BlitScaled(m_textures[COLOR1], NULL, back, &rect);
            }
            else {
                SDL_BlitScaled(m_textures[COLOR2], NULL, back, &rect);
            }
            
        }
    }
    return back;
}

//though SDL_Texture is recommended cause it stores on vram but i don't want to bother with it
SDL_Surface* Renderer::loadImage(std::string filename)
{
    // copying from stack overflow
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    

    loadedImage = IMG_Load(filename.c_str());
    if (!loadedImage) {
        std::cout << "failed to load texture at " << filename;
        std::cout << SDL_GetError();
    }

    //If the image loaded
    if (loadedImage != NULL)
    {
        //Create an optimized image
  //cout<<"Flag";
     
        optimizedImage = SDL_ConvertSurfaceFormat(loadedImage,SDL_PIXELFORMAT_RGBA8888,0);

        //Free the old image
        SDL_FreeSurface(loadedImage);
    }

    //Return the optimized image
    return optimizedImage;
}


void Renderer::renderBoard() {
    SDL_Rect p_rect;
    p_rect.h = box_width;
    p_rect.w = box_width;
    for (int i = 0; i < 64; i++) {
        if ((m_board)[i] == nullptr) {
            
        }
        else {
            pair p = Utils::postoXandY(Utils::map(i+1, box_width, m_flipped), box_width);
            p_rect.x = p.x;
            p_rect.y = p.y;
            int color = m_board[i]->color;
            int value = m_board[i]->value;
            int texture_index = getTextureIndex(value, color);
            SDL_BlitScaled(m_textures[texture_index], NULL, m_surface, &p_rect);
            
        }
    }
}

void Renderer::redraw() {
    renderBack();
    renderBoard();
}

void Renderer::update() {
    SDL_UpdateWindowSurface(m_window);
}

//cleanup

Renderer::~Renderer() {
    SDL_FreeSurface(m_surface);
    SDL_DestroyWindow(m_window);
}


int  Renderer::getTextureIndex(int value, int color) {
    int index;
    if (color == 0) { // white
        index = value - 1;
    }
    else {
        index = value + 6 -1;
    }
    return index;
}




