#pragma once

#include "shader.h"

typedef struct {
    char* uniform_name;
    GLuint location;
} vx_GlProgramUniformLocationRecord;
_VX_OPTION_CREATE_PROT_FOR_TYPE(vx_GlProgramUniformLocationRecord)
_VX_VECTOR_CREATE_PROT_FOR_TYPE(vx_GlProgramUniformLocationRecord)

typedef struct {
    GLuint id;

    /*  I will implement an hashmap... Eventually. :-P  */
    VX_VECTOR(vx_GlProgramUniformLocationRecord) _uniform_locations;
} vx_GlProgram;

vx_GlProgram vx_glprogram_new(vx_GlShader*, vx_GlShader*, vx_GlShader*, vx_GlShader*);
vx_GlProgram vx_glprogram_new_d(vx_GlShader*, vx_GlShader*, vx_GlShader*,vx_GlShader*);
void vx_glprogram_free(vx_GlProgram*);
