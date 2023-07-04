#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include "includes.h"
#include "Engine.h"
#include "main.h"
#include "GL\freeglut.h"
#include "glext.h"
#include "Config.h"

#define CLEAN_COLOR 0.2f,0.2f,0.28f
GLfloat fogColor[4] = {CLEAN_COLOR, 1.0};
//#define CLEAN_COLOR 0.64f,0.76f,0.85f

Engine* engine;

 void initSDL ()
{
	printf("Initiating SDL......");


	/* This is our SDL surface */
	static SDL_Window *window;
	static SDL_Renderer* renderer;

	/* initialize SDL */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );

	}

	int videoFlags;
    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_WINDOW_OPENGL;          /* Enable OpenGL in SDL */
    //videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    //videoFlags |= SDL_WINDOW_HWPALETTE;       /* Store the palette in hardware */
    //videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

	if (engine->renderFullScreen)
		videoFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    /* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	if (Config::antiAliasingMSAA != 1)
	{
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);		// ATI compliance requiere 24 bits, can be pushed to 32 with Nvidia
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
	}

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    /* get a SDL surface */
	window = SDL_CreateWindow(
		"Zen-Frag",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		engine->renderWidth,
		engine->renderHeight,
		videoFlags);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_RendererInfo videoInfo;

	SDL_GetRendererInfo(renderer, &videoInfo);

	//SDL_EnableKeyRepeat(20,20);

	/* Verify there is a surface */
    if ( !window)
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	}

	engine->window = window;

	SDL_SetRelativeMouseMode(SDL_TRUE);
	printf("OK.\n");
}

void initGL ()
{

	printf("Initiating openGL...");
	//Set fog
	/*
	glFogi(GL_FOG_MODE, GL_LINEAR);		// Fog Mode
	glFogfv (GL_FOG_COLOR,fogColor);			// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.20f);				// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_FASTEST);			// Fog Hint Value
	glFogf(GL_FOG_START, 7000.0f);				// Fog Start Depth
	glFogf(GL_FOG_END, 10000.0f);				// Fog End Depth
	glEnable(GL_FOG);
	*/
	//End fog
	glEnable(GL_CULL_FACE);

	if (Config::antiAliasingMSAA != 1)
		glEnable(GL_MULTISAMPLE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    printf("OK.\n");
}



int main(int argc, char* argv[])
//int main()
{
	printf("Starting engine... \n");

	SDL_SetMainReady();

	engine = new Engine();
	engine->readConfiguration();

	initSDL();
	initGL();

	engine->run();


	printf("Ending engine.\n");
	return 0;
}
