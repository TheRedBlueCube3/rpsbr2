#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>

#include "defines.h"
#include "Icon.h"

int main(int argc, char **argv)
{
    int opt;
    int iconCount = 100;
    int winWidth = 640;
    int winHeight = 480;
    BOOL infection = FALSE;

    while((opt = getopt(argc, argv, "ic:w:h:")) != -1)
    {
        switch(opt)
        {
            case 'c':
            {
                if (optarg == NULL) {
                    exit(1);
                }
                char* endptr;
                iconCount = strtol(optarg, &endptr, 10);
                if(*endptr != '\0')
                {
                    exit(1);
                }
                break;
            }
            case 'w':
            {
                if (optarg == NULL) {
                    exit(1);
                }
                char* endptr;
                winWidth = strtol(optarg, &endptr, 10);
                if(*endptr != '\0')
                {
                    exit(1);
                }
                break;
            }
            case 'h':
            {
                if (optarg == NULL) {
                    exit(1);
                }
                char* endptr;
                winHeight = strtol(optarg, &endptr, 10);
                if(*endptr != '\0')
                {
                    exit(1);
                }
                break;
            }
            case 'i':
            {
                infection = TRUE;
                break;
            }
            case '?':
                exit(1);
        }
    }

    srand(time(NULL));
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Initialized SDL3...\n");
    }
    else
    {
        fprintf(stderr, "Failed to initialize SDL3!!! Error: %s\n", SDL_GetError());
        return 1;
    }

    Icon icons[iconCount];

    for(int i = 0; i < iconCount; i++)
    {
        // choose a random location on the screen
        int x = rand() % winWidth;
        int y = rand() % winHeight;
        
        // choose a random alignment
        int alignment = rand() % 3;
        icons[i].m_vecPosition = (Vector) {x, y};
        icons[i].m_iAlignment = alignment;
        icons[i].m_vecAcceleration = g_vecZero;
        icons[i].m_vecVelocity = g_vecZero;
    }

    SDL_Window *window = SDL_CreateWindow("rock paper scissors!", winWidth, winHeight, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (!SDL_SetRenderVSync(renderer, 1)) {
        printf("Warning: VSync could not be enabled! Error: %s\n", SDL_GetError());
    }

    SDL_Texture *rock = IMG_LoadTexture(renderer, "res/icons/rock.png");
    if(!rock) {
        fprintf(stderr, "Failed to load rock.png: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *paper = IMG_LoadTexture(renderer, "res/icons/paper.png");
    if(!paper) {
        fprintf(stderr, "Failed to load paper.png: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *scissors = IMG_LoadTexture(renderer, "res/icons/scissors.png");
    if(!scissors) {
        fprintf(stderr, "Failed to load scissors.png: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Event e;
    BOOL running = TRUE;

    float lastFrameTime = SDL_GetTicks();

    while(running)
    {
        float curTime = SDL_GetTicks();
        float dt = (curTime - lastFrameTime) / 1000.0f;
                lastFrameTime = curTime;
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderClear(renderer);

        // all of the icons have the same src dimensions
        SDL_FRect src = {0, 0., rock->w, rock->h};
        
        for(int i = 0; i < iconCount; i++)
        {
            if(icons[i].m_bDead) continue;
            ICO_Update(&icons[i], dt, winWidth, winHeight, icons, iconCount, infection);

            SDL_FRect dst = {icons[i].m_vecPosition.x - 16, icons[i].m_vecPosition.y - 16, winWidth/20.0f, winWidth/20.0f};
            SDL_Texture* requiredTexture;
            switch(icons[i].m_iAlignment)
            {
                case ICO_ROCK:
                    requiredTexture = rock;
                    break;
                case ICO_PAPER:
                    requiredTexture = paper;
                    break;
                case ICO_SCISSORS:
                    requiredTexture = scissors;
                    break;
            }
            SDL_RenderTexture(renderer, requiredTexture, &src, &dst);
        }
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}