INCLUDES = -I ../../../include/darwin
LIBS = -L ../../../lib/debug/darwin/arm64

LIBS += -lm -liconv -framework CoreMIDI -framework CoreFoundation -framework AudioToolbox -framework CoreAudio -framework Carbon -framework ForceFeedback -framework CoreVideo -framework AppKit -framework Metal -framework IOKit -framework Cocoa
LIBS += -framework CoreHaptics -framework MediaPlayer -framework GameController

fontgenerator: source/main.cpp
	clang++ source/main.cpp $(INCLUDES) $(LIBS) -lsdl2 -lsdl2_ttf -lfreetype -o fontgenerator 
