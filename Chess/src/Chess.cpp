#include <iostream>
#include <SDL.h>

#include <iostream>
#include "Engine/Engine.h"
#include "UI/Renderer.h"

const int WIDTH = 640;
const int BOX = WIDTH / 8;




int main(int argc, char* argv[])
{
    Engine* engine = new Engine;
    Renderer *renderer = new Renderer(WIDTH,engine->getBoard(),false);
   
    renderer->renderBoard();
    renderer->update();
    bool open = true;
    //initialise renderer

    SDL_Event current_event;
        while(open){
            while (SDL_WaitEvent(&current_event) > 0) {
                switch (current_event.type) {
                case SDL_QUIT:
                    open = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    pair cord;
                    SDL_GetMouseState(&cord.x, &cord.y);
                    int pos = Utils::cordToPos(cord, BOX);
                    engine->processEvent(pos);
                    renderer->redraw();
                    renderer->update();

                    
                    
                }
            }

    }
    return 0;
}