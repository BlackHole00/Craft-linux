#include "texture.h"

#include "../../utilis/utilis.h"

vx_GlTexture vx_gltexture_new(const vx_GlTextureDescriptor* descriptor, const vx_GlTextureData* data) {
    VX_NULL_ASSERT(descriptor);

    vx_GlTexture texture;
    glGenTextures(1, &texture.id);

    texture.format = descriptor->format;
    texture.texture_unit = descriptor->texture_unit;
    texture.type = descriptor->type;

    vx_gltexture_bind(&texture);

    glTexParameteri(texture.format, GL_TEXTURE_MIN_FILTER, descriptor->min_filter);
    glTexParameteri(texture.format, GL_TEXTURE_MAG_FILTER, descriptor->mag_filter);
    glTexParameteri(texture.format, GL_TEXTURE_WRAP_S, descriptor->warp_s);
    glTexParameteri(texture.format, GL_TEXTURE_WRAP_T, descriptor->warp_t);

    if (data != NULL) {
        vx_gltexture_data(&texture, data);
    }

    return texture;
}

void vx_gltexture_bind(const vx_GlTexture* texture) {
    VX_NULL_ASSERT(texture);

    glActiveTexture(GL_TEXTURE0 + texture->texture_unit);
    glBindTexture(texture->type, texture->id);
}

void vx_gltexture_data(const vx_GlTexture* texture, const vx_GlTextureData* data) {
    VX_NULL_ASSERT(texture);
    VX_NULL_ASSERT(data);

    vx_gltexture_bind(texture);
    glTexImage2D(texture->type, 0, texture->format, data->width, data->height, 0, data->format, data->encoding_type, data->data);
    glGenerateMipmap(texture->type);
}

void vx_gltexture_free(const vx_GlTexture* texture) {
    VX_NULL_ASSERT(texture);

    glDeleteTextures(1, &texture->id);
}
