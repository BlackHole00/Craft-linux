#include "mesh.h"

vx_Mesh vx_mesh_new(const vx_MeshDescriptor* descriptor, const vx_GlBufferData* initial_vertex_data, const vx_GlBufferData* initial_index_data, const mat4* initial_transform) {
    VX_NULL_ASSERT(descriptor);
    VX_NULL_ASSERT(descriptor->model_uniform_name);

    vx_Mesh mesh;
    mesh._v_data = vx_glbuffer_new(&(vx_GlBufferDescriptor){
        .type   = VX_GL_VERTEX_BUFFER,
        .usage  = descriptor->vertex_usage
    }, initial_vertex_data);
    mesh._i_data = vx_glbuffer_new(&(vx_GlBufferDescriptor){
        .type   = VX_GL_INDEX_BUFFER,
        .usage  = descriptor->index_usage
    }, initial_index_data);

    if (initial_transform != NULL) {
        glm_mat4_copy((f32 (*)[4])(*initial_transform), mesh.transform);
    } else {
        glm_mat4_identity(mesh.transform);
    }

    mesh.model_uniform_name = descriptor->model_uniform_name;

    return mesh;
}

void vx_mesh_bind(const vx_Mesh* mesh, vx_GlProgram* program) {
    VX_NULL_ASSERT(mesh);
    VX_NULL_ASSERT(program);

    vx_glbuffer_bind(&mesh->_v_data);
    vx_glbuffer_bind(&mesh->_i_data);
    vx_glprogram_uniform_mat4(program, mesh->model_uniform_name, mesh->transform);
}

void vx_mesh_free(const vx_Mesh* mesh) {
    VX_NULL_ASSERT(mesh);

    vx_glbuffer_free(&mesh->_v_data);
    vx_glbuffer_free(&mesh->_i_data);
}

void vx_mesh_translate(vx_Mesh* mesh, const vec3 translation, const f32 amount) {
    VX_NULL_ASSERT(mesh);
    VX_NULL_ASSERT(translation);

    vec3 temp;
    glm_vec3_copy((f32*)translation, temp);
    glm_vec3_scale(temp, amount, temp);

    glm_translate(mesh->transform, temp);
}

void vx_mesh_rotate(vx_Mesh* mesh, const vec3 rotation, const f32 amount) {
    VX_NULL_ASSERT(mesh);
    VX_NULL_ASSERT(rotation);

    glm_rotate(mesh->transform, amount, (f32*)rotation);
}

void vx_mesh_scale(vx_Mesh* mesh, const vec3 scaling, const f32 amount) {
    VX_NULL_ASSERT(mesh);
    VX_NULL_ASSERT(scaling);

    vec3 temp;
    glm_vec3_copy((f32*)scaling, temp);
    glm_vec3_scale(temp, amount, temp);
    glm_scale(mesh->transform, temp);
}
