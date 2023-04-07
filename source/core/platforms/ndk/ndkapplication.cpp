// ndkapplication.cpp
// The cpp file defines the implementation of the class

#include "ndkapplication.h"

NDKApplication::NDKApplication()
	: Application()
{
	Input = ndkinput();
	Debug = ndkdebug();
	sceneManager = SceneManager();
	m_renderer = new NDKRender();

	SetRenderer(m_renderer);
}

void NDKApplication::UpdateFrame() {
}

Application::~Application() {
}

void NDKApplication::Update() {
	// Update Sprites
	sceneManager.Update(); 

	// BLEH! Update the input system
	Input.Update();

#ifdef _DEBUG
	// Update Debug Console
	Debug.Update();
#endif

	// Increment Frame variable
	Frame++;
}

void NDKApplication::Run() {
	/*while(running) {
		Update();
		if (!paused)
			UpdateFrame();
	}*/
}

int NDKApplication::Main(int argumentQuantity, char *arguments[])
{
	return false;
}

void Button::Update() {
}