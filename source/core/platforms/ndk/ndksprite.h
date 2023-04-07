#ifndef __NDK_SPRITE_H__
#define __NDK_SPRITE_H__

//#include <Bitmap.h>
//#include <BitmapFactory.h>
#include <GLES/gl.h>
#include <jni.h>
//#include <GLES/glext.h>
#include "../../renderer/sprite.h"

/// Sprite class
/// Supports collisions and animation
class ndksprite : public Sprite {
protected:
	//char *animations[];
	
	//Hitbox m_hitbox;
    
public:	
	ndksprite() {};
	~ndksprite();

	ndksprite(int pos_x, int pos_y, char *filename);
	ndksprite(char *filename);

	// Add animation
	void add(int anim, int frames[], int frames_length, int framerate, bool looping);
	
	// Play
	void play(char *anim);
	
	// hide sprite
	void hide();
	
	// show sprite
	void show();
	
	// flip sprite
	void flip();
	void vflip();
	
	// rotation
	void rotationEnable();
	void rotate(int theta); // (If rotation is not already enabled, this will enable it.)
	
	// Collision detection (might need a Sprite_System Singleton class to hanlde this)
	bool collide(int type, int x, int y);

	void draw();
	//Bitmap bitmap;
	
	GLuint videoFrameTexture;
};

/*
static const GLfloat squareVertices[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f,  1.0f,
	1.0f,  1.0f,
};

static const GLfloat textureVertices[] = {
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
};*/

#endif