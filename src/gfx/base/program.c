#include "program.h"
#include <string.h>

#include "gl_error.h"

static GLuint get_uniform_location(vx_GlProgram* program, char* uniform_name) {
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

vx_GlProgram vx_glprogram_new(const vx_GlShader* vertex_shader, const vx_GlShader* fragment_shader, const vx_GlShader* geometry_shader, const vx_GlShader* compute_shader) {
    VX_NULL_ASSERT(vertex_shader);
    VX_NULL_ASSERT(fragment_shader);

    vx_GlProgram program;
    program._uniform_locations = vx_vector_new();

    program.id = glCreateProgram();
    glAttachShader(program.id, vertex_shader->id);
    glAttachShader(program.id, fragment_shader->id);
    if (geometry_shader != NULL) { glAttachShader(program.id, geometry_shader->id); }
    if (compute_shader  != NULL) { glAttachShader(program.id, compute_shader->id);  }
    glLinkProgram(program.id);

    VX_GL_CHECK_ERRORS()

    return program;
}

vx_GlProgram vx_glprogram_new_d(const vx_GlShader* vertex_shader, const vx_GlShader* fragment_shader, const vx_GlShader* geometry_shader, const vx_GlShader* compute_shader) {
    vx_GlProgram program = vx_glprogram_new(vertex_shader, fragment_shader, geometry_shader, compute_shader);

    /*  vx_glprogram_new() already checks is the vertex and fragment shaders are NULL   */
    vx_glshader_free(vertex_shader);
    vx_glshader_free(fragment_shader);
    if (geometry_shader != NULL) { vx_glshader_free(geometry_shader); }
    if (compute_shader  != NULL) { vx_glshader_free(compute_shader);  }

    return program;
}

void vx_glprogram_free(vx_GlProgram* program) {
    VX_NULL_ASSERT(program);
    glDeleteProgram(program->id);
    VX_GL_CHECK_ERRORS()

    vx_vector_free(&program->_uniform_locations);
}

void vx_glprogram_bind(const vx_GlProgram* program) {
    VX_NULL_ASSERT(program);
    glUseProgram(program->id);
    VX_GL_CHECK_ERRORS()
}

void vx_glprogram_unbind() {
    glUseProgram(0);
}

void VX_TEMPLATE_NAME(f32, vx_glprogram_uniform)(vx_GlProgram* program, char* uniform_name, f32 value) {
    VX_NULL_ASSERT(program);

    vx_glprogram_bind(program);
    glUniform1f(get_uniform_location(program, uniform_name), value);
    VX_GL_CHECK_ERRORS()
}

_VX_OPTION_CREATE_BODY_FOR_TYPE(vx_GlProgramUniformLocationRecord)
_VX_VECTOR_CREATE_BODY_FOR_TYPE(vx_GlProgramUniformLocationRecord)
