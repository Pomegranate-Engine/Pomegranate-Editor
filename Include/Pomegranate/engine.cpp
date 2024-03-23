#include"engine.h"
namespace Pomegranate
{
    float delta_time = 0.0f;

    int pomegranate_init(bool input, bool video, bool audio, bool img, bool ttf)
    {
        auto* compiled = (SDL_version*)malloc(sizeof(SDL_version));
        SDL_GetVersion(compiled);
        print_info(std::to_string(compiled->major) + "." + std::to_string(compiled->minor) + "." + std::to_string(compiled->patch));
        if(audio)
        {
            if (SDL_Init(SDL_INIT_AUDIO) < 0)
            {
                print_error("Failed to initialize the SDL2 audio");
            }
            if(Mix_Init(MIX_INIT_WAVPACK)<0)
            {
                print_error("Failed to initialize the SDL2 mixer");
            }
            //Debug audio devices to console
            if(Mix_OpenAudio(0,nullptr)<0)
            {
                print_error("Failed to initialize the SDL2 mixer");
            }
        }
        if(video)
        {
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                print_error("Failed to initialize the SDL2 library");
            }
        }
        if(input)
        {
            InputManager::init();
        }
        if(img)
        {
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                print_error("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
            }
        }
        if(ttf)
        {
            if(TTF_Init() == -1)
            {
                print_error("SDL_ttf could not initialize! SDL_ttf Error: " + std::string(TTF_GetError()));
            }
        }
        return 0;
    }

    void pomegranate_quit()
    {
        SDL_Quit();
        Mix_Quit();
        IMG_Quit();
        TTF_Quit();
    }
}