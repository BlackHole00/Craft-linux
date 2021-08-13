#pragma once

#include <glad/glad.h>
#include "../../utilis/utilis.h"

typedef enum {
    VX_GL_VERTEX_BUFFER = GL_ARRAY_BUFFER,
    VX_GL_INDEX_BUFFER  = GL_ELEMENT_ARRAY_BUFFER
} vx_GlBufferType;

typedef enum {
    VX_GL_STATIC_DRAW   = GL_STATIC_DRAW,
    VX_GL_STREAM_DRAW   = GL_STREAM_DRAW,
    VX_GL_DYNAMIC_DRAW  = GL_DYNAMIC_DRAW
} vx_GlBufferUsage;

typedef struct {
    const void* data;
    usize data_size;
} vx_GlBufferData;

typedef struct {
    vx_GlBufferType type;
    vx_GlBufferUsage usage;
} vx_GlBufferDescriptor;

typedef struct {
    vx_GlBufferDescriptor descriptor;
    GLuint id;
} vx_GlBuffer;

vx_GlBuffer vx_glbuffer_new(const vx_GlBufferDescriptor*, const vx_GlBufferData*);
void vx_glbuffer_bind(const vx_GlBuffer*);
void vx_glbuffer_data(const vx_GlBuffer*, const vx_GlBufferData*);
void vx_glbuffer_free(const vx_GlBuffer*);
#define vx_glbuffer_unbind(_GL_BUFFER_TYPE) glBindBuffer((_GL_BUFFER_TYPE), 0) 
