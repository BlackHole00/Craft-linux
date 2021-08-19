#pragma once

#include "base/buffer.h"
#include "base/program.h"
#include <cglm.h>

typedef struct {
    vx_GlBufferUsage vertex_usage;
    vx_GlBufferUsage index_usage;
    const char* model_uniform_name;
} vx_MeshDescriptor;

typedef struct {
    vx_GlBuffer _v_data;
    vx_GlBuffer _i_data;

    mat4 transform;
    const char* model_uniform_name;
} vx_Mesh;

vx_Mesh vx_mesh_new(const vx_MeshDescriptor*, const vx_GlBufferData*, const vx_GlBufferData*, const mat4*);
void vx_mesh_bind(const vx_Mesh*, vx_GlProgram*);
void vx_mesh_free(const vx_Mesh*);

void vx_mesh_translate(vx_Mesh*, const vec3, const f32);
void vx_mesh_rotate(vx_Mesh*, const vec3, const f32);
void vx_mesh_scale(vx_Mesh*, const vec3, const f32);
