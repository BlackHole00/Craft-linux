#include "layout.h"

#include "gl_error.h"

static void apply_layout(const vx_GlLayout* layout, const vx_GlLayoutDescriptor* descriptor) {
    VX_NULL_ASSERT(layout);
    VX_NULL_ASSERT(descriptor);

    vx_gllayout_bind(layout);
    
    usize stride = 0;
    for (u32 i = 0; i < descriptor->element_number; i++) {
        stride += vx_gltype_size(descriptor->elements[i].type) * descriptor->elements[i].count;
    }

    usize offset = stride;
    for (u32 i = descriptor->element_number; i > 0; i--) {
        u32 index = i - descriptor->element_number + 1;

        offset -= vx_gltype_size(descriptor->elements[i - 1].type) * descriptor->elements[i - 1].count;

        glVertexAttribPointer(index, descriptor->elements[i - 1].count, descriptor->elements[i - 1].type, descriptor->elements[i - 1].normalized, stride, (const void*)offset);
        glEnableVertexAttribArray(index);
    }

    VX_GL_CHECK_ERRORS()
}

vx_GlLayout vx_gllayout_new(const vx_GlLayoutDescriptor* descriptor) {
    VX_NULL_ASSERT(descriptor);
    VX_DBG_ASSERT("The descriptor has not elements!!!", descriptor->element_number > 0 && descriptor->elements != NULL);

    vx_GlLayout layout;
    glGenVertexArrays(1, &layout.id);

    apply_layout(&layout, descriptor);

    VX_GL_CHECK_ERRORS()

    return layout;
}

void vx_gllayout_bind(const vx_GlLayout* layout) {
    VX_NULL_ASSERT(layout);
    glBindVertexArray(layout->id);
    VX_GL_CHECK_ERRORS()
}

void vx_gllayout_unbind() {
    glBindVertexArray(0);
}

void vx_gllayout_free(const vx_GlLayout* layout) {
    VX_NULL_ASSERT(layout);
    glDeleteVertexArrays(1, &layout->id);
    VX_GL_CHECK_ERRORS()
}
