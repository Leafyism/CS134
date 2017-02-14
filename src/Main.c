#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<GL/glew.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include"DrawUtils.h"
#include<dos.h>

// Set this to true to make the game loop exit.
char shouldExit = 0;

// The previous frame's keyboard state.
unsigned char kbPrevState[SDL_NUM_SCANCODES] = {0};

// The current frame's keyboard state.
const unsigned char* kbState = NULL;

// Position of the sprite.
int spritePos[2] = {150, 50};
int sprite2Pos[2] = { 10, 10 };
// Texture for the sprite.
GLuint spriteTex;
GLuint spriteTex2;
GLuint spriteTex3;

// Size of the sprite.
int spriteSize[2];
int spriteSize2[2];

void move(int speed)
{

	if (kbState[SDL_SCANCODE_LEFT])
	{
		printf("left");
		//move sprite left
		if (spritePos[0] > 3)
		{
			spritePos[0] -= speed * 2;
			sprite2Pos[0] -= speed;
			printf("%i %i", spritePos[0], speed);
		}
	}
	else if (kbState[SDL_SCANCODE_RIGHT])
	{
		printf("right");
		//move sprite right
		if (spritePos[0] < 730)
		{
			spritePos[0] += speed * 2;
			sprite2Pos[0] += speed;
			printf("%i %i", spritePos[0], speed);
		}
	}
	else if (kbState[SDL_SCANCODE_UP])
	{
		printf("up");
		//move sprite up
		if (spritePos[1] > 3)
		{
			spritePos[1] -= speed * 2;
			sprite2Pos[1] -= speed;
		}
	}
	else if (kbState[SDL_SCANCODE_DOWN])
	{
		printf("down");
		//move sprite down
		if (spritePos[1] < 525)
		{
			spritePos[1] += speed * 2;
			sprite2Pos[1] += speed;
		}
	}
	else
	{
		//chase algorithm
		if ((sprite2Pos[0] < spritePos[0]) && (sprite2Pos[1] < spritePos[1]))
		{
			if (sprite2Pos[0] < 730 && sprite2Pos[1] < 525)
			{
				sprite2Pos[0] += speed;
				sprite2Pos[1] += speed;
			}
		}
		else if ((sprite2Pos[0] > spritePos[0]) && (sprite2Pos[1] < spritePos[1]))
		{
			if (sprite2Pos[0] < 730 && sprite2Pos[1] < 525)
			{
				sprite2Pos[0] -= speed;
				sprite2Pos[1] += speed;
			}
		}
		else if ((sprite2Pos[0] < spritePos[0]) && (sprite2Pos[1] > spritePos[1]))
		{
			if (sprite2Pos[0] < 730 && sprite2Pos[1] < 525)
			{
				sprite2Pos[0] += speed;
				sprite2Pos[1] -= speed;
			}
		}
		else if ((sprite2Pos[0] > spritePos[0]) && (sprite2Pos[1] > spritePos[1]))
		{
			if (sprite2Pos[0] < 730 && sprite2Pos[1] < 525)
			{
				sprite2Pos[0] -= speed;
				sprite2Pos[1] -= speed;
			}
		}
		else if ((sprite2Pos[0] > spritePos[0]) && (sprite2Pos[1] == spritePos[1]))
		{
			if (sprite2Pos[0] < 730 && sprite2Pos[1] < 525)
			{
				sprite2Pos[0] -= speed;
			}
		}
		else if ((sprite2Pos[0] < spritePos[0]) && (sprite2Pos[1] == spritePos[1]))
		{
			if (sprite2Pos[0] < 730 && sprite2Pos[1] < 525)
			{
				sprite2Pos[0] += speed;
			}
		}
		else if ((sprite2Pos[0] == spritePos[0]) && (sprite2Pos[1] > spritePos[1]))
		{
			if (sprite2Pos[0] < 730 && sprite2Pos[1] < 525)
			{
				sprite2Pos[1] -= speed;
			}
		}
		else if ((sprite2Pos[0] == spritePos[0]) && (sprite2Pos[1] < spritePos[1]))
		{
			if (sprite2Pos[0] < 730 && sprite2Pos[1] < 525)
			{
				sprite2Pos[1] += speed;
			}
		}
	}
}

int main(void)
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
        return 1;
    }

    // Create the window and OpenGL context.
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window* window = SDL_CreateWindow(
            "SDLTemplate",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            800, 600,
            SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_GL_CreateContext(window);

    // Make sure we have a recent version of OpenGL.
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
        SDL_Quit();
        return 1;
    }
    if (GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 or greater supported: Version=%s\n",
                 glGetString(GL_VERSION));
    } else {
        fprintf(stderr, "OpenGL max supported version is too low.\n");
        SDL_Quit();
        return 1;
    }

    // Setup OpenGL state.
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, 800, 600, 0, 0, 100);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Game initialization goes here.
    spriteTex = glTexImageTGAFile("magikarp.tga", &spriteSize[0], &spriteSize[1]);
	spriteTex2 = glTexImageTGAFile("mario.fw.tga", &spriteSize[0], &spriteSize[1]);
	spriteTex3 = glTexImageTGAFile("32191.tga", &spriteSize2[0], &spriteSize2[1]);

    // The game loop.
	kbState = SDL_GetKeyboardState(NULL);

	int speed = 0;
	Uint32 lastTime = 0;
	
	while (!shouldExit) {
        assert(glGetError() == GL_NO_ERROR);
        memcpy(kbPrevState, kbState, sizeof(kbPrevState));
        
        // Handle OS message pump.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    shouldExit = 1;
            }
        }

        // Game logic goes here.
		
        if (kbState[SDL_SCANCODE_ESCAPE]) {
            shouldExit = 1;
        }

		Uint32 currentTime = SDL_GetTicks();
		speed = (int)(currentTime - lastTime) / 10;

		printf("%i", speed);
		move(speed);
		lastTime = currentTime;

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

		// Game drawing goes here.
		glDrawSprite(spriteTex, spritePos[0], spritePos[1], spriteSize[0], spriteSize[1]);
		glDrawSprite(spriteTex2, sprite2Pos[0], sprite2Pos[1], spriteSize[0], spriteSize[1]);
		glDrawSprite(spriteTex3, 650, 500, spriteSize2[0], spriteSize2[1]);
		/*
		
		*/
        // Present the most recent frame.
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();

    return 0;
}
