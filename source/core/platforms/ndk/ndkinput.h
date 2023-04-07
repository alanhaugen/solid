#ifndef __NDK_INPUT_H__
#define __NDK_INPUT_H__

#include "../../tools/input.h"

class ndkinput : public input {
public:
	ndkinput();

	mouse Mouse;
	float pressure;
};

#endif