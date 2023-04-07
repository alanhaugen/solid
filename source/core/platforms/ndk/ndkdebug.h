#ifndef NDK_DEBUG_H
#define NDK_DEBUG_H

#include <jni.h>
#include <android/log.h>
#include "../../tools/debug.h"

class ndkdebug: public debug {
public:
	void Log(const char *text, ...);
	void LogError(const char *text, ...);
	void LogWarning(const char *text, ...);
};

#endif