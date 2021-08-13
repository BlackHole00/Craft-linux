#include "program.h"

vx_GlProgram vx_glprogram_new(vx_GlShader* vertex_shader, vx_GlShader* fragment_shader, vx_GlShader* geometry_shader, vx_GlShader* compute_shader) {
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

    return program;
}

vx_GlProgram vx_glprogram_new_d(vx_GlShader* vertex_shader, vx_GlShader* fragment_shader, vx_GlShader* geometry_shader, vx_GlShader* compute_shader) {
    vx_GlProgram program = vx_glprogram_new(vertex_shader, fragment_shader, geometry_shader, compute_shader);

    /*  vx_glprogram_new() already checks is the vertex and fragment shaders are NULL   */
    vx_glshader_free(vertex_shader);
    vx_glshader_free(fragment_shader);
    if (geometry_shader != NULL) { vx_glshader_free(geometry_shader); }
    if (compute_shader  != NULL) { vx_glshader_free(compute_shader);  }

    return program;
}

void vx_glprogram_free(vx_GlProgram* shader) {
    VX_NULL_ASSERT(shader);
    glDeleteProgram(shader->id);

    vx_vector_free(&shader->_uniform_locations);
}

_VX_OPTION_CREATE_BODY_FOR_TYPE(vx_GlProgramUniformLocationRecord)
_VX_VECTOR_CREATE_BODY_FOR_TYPE(vx_GlProgramUniformLocationRecord)
