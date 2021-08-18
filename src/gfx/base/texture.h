#pragma once

#include "base.h"
#include <glad/glad.h>

typedef enum {
    VX_GL_TEXTURE_2D = GL_TEXTURE_2D
    /*  I'll add more in the future as I need...    */
} vx_GlTextureType;

typedef enum {
    VX_GL_RED               = GL_RED,
    VX_GL_RG                = GL_RG,
    VX_GL_RGB               = GL_RGB,
    VX_GL_RGBA              = GL_RGBA,
    VX_GL_DEPTH_COMPONENT   = GL_DEPTH_COMPONENT,
    VX_GL_DEPTH_STENCIL     = GL_DEPTH_STENCIL
} vx_GlTextureFormat;

typedef enum {
    VX_GL_REPEAT            = GL_REPEAT,
    VX_GL_MIRRORED_REPEAT   = GL_MIRRORED_REPEAT,
    VX_GL_CLAMP_TO_EDGE     = GL_CLAMP_TO_EDGE,
    VX_GL_CLAMP_TO_BORDER   = GL_CLAMP_TO_BORDER,
} vx_GlTextureWarp;

typedef enum {
    VX_GL_NEAREST                   = GL_NEAREST,
    VX_GL_LINEAR                    = GL_LINEAR,
    VX_GL_NEAREST_MIPMAP_NEAREST    = GL_NEAREST_MIPMAP_NEAREST,
    VX_GL_LINEAR_MIPMAP_NEAREST     = GL_LINEAR_MIPMAP_NEAREST,
    VX_GL_NEAREST_MIPMAP_LINEAR     = GL_NEAREST_MIPMAP_LINEAR,
    VX_GL_LINEAR_MIPMAP_LINEAR      = GL_LINEAR_MIPMAP_LINEAR,
} vx_GlTextureFilter;

/*typedef enum {
    //  Todo: for now i'm using vx_GlTextureFormat. Look at http://docs.gl/gl4/glTexImage2D for implementing.
} _vx_GlTextureInternalFormat;*/
typedef vx_GlTextureFormat vx_GltextureInternalFormat;

typedef struct {
    vx_GlTextureFormat format;
    vx_GlType encoding_type;
    u32 width;
    u32 height;
    void* data;
} vx_GlTextureData;

typedef struct {
    vx_GlTextureType type;
    vx_GltextureInternalFormat format;  /*  this is used as an internal format. */
    GLuint texture_unit;
    
    vx_GlTextureWarp warp_s;
    vx_GlTextureType warp_t;
    vx_GlTextureFilter min_filter;
    vx_GlTextureFilter mag_filter;
} vx_GlTextureDescriptor;

typedef struct {
    GLuint id;

    vx_GlTextureType type;
    vx_GlTextureFormat format;  /*  this is used as an internal format. */
    GLuint texture_unit;
} vx_GlTexture;

vx_GlTexture vx_gltexture_new(const vx_GlTextureDescriptor*, const vx_GlTextureData*);
vx_GlTexture vx_gltexture_from_path(const vx_GlTextureDescriptor*, const char*);
void vx_gltexture_bind(const vx_GlTexture*);
void vx_gltexture_data(const vx_GlTexture*, const vx_GlTextureData*);
void vx_gltexture_data_from_path(const vx_GlTexture*, const char*);
void vx_gltexture_free(const vx_GlTexture*);
