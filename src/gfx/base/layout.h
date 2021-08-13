#pragma once

#include <glad/glad.h>
#include "base.h"
#include "../../utilis/utilis.h"

typedef struct {
    vx_GlType type;
    u32 count;
    bool normalized;
} vx_GlLayoutElement;

typedef struct {
    usize element_number;
    vx_GlLayoutElement* elements;
} vx_GlLayoutDescriptor;

typedef struct {
    GLuint id;
} vx_GlLayout;

vx_GlLayout vx_gllayout_new(const vx_GlLayoutDescriptor*);
void vx_gllayout_bind(const vx_GlLayout*);
void vx_gllayout_unbind();
void vx_gllayout_free(const vx_GlLayout*);
