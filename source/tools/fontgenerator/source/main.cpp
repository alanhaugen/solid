#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

int main(int numberOfArguments, char* argumentArray[])
{
	if (TTF_Init() != 0)
	{
		printf("Failed to initalise SDL2 ttf, quitting.\n");
		return -1;
	}

	TTF_Font* font = TTF_OpenFont("fonts/NotoSansJP-Regular.ttf", 52);

	if (font == nullptr)
	{
		printf("Failed to open font, quitting.\n");
		return -1;
	}

	SDL_Color bg = { 255, 0, 255, 255 }; // magenta
	SDL_Color color = { 0, 0, 0, 0 };

	//std::wstring text = L"あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをんがぎぐげござじずぜぞだづでどばびぶべぼぱぴぷぺぽ 。";
	std::wstring text = L" !\"#$\%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ";

	for (int i = 0; i < text.length(); i++)
	{
		std::wstring letter = text.substr(i, i+1);
		//SDL_Surface* surf = TTF_RenderUNICODE(font, (const Uint16*) letter.c_str(), color, bg);
		SDL_Surface* surf = TTF_RenderUNICODE_Blended(font, (const Uint16*) letter.c_str(), color);

		if (surf == nullptr){
			TTF_CloseFont(font);
			printf("Failed to create surface, quitting.\n");
			return -1;
		}

		char dest[256];
		if (i < 10)
		{
			snprintf(dest, 256, "output_0%d.bmp", i);
		}
		else
		{
			snprintf(dest, 256, "output_%d.bmp", i);
		}

		SDL_SaveBMP(surf, dest);

		SDL_FreeSurface(surf);
	}

	TTF_CloseFont(font);

    return 0;
}

