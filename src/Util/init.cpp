#include "init.h"


#include "SDL/SDL_image.h"
#include "text.h"

GLuint loadTexture(std::string filename) {
	//Temporary storage for the image that's loaded
	SDL_Surface* surface = NULL;
	GLuint texture;
	//Load the image
	surface = IMG_Load(filename.c_str());
	if(!surface){
		printf("bad image\n");
		exit(1);
	}
	SDL_LockSurface(surface);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
	if ( surface ) {
		SDL_FreeSurface( surface );
	}
	//Return the optimized image
	return texture;
}

/**
 * The init_GL method initializes the OpenGL pipeline, sets up the camera and enables certain functions.
 *
 * @return
 * 		bool flag stating whether the init_GL succeeded with errors or not.
 */
bool init_GL(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
	//Set clear color
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	float aspectRatio = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
	gluPerspective(45.0, aspectRatio, 0.001, 5000.0);
	//Initialize modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//If there was any errors
	if (glGetError() != GL_NO_ERROR) {
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

void generateGlyphs(TTF_Font* mainFont, int numChars, Glyph* glyphs){
	unsigned int Rmask, Gmask, Bmask, Amask;
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			Rmask = 0xff000000;
			Gmask = 0x00ff0000;
			Bmask = 0x0000ff00;
			Amask = 0x000000ff;
		#else
			Rmask = 0x000000ff;
			Gmask = 0x0000ff00;
			Bmask = 0x00ff0000;
			Amask = 0xff000000;
		#endif

	//Generate Number Glyphs
	cout << "Init: text " << mainFont << endl;
	for(int i = 0; i < numChars; i++){
		SDL_Color color = {255, 255, 255};
		SDL_Surface *temp = TTF_RenderGlyph_Blended(mainFont, i+32, color);
		SDL_SetAlpha(temp, 0, 0);
		SDL_Surface *text =  SDL_CreateRGBSurface(0, nextPowerOfTwo(temp->w), nextPowerOfTwo(temp->h), 32, Rmask, Gmask, Bmask, Amask);
		SDL_BlitSurface(temp, 0, text, 0);
		GLuint gTex;
		GLuint dl;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &gTex);
		glBindTexture(GL_TEXTURE_2D, gTex);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, text->w, text->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, text->pixels );
		dl = glGenLists(1);
		float x = 0, y = 0, z = 0, dy = text->h-temp->h;
		if((char)(i+32)=='q' || (char)(i+32)=='p'|| (char)(i+32)=='g' || (char)(i+32)=='j')
			dy += 12;
		float w = text->w, h = text->h;
		glNewList(dl, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, gTex );
		glBegin(GL_QUADS);
		glColor4ub(255, 255, 255, 255);
		glTexCoord2d(0, 1); glVertex3d(x, y-dy, z);
		glTexCoord2d(1, 1); glVertex3d(x+w, y-dy, z);
		glTexCoord2d(1, 0); glVertex3d(x+w, y+h-dy, z);
		glTexCoord2d(0, 0); glVertex3d(x, y+h-dy, z);
		glEnd();
		glEndList();
		float s = (34-temp->w)/2;
		Glyph l = {temp->w, h, s, s, 40, dl};
		glyphs[i] = l;
		SDL_FreeSurface(temp);
		SDL_FreeSurface(text);
	}
}
