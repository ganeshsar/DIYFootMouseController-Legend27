#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[]) 
{

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    }
    std::cout << "passed" << std::endl;

    while (true) {

    }

    return 0;
}

// Intro to sdl https://lazyfoo.net/tutorials/SDL/19_gamepads_and_joysticks/index.php