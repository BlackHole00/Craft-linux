#pragma once

#include <glad/glad.h>
#include "sprogram.h"
#include "layout.h"

typedef enum {
    VX_CULL_NONE        = 0,
    VX_GL_FRONT         = GL_FRONT,
    VX_GL_BACK          = GL_BACK,
    VX_GL_FRONT_AND_BACK = GL_FRONT_AND_BACK
} vx_GlCullFace;

typedef enum {
    VX_GL_CW            = GL_CW,
    VX_GL_CCW           = GL_CCW,
} vx_GlFrontFace;

typedef enum {
    VX_DEPTHTEST_NONE   = 0,
    VX_GL_ALWAYS        = GL_ALWAYS,
    VX_GL_NEVER         = GL_NEVER,
    VX_GL_LESS          = GL_LESS,
    VX_GL_EQUAL         = GL_EQUAL,
    VX_GL_LEQUAL        = GL_LEQUAL,
    VX_GL_GREATER       = GL_GREATER,
    VX_GL_NOTEQUAL      = GL_NOTEQUAL,
    VX_GL_GEQUAL        = GL_GEQUAL
} vx_GlDepthTest;

typedef enum {
    VX_BLENDING_NONE    = 0,
    VX_GL_ZERO          = GL_ZERO,
    VX_GL_ONE           = GL_ONE,
    VX_GL_SRC_COLOR     = GL_SRC_COLOR,
    VX_GL_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
    VX_GL_DST_COLOR     = GL_DST_COLOR,
    VX_GL_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
    VX_GL_SRC_ALPHA     = GL_SRC_ALPHA,
    VX_GL_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    VX_GL_DST_ALPHA     = GL_DST_ALPHA,
    VX_GL_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
    VX_GL_CONSTANT_COLOR = GL_CONSTANT_COLOR,
    VX_GL_ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
    VX_GL_CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
    VX_GL_ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA
} vx_GlBlendFunc;

typedef struct {
    bool enabled;
    vx_GlBlendFunc src_rgb_func;
    vx_GlBlendFunc dst_rgb_func;
    vx_GlBlendFunc src_alpha_func;
    vx_GlBlendFunc dstdst_alphargb_func;
} vx_GlProgramBlendingInfo;

typedef struct {
    vx_GlCullFace culling_face;
    vx_GlFrontFace front_face;
} vx_GlProgramCullingInfo;

typedef struct {
    vx_GlProgramCullingInfo culling;

    vx_GlDepthTest depth_test;

    vx_GlProgramBlendingInfo blending;
} vx_GlProgramStates;

typedef struct {
    const vx_GlShader* vertex_shader;
    const vx_GlShader* fragment_shader;
    const vx_GlShader* geometry_shader;
    const vx_GlShader* compute_shader;

    const vx_GlLayoutDescriptor* layout;
    const vx_GlProgramStates* states;
} vx_GlProgramDescriptor;

typedef struct {
    vx_GlSimpleProgram _program;
    vx_GlLayout _layout;
    vx_GlProgramStates _states;
} vx_GlProgram;

vx_GlProgram vx_glprogram_new(const vx_GlProgramDescriptor*);
vx_GlProgram vx_glprogram_new_d(const vx_GlProgramDescriptor*);
void vx_glprogram_free(vx_GlProgram*);
void vx_glprogram_bind(vx_GlProgram*);

void static inline vx_glprogram_uniform_f32(vx_GlProgram* program, const char* name, const f32 value) {
    VX_NULL_ASSERT(program);
    vx_glprogram_bind(program);
    vx_glsimpleprogram_uniform_f32(&program->_program, name, value);
}
void static inline vx_glprogram_uniform_mat4(vx_GlProgram* program, const char* name, const mat4 value) {
    VX_NULL_ASSERT(program);
    vx_glprogram_bind(program);
    vx_glsimpleprogram_uniform_mat4(&program->_program, name, value);
}
