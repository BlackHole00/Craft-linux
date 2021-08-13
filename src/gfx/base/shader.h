#pragma once
#include <glad/glad.h>
#include "../../utilis/utilis.h"

typedef enum {
    VX_GL_VERTEX_SHADER     = GL_VERTEX_SHADER,
    VX_GL_FRAGMENT_SHADER   = GL_FRAGMENT_SHADER,
    VX_GL_GEOMETRY_SHADER   = GL_GEOMETRY_SHADER,
    VX_GL_COMPUTE_SHADER    = GL_COMPUTE_SHADER
} vx_GlShaderType;

typedef struct {
    vx_GlShaderType type;
    char* shader_path;
} vx_GlShaderDescriptor;
VX_CREATE_DEFAULT(vx_GlShaderDescriptor, .type = VX_GL_VERTEX_SHADER, .shader_path = "")

typedef struct {
    GLuint id;
} vx_GlShader;

vx_GlShader vx_glshader_new(const vx_GlShaderDescriptor*);
void vx_glshader_free(const vx_GlShader*);
