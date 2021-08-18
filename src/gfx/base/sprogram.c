#include "sprogram.h"
#include <string.h>

#include "gl_error.h"

static GLuint get_uniform_location(vx_GlSimpleProgram* program, char* uniform_name) {
    VX_NULL_ASSERT(program);
    VX_NULL_ASSERT(uniform_name);
    
    i32 location = -1;
    VX_VECTOR_FOREACH(vx_GlProgramUniformLocationRecord, record, &program->_uniform_locations, 
        if (strcmp(uniform_name, record.uniform_name) == 0) {
            location = record.location;
            break;
        }
    )

    if (location == -1) {
        /*  Location not found in vector. Get uniform location and register it. */
        location = glGetUniformLocation(program->id, uniform_name);

        VX_DBG_ASSERT("Uniform not valid!!!", location >= 0);

        VX_T(vx_GlProgramUniformLocationRecord, vx_vector_push)(&program->_uniform_locations, (vx_GlProgramUniformLocationRecord){
            .location = location,
            .uniform_name = uniform_name
        });
    }

    return location;
}

vx_GlSimpleProgram vx_glsimpleprogram_new(const vx_GlSimpleProgramDescriptor* descriptor) {
    VX_NULL_ASSERT(descriptor);
    VX_NULL_ASSERT(descriptor->vertex_shader);
    VX_NULL_ASSERT(descriptor->fragment_shader);

    vx_GlSimpleProgram program;
    program._uniform_locations = vx_vector_new();

    program.id = glCreateProgram();
    glAttachShader(program.id, descriptor->vertex_shader->id);
    glAttachShader(program.id, descriptor->fragment_shader->id);
    if (descriptor->geometry_shader != NULL) { glAttachShader(program.id, descriptor->geometry_shader->id); }
    if (descriptor->compute_shader  != NULL) { glAttachShader(program.id, descriptor->compute_shader->id);  }
    glLinkProgram(program.id);

    VX_GL_CHECK_ERRORS()

    return program;
}

vx_GlSimpleProgram vx_glsimpleprogram_new_d(const vx_GlSimpleProgramDescriptor* descriptor) {
    vx_GlSimpleProgram program = vx_glsimpleprogram_new(descriptor);

    /*  vx_glprogram_new() already checks is the vertex and fragment shaders are NULL   */
    vx_glshader_free(descriptor->vertex_shader);
    vx_glshader_free(descriptor->fragment_shader);
    if (descriptor->geometry_shader != NULL) { vx_glshader_free(descriptor->geometry_shader); }
    if (descriptor->compute_shader  != NULL) { vx_glshader_free(descriptor->compute_shader);  }

    return program;
}

void vx_glsimpleprogram_free(vx_GlSimpleProgram* program) {
    VX_NULL_ASSERT(program);
    glDeleteProgram(program->id);
    VX_GL_CHECK_ERRORS()

    vx_vector_free(&program->_uniform_locations);
}

void vx_glsimpleprogram_bind(const vx_GlSimpleProgram* program) {
    VX_NULL_ASSERT(program);
    glUseProgram(program->id);
    VX_GL_CHECK_ERRORS()
}

void vx_glsimpleprogram_unbind() {
    glUseProgram(0);
}

void vx_glsimpleprogram_uniform_f32(vx_GlSimpleProgram* program, char* uniform_name, f32 value) {
    VX_NULL_ASSERT(program);

    vx_glsimpleprogram_bind(program);
    glUniform1f(get_uniform_location(program, uniform_name), value);
    VX_GL_CHECK_ERRORS()
}

void vx_glsimpleprogram_uniform_mat4(vx_GlSimpleProgram* program, char* uniform_name, mat4 value) {
    VX_NULL_ASSERT(program)

    vx_glsimpleprogram_bind(program);
    glUniformMatrix4fv(get_uniform_location(program, uniform_name), 1, GL_FALSE, (const f32*)value);
    VX_GL_CHECK_ERRORS()
}

_VX_OPTION_CREATE_BODY_FOR_TYPE(vx_GlProgramUniformLocationRecord)
_VX_VECTOR_CREATE_BODY_FOR_TYPE(vx_GlProgramUniformLocationRecord)
