#pragma once

#include <glad/glad.h>
#include "../../utilis/utilis.h"

#ifdef _DEBUG
#define VX_GL_CHECK_ERRORS() { GLenum err; while((err = glGetError()) != GL_NO_ERROR){ vx_gl_error(err, __FILE__, __LINE__, __PRETTY_FUNCTION__); } }
#else
#define VX_GL_CHECK_ERRORS()
#endif

void vx_gl_error(u32, char*, int, const char*);
