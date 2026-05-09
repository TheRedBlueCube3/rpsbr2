#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
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

    if(!TTF_Init())
    {
        fprintf(stderr, "Failed to initialize SDL3_ttf!!! Error: %s\n", SDL_GetError());
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
        icons[i].m_bDead = FALSE;
    }

    SDL_Window *window = SDL_CreateWindow("rock paper scissors!", winWidth, winHeight, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (!SDL_SetRenderVSync(renderer, 1)) {
        printf("Warning: VSync could not be enabled! Error: %s\n", SDL_GetError());
    }

    SDL_Texture *rock = IMG_LoadTexture(renderer, "res/icons/rock.png");
    if(!rock)
    {
        fprintf(stderr, "Failed to load rock.png: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *paper = IMG_LoadTexture(renderer, "res/icons/paper.png");
    if(!paper)
    {
        fprintf(stderr, "Failed to load paper.png: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *scissors = IMG_LoadTexture(renderer, "res/icons/scissors.png");
    if(!scissors)
    {
        fprintf(stderr, "Failed to load scissors.png: %s\n", SDL_GetError());
        return 1;
    }
    TTF_Font *font = TTF_OpenFont("res/fonts/font.ttf", 24);
    if(!font)
    {
        fprintf(stderr, "Failed to load res/fonts/font.ttf! Error: %s\n", SDL_GetError());
    }

    TTF_TextEngine *engine = TTF_CreateRendererTextEngine(renderer);

    TTF_Text *rockText = TTF_CreateText(engine, font, "Rocks: 0", 0);
    TTF_Text *paperText = TTF_CreateText(engine, font, "Papers: 0", 0);
    TTF_Text *scissorsText = TTF_CreateText(engine, font, "Scissors: 0", 0);
    TTF_SetTextColor(rockText, 0, 0, 0, 255);
    TTF_SetTextColor(paperText, 0, 0, 0, 255);
    TTF_SetTextColor(scissorsText, 0, 0, 0, 255);

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
                    running = FALSE;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // all of the icons have the same src dimensions
        SDL_FRect src = {0, 0., rock->w, rock->h};

        int rockCount = 0;
        int paperCount = 0;
        int scissorsCount = 0;
        
        for(int i = 0; i < iconCount; i++)
        {
            if(icons[i].m_bDead) continue;
            ICO_Update(&icons[i], dt, winWidth, winHeight, icons, iconCount, infection);

            SDL_FRect dst = {icons[i].m_vecPosition.x - 16, icons[i].m_vecPosition.y - 16, winWidth/20.0f, winWidth/20.0f};
            SDL_Texture* requiredTexture;
            switch(icons[i].m_iAlignment)
            {
                case ICO_ROCK:
                    rockCount++;
                    requiredTexture = rock;
                    break;
                case ICO_PAPER:
                    paperCount++;
                    requiredTexture = paper;
                    break;
                case ICO_SCISSORS:
                    scissorsCount++;
                    requiredTexture = scissors;
                    break;
            }
            SDL_RenderTexture(renderer, requiredTexture, &src, &dst);
        }

        char rockBuffer[64];
        char paperBuffer[64];
        char scissorsBuffer[64];

        snprintf(rockBuffer, 64, "Rocks: %d", rockCount);
        snprintf(paperBuffer, 64, "Papers: %d", paperCount);
        snprintf(scissorsBuffer, 64, "Scissors: %d", scissorsCount);

        TTF_SetTextString(rockText, rockBuffer, 0);
        TTF_SetTextString(paperText, paperBuffer, 0);
        TTF_SetTextString(scissorsText, scissorsBuffer, 0);

        TTF_DrawRendererText(rockText, 0, 0);
        TTF_DrawRendererText(paperText, 0, 25);
        TTF_DrawRendererText(scissorsText, 0, 50);
        
        SDL_RenderPresent(renderer);
    }

    TTF_DestroyText(rockText);
    TTF_DestroyText(paperText);
    TTF_DestroyText(scissorsText);
    TTF_DestroyRendererTextEngine(engine);
    TTF_CloseFont(font);
    SDL_DestroyTexture(rock);
    SDL_DestroyTexture(paper);
    SDL_DestroyTexture(scissors);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}