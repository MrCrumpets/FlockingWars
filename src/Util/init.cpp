#include "init.h"
#include "SFML/Graphics.hpp"

//GLuint loadTexture(std::string filename) {
//	//Temporary storage for the image that's loaded
//	SDL_Surface* surface = NULL;
//	GLuint texture;
//	//Load the image
//	surface = IMG_Load(filename.c_str());
//	if(!surface){
//		printf("bad image\n");
//		exit(1);
//	}
//	SDL_LockSurface(surface);
//	glEnable(GL_TEXTURE_2D);
//	glGenTextures(1, &texture);
//	glBindTexture( GL_TEXTURE_2D, texture );
//	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
//			GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
//	if ( surface ) {
//		SDL_FreeSurface( surface );
//	}
//	//Return the optimized image
//	return texture;
//}

/**
 * The init_GL method initializes the OpenGL pipeline, sets up the camera and enables certain functions.
 *
 * @return
 * 		bool flag stating whether the init_GL succeeded with errors or not.
 */

bool init_GL(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
	//Set clear color
	glClearColor(0, 0, 0, 0);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//If there was any errors
        GLenum err;
	if ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
            return false;
	}
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//If everything initialized
	return true;
}

int nextPowerOfTwo(int n){
	n--;
	n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
	n |= n >> 2;   // and then or the results.
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;
	return n;
}
