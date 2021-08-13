#pragma once

#include <glad/glad.h>
#include "../../utilis/utilis.h"

typedef enum {
    VX_GL_VERTEX_BUFFER = GL_VERTEX_ARRAY,
    VX_GL_INDEX_BUFFER  = GL_ELEMENT_ARRAY_BUFFER
} vx_GlBufferType;

typedef enum {
    VX_GL_STATIC_DRAW   = GL_STATIC_DRAW,
    VX_GL_STREAM_DRAW   = GL_STREAM_DRAW,
    VX_GL_DYNAMIC_DRAW  = GL_DYNAMIC_DRAW
} vx_GlBufferUsage;

typedef struct {
    vx_GlBufferType type;
    vx_GlBufferUsage usage;
} vx_GlBufferDescriptor;

typedef struct {
    vx_GlBufferDescriptor descriptor;
    GLuint id;
} vx_GlBuffer;

vx_GlBuffer vx_glbuffer_new(vx_GlBufferDescriptor*);
void vx_glbuffer_bind(vx_GlBuffer*);
void vx_glbuffer_data(vx_GlBuffer*, const void*, usize);
void vx_glbuffer_free(vx_GlBuffer*);
#define vx_glbuffer_unbind(_GL_BUFFER_TYPE) glBindBuffer((_GL_BUFFER_TYPE), 0) 
