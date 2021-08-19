#pragma once

#include <glad/glad.h>
#include "../../utilis/utilis.h"

#define VX_DEFAULT_MODEL_UNIFORM_NAME "uModel"
#define VX_DEFAULT_VIEW_UNIFORM_NAME "uView"
#define VX_DEFAULT_PROJ_UNIFORM_NAME "uProj"

typedef enum {
    VX_GL_F32   = GL_FLOAT,
    VX_GL_I32   = GL_INT,
    VX_GL_U32   = GL_UNSIGNED_INT,
    VX_GL_BYTE  = GL_UNSIGNED_BYTE

    /*  I'll fill the others as I need...   */
} vx_GlType;

usize vx_gltype_size(vx_GlType);
