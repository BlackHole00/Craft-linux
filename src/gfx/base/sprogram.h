#pragma once

#include "shader.h"

typedef struct {
    char* uniform_name;
    GLuint location;
} vx_GlProgramUniformLocationRecord;
_VX_OPTION_CREATE_PROT_FOR_TYPE(vx_GlProgramUniformLocationRecord)
_VX_VECTOR_CREATE_PROT_FOR_TYPE(vx_GlProgramUniformLocationRecord)

typedef struct {
    const vx_GlShader* vertex_shader;
    const vx_GlShader* fragment_shader;
    const vx_GlShader* geometry_shader;
    const vx_GlShader* compute_shader;
} vx_GlSimpleProgramDescriptor;

typedef struct {
    GLuint id;

    /*  I will implement an hashmap... Eventually. :-P  */
    VX_VECTOR(vx_GlProgramUniformLocationRecord) _uniform_locations;
} vx_GlSimpleProgram;

vx_GlSimpleProgram vx_glsimpleprogram_new(const vx_GlSimpleProgramDescriptor*);
vx_GlSimpleProgram vx_glsimpleprogram_new_d(const vx_GlSimpleProgramDescriptor*);
void vx_glsimpleprogram_free(vx_GlSimpleProgram*);

void vx_glsimpleprogram_bind(const vx_GlSimpleProgram*);
void vx_glsimpleprogram_unbind();

/*  In the future I'll add more types...    */
void VX_TEMPLATE_NAME(f32, vx_glsimpleprogram_uniform)(vx_GlSimpleProgram*, char*, f32);
