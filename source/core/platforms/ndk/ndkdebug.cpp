#include "ndkdebug.h"

void ndkdebug::Log(const char *text, ...) {
	__android_log_print(ANDROID_LOG_INFO, "Game", text);
}

void ndkdebug::LogError(const char *text, ...) {
	__android_log_print(ANDROID_LOG_WARN, "Game", text);
}

void ndkdebug::LogWarning(const char *text, ...) {
	__android_log_print(ANDROID_LOG_DEBUG, "Game", text);
}
