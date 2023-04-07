#include "ndkrender.h"

NDKRender::NDKRender() {
}

NDKRender::~NDKRender() {
	DeInitialize();
}

void NDKRender::DeInitialize() {
}

void NDKRender::PreRender() {
}

void NDKRender::PostRender() {
}

bool NDKRender::Init(bool openWindowed, const char *windowTitle) {
	return true;
}

void NDKRender::SetBuffers() {

}

void NDKRender::Render(SceneManager *sceneManager)  {
	PreRender();	
	sceneManager->Render();
	//al_draw_text(font, al_map_rgba_f(1, 1, 1, 0.5), 0, 0, 0, text);
	PostRender();
}

void NDKRender::FlipDoubleBuffer() {
}

void NDKRender::SetClearColour() {
	//al_clear_to_color(ALLEGRO_COLOR color);
}

void NDKRender::Resize(int width, int height) {

}
