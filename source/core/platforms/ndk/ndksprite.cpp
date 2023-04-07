#include "ndksprite.h"

/*/// Helper function /  Engine takes care ofthis
Bitmap LoadABitmap(char *filename)
{
	InputStream stream = app.getAssets().open(filename);
	Bitmap bitmap = BitmapFactory.decodeStream(stream);
	gl.glBindTexture(GL10.GL_TEXTURE_2D, textureID);
	GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0);
	return bitmap; 
}*/
/*
int jni_load_texture(const char *s) {
    JNIEnv *env = GetJEnv();
	cls = env->FindClass("com/example/SanAngeles/load_texture");
	ctor = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;)V");
	readMethod = env->GetMethodID(cls, "GetBytes", "([B)I");
	
	jstring mystr = env->NewStringUTF(s);
	
	jobject reader = env->NewObject(cls,ctor,mystr);
	
	jbyteArray bytes = env->NewByteArray(numbytes);
	jint bytesRead = env->CallIntMethod((jobject)reader, readMethod, bytes);
	jbyte *bPtr = env->GetByteArrayElements(bytes, JNI_FALSE);
	
    return bPtr;
}

int load_texture(char *filename) {
	JNIEnv* env = GetJEnv();
    jstring jstr = (*env)->NewStringUTF(env, "This comes from jni.");
    jclass clazz = (*env)->FindClass(env, "com/example/SanAngeles/load_texture");
    jmethodID messageMe = (*env)->GetMethodID(env, clazz, filename, "(Ljava/lang/String;)Ljava/lang/String;");
    jobject result = (*env)->CallObjectMethod(env, obj, messageMe, jstr);

    const char* str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    printf("%s\n", str);

    return (*env)->NewStringUTF(env, str);
}*/

/// Create a new instance of Sprite
ndksprite::ndksprite(int pos_x, int pos_y, char *filename)
	: Sprite()
{
	x = pos_x;
	y = pos_y;
	isVisible = true;
	
	// Load Bitmap
	//bitmap = LoadABitmap(filename);
	
	// Get height and witdh of bitmap
	/*BITMAP bmap;
	GetObject(bitmap, sizeof(bmap), &bmap);
	width  = bmap.bmWidth;
	height = bmap.bmHeight;*/

	// Increment and add to sprite list
/*	sprites[sprite_id] = this;
	sprite_id++;*/
};

ndksprite::ndksprite(char *filename) {
	//bitmap = LoadABitmap(filename);
	videoFrameTexture = 0;
};

void ndksprite::draw()
{
	if (isVisible) {
		// DRAW BITMAP !!!!
		static const GLfixed quadVertices[] = {
			-0x10000, -0x10000,
			 0x10000, -0x10000,
			-0x10000,  0x10000,
			 0x10000, -0x10000,
			 0x10000,  0x10000,
			-0x10000,  0x10000
		};
		
        glColor4x(0.5, 1.0, 1.0, 0);

        /*glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        glDisable(GL_LIGHTING);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        */
		glVertexPointer(2, GL_FIXED, 0, quadVertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
/*
        glEnableClientState(GL_COLOR_ARRAY);

        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
		*/
		
		/*
	

		const GLfixed fadeColor = 1.0;
		glColor4x(fadeColor, 0.0, 0.0, 0);
*/
		/*glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		glDisable(GL_LIGHTING);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(2, GL_FIXED, 0, quadVertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*glEnableClientState(GL_COLOR_ARRAY);

		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);*/
		
		// setting the texture uniform
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, videoFrameTexture);

		glUniform1i(uniforms[UNIFORM_VIDEOFRAME], 0); 
		
		// setting the attributes
		glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVertices);
		glEnableVertexAttribArray(ATTRIB_VERTEX);
		glVertexAttribPointer(ATTRIB_TEXTUREPOSITON, 2, GL_FLOAT, 0, 0, textureVertices);
		glEnableVertexAttribArray(ATTRIB_TEXTUREPOSITON);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		/*GLfloat glverts[9];
		glVertexPointer(3, GL_FLOAT, 0, glverts);
		
		//fill in vertex positions with your data
		for (int i = 0; i < 3; i++) {
		  glverts[i*3]   = ...;
		  glverts[i*3+1] = ...;
		  glverts[i*3+2] = ...;
		}*/
		
		/*HBITMAP originalBitMap;
		originalBitMap = (HBITMAP)SelectObject(bitmapHDC,bitmap);
		//BitBlt(backHDC,x,y,x+width,y+height,bitmapHDC,0,0,SRCCOPY);
		TransparentBlt(backHDC, x, y, width, height, bitmapHDC, 0, 0, width, height, RGB (255, 0, 255));
	
		SelectObject(bitmapHDC,originalBitMap); 

		//TransparentBlt(backHDC, x, y, width, height, bitmapHDC, x, y, width, height, RGB (255, 0, 255));*/
	}
};

ndksprite::~ndksprite() {
	/*hide();
	// FIXME: Swap with last, then sprite_id--
	sprite_id--;*/
}

void ndksprite::add(int anim, int frames[], int frames_length, int framerate, bool looping) {};

void ndksprite::play(char *anim) {};
	
// Collision detection (not sure if I will care to use this)
bool ndksprite::collide(int type, int x, int y) {
	/*for (unsigned int i = 0; i < sprite_id; i++) { 
		if (sprites[i]->isVisible && sprites[i]->type == type && 
			(sprites[i]->x > x && sprites[i]->x + sprites[i]->width  < x) &&
			(sprites[i]->y < y && sprites[i]->y + sprites[i]->height > y))
		{
			return true;
		}
	}*/
	return false;
};

// Horizontal flip
void ndksprite::flip() {
};

// Vertical flip
void ndksprite::vflip() {
};

void ndksprite::hide() {
	isVisible = false;
};

void ndksprite::show() {
	isVisible = true;
};

void ndksprite::rotationEnable() {
};

void ndksprite::rotate(int theta) {
	rotationEnable();
	
	// rotate..
};

	
	/*int x, y, width, height, type, offset_x, offset_y;
	
	u8 *m_obj_data[];
	int m_obj_shape;
	int m_obj_size;*/

	//LoadTileData(4, 0, obj_data, sizeof earthsTiles);
	//LoadPaletteObjData(0, earthsPal, sizeof earthsPal);
	
	/*SetObject(0,
	          ATTR0_SHAPE(0) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(76),
			  ATTR1_SIZE(0) | ATTR1_X(116),
			  ATTR2_ID8(4));
	*/
	
	/*SetObject(0,  // id
	ATTR0_SHAPE(0) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(76),
	ATTR1_SIZE(0) | ATTR1_X(116),
	ATTR2_ID8(4));*/