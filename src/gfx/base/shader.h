#pragma once
#include <glad/glad.h>
#include "../../utilis/utilis.h"

typedef enum {
    VX_GL_VERTEX_SHADER     = GL_VERTEX_SHADER,
    VX_GL_FRAGMENT_SHADER   = GL_FRAGMENT_SHADER,
    VX_GL_GEOMETRY_SHADER   = GL_GEOMETRY_SHADER,
    VX_GL_COMPUTE_SHADER    = GL_COMPUTE_SHADER
} vx_GlShader_Type;

typedef struct {
    vx_GlShader_Type type;
    char* shader_path;
} vx_GlShaderDescriptor;
VX_CREATE_DEFAULT(vx_GlShaderDescriptor, .type = VX_GL_VERTEX_SHADER, .shader_path = "")

typedef struct {
    GLuint id;
} vx_GlShader;

vx_GlShader vx_glshader_new(vx_GlShaderDescriptor*);
void vx_glshader_free(vx_GlShader*);

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
