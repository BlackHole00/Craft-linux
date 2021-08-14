#include "gl_error.h"

void vx_gl_error(u32 error, char* file, int line, const char* function) { 
    switch (error) {
        case GL_INVALID_ENUM:
            printf("GL_INVALID_ENUM error in function %s(%s::%d)\n", function, file, line);
            break;
        case GL_INVALID_VALUE:
            printf("GL_INVALID_VALUE error in function %s(%s::%d)\n", function, file, line);
            break;
        case GL_INVALID_OPERATION:
            printf("GL_INVALID_OPERATION error in function %s(%s::%d)\n", function, file, line);
            break;
        case GL_STACK_OVERFLOW:
            printf("GL_STACK_OVERFLOW error in function %s(%s::%d)\n", function, file, line);
            break;
        case GL_STACK_UNDERFLOW:
            printf("GL_STACK_UNDERFLOW error in function %s(%s::%d)\n", function, file, line);
            break;
        case GL_OUT_OF_MEMORY:
            printf("GL_OUT_OF_MEMORY error in function %s(%s::%d)\n", function, file, line);
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("GL_INVALID_FRAMEBUFFER_OPERATION error in function %s(%s::%d)\n", function, file, line);
            break;
        default:
            printf("Unknown gl error in function %s(%s::%d)\n", function, file, line);
    }
}
