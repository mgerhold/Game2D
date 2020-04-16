#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ScopedProfiler.h"

#ifdef _DEBUG
	#define SCOPED_PROFILER auto _dontcare = ScopedProfiler(__FILE__, __FUNCTION__, __LINE__)
#else
	#define SCOPED_PROFILER
#endif

#define GLCALL(x) x; \
	checkGLErrors(__FILE__, __LINE__)

void checkGLErrors(const char* file, int line);