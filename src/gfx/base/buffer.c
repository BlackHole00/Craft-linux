#include "buffer.h"

#include "gl_error.h"

vx_GlBuffer vx_glbuffer_new(const vx_GlBufferDescriptor* descriptor, const vx_GlBufferData* initial_data) {
    VX_NULL_ASSERT(descriptor);

    vx_GlBuffer buffer;
    glGenBuffers(1, &buffer.id);

    buffer.descriptor = *descriptor;

    if (initial_data != NULL) {
        vx_glbuffer_data(&buffer, initial_data);
    }
    VX_GL_CHECK_ERRORS()

    return buffer;
}

void vx_glbuffer_bind(const vx_GlBuffer* buffer) {
    VX_NULL_ASSERT(buffer);
    glBindBuffer(buffer->descriptor.type, buffer->id);
    VX_GL_CHECK_ERRORS()
}

void vx_glbuffer_data(const vx_GlBuffer* buffer, const vx_GlBufferData* data) {
    VX_NULL_ASSERT(data);
    VX_NULL_ASSERT(data->data);
    VX_DBG_ASSERT("Invalid data size!", data->data_size > 0);

    vx_glbuffer_bind(buffer);
    glBufferData(buffer->descriptor.type, data->data_size, data->data, buffer->descriptor.usage);
    VX_GL_CHECK_ERRORS()
}

void vx_glbuffer_free(const vx_GlBuffer* buffer) {
    VX_NULL_ASSERT(buffer);
    glDeleteBuffers(1, &buffer->id);
    VX_GL_CHECK_ERRORS()
}
