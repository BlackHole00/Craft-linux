#include "program.h"

#include "gl_error.h"

static void _apply_states(const vx_GlProgramStates* states) {
    if (states->culling.culling_face == VX_CULL_NONE) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
        glCullFace(states->culling.culling_face);
        glFrontFace(states->culling.front_face); 
    }
    VX_GL_CHECK_ERRORS()

    if (states->depth_test == VX_DEPTHTEST_NONE) {
        glDisable(GL_DEPTH_TEST);
    } else {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(states->depth_test);
    }
    VX_GL_CHECK_ERRORS()

    if (!states->blending.enabled) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        glBlendFuncSeparate(states->blending.src_rgb_func,
            states->blending.dst_rgb_func,
            states->blending.src_alpha_func,
            states->blending.dstdst_alphargb_func
        );
    }
    VX_GL_CHECK_ERRORS()
}

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

void vx_glprogram_bind(vx_GlProgram* program) {
    VX_NULL_ASSERT(program);

    vx_gllayout_bind(&program->_layout);
    vx_glsimpleprogram_bind(&program->_program);
    _apply_states(&program->_states);
}

void vx_glprogram_free(vx_GlProgram* program) {
    VX_NULL_ASSERT(program);

    vx_glsimpleprogram_free(&program->_program);
    vx_gllayout_free(&program->_layout);
}
