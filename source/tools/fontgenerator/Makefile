INCLUDES = -I /usr/local/Cellar/sdl2/2.0.12_1/include/SDL2 -I /usr/local/Cellar/sdl2_ttf/2.0.15/include/SDL2
LIBS = -L /usr/local/Cellar/sdl2/2.0.12_1/lib -L /usr/local/Cellar/sdl2_ttf/2.0.15/lib

fontgenerator: source/main.cpp
	clang++ source/main.cpp $(INCLUDES) $(LIBS) -lsdl2 -lsdl2_ttf -o fontgenerator
