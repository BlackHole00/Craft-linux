#include "buffer.h"

vx_GlBuffer vx_glbuffer_new(const vx_GlBufferDescriptor* descriptor) {
    VX_NULL_ASSERT(descriptor);

    vx_GlBuffer buffer;
    glGenBuffers(1, &buffer.id);

    buffer.descriptor = *descriptor;

    return buffer;
}

void vx_glbuffer_bind(const vx_GlBuffer* buffer) {
    VX_NULL_ASSERT(buffer);

    glBindBuffer(buffer->descriptor.type, buffer->id);
}

void vx_glbuffer_data(const vx_GlBuffer* buffer, const void* data_ptr, usize data_size) {
    VX_NULL_ASSERT(data_ptr);
    VX_DBG_ASSERT("Invalid data size!", data_size > 0);

    vx_glbuffer_bind(buffer);
    glBufferData(buffer->descriptor.type, data_size, data_ptr, buffer->descriptor.usage);
}

void vx_glbuffer_free(const vx_GlBuffer* buffer) {
    VX_NULL_ASSERT(buffer);

    glDeleteBuffers(1, &buffer->id);
}
