#include "program.h"

vx_GlProgram vx_glprogram_new(const vx_GlProgramDescriptor* descriptor) {
    VX_NULL_ASSERT(descriptor);

    vx_GlProgram program;
    program._program = vx_glsimpleprogram_new(&(vx_GlSimpleProgramDescriptor){
        .vertex_shader = descriptor->vertex_shader,
        .fragment_shader = descriptor->fragment_shader,
        .geometry_shader = descriptor->geometry_shader,
        .compute_shader  = descriptor->compute_shader 
    });
    program._layout = vx_gllayout_new(descriptor->layout);
    program._states = *descriptor->states;    

    return program;
}

vx_GlProgram vx_glprogram_new_d(const vx_GlProgramDescriptor* descriptor) {
    vx_GlProgram program = vx_glprogram_new(descriptor);

    vx_glshader_free(descriptor->vertex_shader);
    vx_glshader_free(descriptor->fragment_shader);
    if (descriptor->geometry_shader != NULL) { vx_glshader_free(descriptor->geometry_shader); }
    if (descriptor->compute_shader  != NULL) { vx_glshader_free(descriptor->compute_shader);  }

    return program;
}

void vx_glprogram_free(vx_GlProgram* program) {
    VX_NULL_ASSERT(program);

    vx_glsimpleprogram_free(&program->_program);
    vx_gllayout_free(&program->_layout);
}
