#include "texture.h"

#include "../../utilis/utilis.h"
#include <stb_image.h>
#include "gl_error.h"

vx_GlTexture vx_gltexture_new(const vx_GlTextureDescriptor* descriptor, const vx_GlTextureData* data) {
    VX_NULL_ASSERT(descriptor);

    vx_GlTexture texture;
    glGenTextures(1, &texture.id);

    texture.format = descriptor->format;
    texture.texture_unit = descriptor->texture_unit;
    texture.type = descriptor->type;

    vx_gltexture_bind(&texture);
    VX_GL_CHECK_ERRORS()

    glTexParameteri(texture.type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(texture.type, GL_TEXTURE_MAG_FILTER, descriptor->mag_filter);
    glTexParameteri(texture.type, GL_TEXTURE_WRAP_S, descriptor->warp_s);
    glTexParameteri(texture.type, GL_TEXTURE_WRAP_T, descriptor->warp_t);

    if (data != NULL) {
        vx_gltexture_data(&texture, data);
    }

    VX_GL_CHECK_ERRORS()

    return texture;
}

vx_GlTexture vx_gltexture_from_path(const vx_GlTextureDescriptor* descriptor, const char* path) {
    VX_NULL_ASSERT(descriptor);
    VX_NULL_ASSERT(path);

    vx_GlTexture texture = vx_gltexture_new(descriptor, NULL);
    vx_gltexture_data_from_path(&texture, path);
    return texture;
}

void vx_gltexture_bind(const vx_GlTexture* texture) {
    VX_NULL_ASSERT(texture);

    glActiveTexture(GL_TEXTURE0 + texture->texture_unit);
    glBindTexture(texture->type, texture->id);

    VX_GL_CHECK_ERRORS()
}

void vx_gltexture_data(const vx_GlTexture* texture, const vx_GlTextureData* data) {
    VX_NULL_ASSERT(texture);
    VX_NULL_ASSERT(data);

    vx_gltexture_bind(texture);
    glTexImage2D(texture->type, 0, texture->format, data->width, data->height, 0, data->format, data->encoding_type, data->data);
    glGenerateMipmap(texture->type);

    VX_GL_CHECK_ERRORS()
}

void vx_gltexture_data_from_path(const vx_GlTexture* texture, const char* path) {
    VX_NULL_ASSERT(texture);
    VX_NULL_ASSERT(path);

    vx_GlTextureData data;
    i32 nr_channels;
    {
        i32 width;
        i32 height;
        data.data   = stbi_load(path, &width, &height, &nr_channels, 0);
        data.width  = (u32)width;
        data.height = (u32)height;
    }

    data.encoding_type = VX_GL_BYTE;

    switch (nr_channels) {
        case 1:
            data.format = VX_GL_RED;
            break;
        case 2:
            data.format = VX_GL_RG;
            break;
        case 3:
            data.format = VX_GL_RGB;
            break;
        case 4:
            data.format = VX_GL_RGBA;
            break;
        default:
            VX_PANIC("Image with number of channels not valid!!!");
    }

    vx_gltexture_data(texture, &data);

    stbi_image_free(data.data);
}

void vx_gltexture_free(const vx_GlTexture* texture) {
    VX_NULL_ASSERT(texture);

    glDeleteTextures(1, &texture->id);

    VX_GL_CHECK_ERRORS()
}
