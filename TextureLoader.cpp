#include "TextureLoader.h"
#include "includes.h"
#ifdef _WIN32
#include "glextwin32.h"
#else
#include <OpenGL/glext.h>
#endif
#include "SDL2/SDL_image.h"

TextureLoader::TextureLoader(){}
TextureLoader::~TextureLoader(){}

TextureLoader* TextureLoader::_instance = 0 ;

TextureLoader* TextureLoader::instance ()
{
	if (_instance == 0)  // is it the first call?
	{

	  _instance = new TextureLoader(); // create sole instance
	}

	return _instance; // address of sole instance
}

unsigned int TextureLoader::load(const char *filename)
{
	string stringFilename = string(filename);



	GLuint image = cache[stringFilename];

	if (image != 0)
		return image;


	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;

	if ( (surface = IMG_Load(filename)) ) {

		// Check that the image's width is a power of 2
		if ( (surface->w & (surface->w - 1)) != 0 ) {
			printf("warning: %s's width is not a power of 2\n",filename);
		}

		// Also check if the height is a power of 2
		if ( (surface->h & (surface->h - 1)) != 0 ) {
			printf("warning: image.bmp's height is not a power of 2\n");
		}

        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
        } else {
			printf("warning: the image is not truecolor..  this will probably break\n");
			// this error should not go unhandled
        }

		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &image );

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, image );

		// Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
					 texture_format, GL_UNSIGNED_BYTE, surface->pixels );

		//printf("SDL loaded image: '%s' .\n", filename);
		cache[stringFilename] = image;
	}
	else {
		printf("'%s' .\n", SDL_GetError());
		return 0;
	}

	// Free the SDL_Surface only if it was successfully created
	if ( surface )
		SDL_FreeSurface( surface );



	return image;

}