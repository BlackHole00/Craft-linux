#include "base.h"

usize vx_gltype_size(vx_GlType type) {
    switch (type) {
        case VX_GL_F32:
            return sizeof(GLfloat);
        case VX_GL_I32:
            return sizeof(GLint);
        case VX_GL_U32:
            return sizeof(GLuint);
        case VX_GL_BYTE:
            return sizeof(GLubyte);
        default:
            VX_PANIC("Unknown type!");
    };

    return 0;
}
