#ifndef __NDK_RENDER_H__
#define __NDK_RENDER_H__

#include "../../renderer/renderer.h"

class NDKRender : public IRenderer
{
private:
	void DeInitialize();

	void PreRender();

	void PostRender();

public:
	NDKRender();
	~NDKRender();

	bool Init(bool openWindowed = false, const char *windowTitle  = "SolidCore");

	void SetBuffers();

	void Render(SceneManager*);

	void FlipDoubleBuffer();

	void SetClearColour();

	void Resize(int width, int height);
};

#endif