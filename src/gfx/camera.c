#include "camera.h"

static const vec3 _WORLD_UP = (const vec3){ 0.0f, 1.0f, 0.0f };

static void _update_front(vx_Camera* camera) {
    if (camera->limit_rotation) {
        if (camera->rotation[VX_Y] > 89.99f) {
            camera->rotation[VX_Y] = 89.99f;
        } else if (camera->rotation[VX_Y] < -89.99f) {
            camera->rotation[VX_Y] = -89.99f;
        }

        /*  No extra-long term overflow */
        if (camera->rotation[VX_X] > 360.0f) {
            camera->rotation[VX_X] -= 360.0f;
        } else if (camera->rotation[VX_X] < -360.0f) {
            camera->rotation[VX_X] += 360.0f;
        }
    }

    /*  Calculate new camera front  */
    camera->front[VX_X] = cos(glm_rad(camera->rotation[VX_X])) * cos(glm_rad(camera->rotation[VX_Y]));
    camera->front[VX_Y] = sin(glm_rad(camera->rotation[VX_Y]));
    camera->front[VX_Z] = sin(glm_rad(camera->rotation[VX_X])) * cos(glm_rad(camera->rotation[VX_Y]));
    glm_normalize(camera->front);
}

vx_Camera vx_camera_new(const vx_CameraDescriptor* descriptor) {
    vx_Camera camera;

    camera.type         = descriptor->type;
    camera.near         = descriptor->near;
    camera.far          = descriptor->far;
    camera.screen_rateo = descriptor->screen_rateo;
    camera.limit_rotation = descriptor->limit_rotation;
    if (camera.type == VX_CAMERA_PERSPECTIVE) {
        camera.p_fov    = descriptor->p_fov;
    } else {
        camera.o_left   = descriptor->o_left;
        camera.o_right  = descriptor->o_right;
        camera.o_top    = descriptor->o_top;
        camera.o_bottom = descriptor->o_bottom;
    }

    glm_vec3_copy((f32*)descriptor->position, camera.position);
    glm_vec3_copy((f32*)descriptor->rotation, camera.rotation);

    /*  The view is always recalculated in vx_camera_bind() */
    glm_mat4_identity(camera.view);
    /*  Needed for movement */
    _update_front(&camera);
    vx_camera_update_proj_matrix(&camera);

    return camera;
}

void vx_camera_update_view_matrix(vx_Camera* camera) {
    VX_NULL_ASSERT(camera);

    _update_front(camera);

    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, (f32*)_WORLD_UP, camera->view);
    //glm_lookat(camera->position, (vec3){ 0.0f, 0.0f, 0.0f }, (f32*)_WORLD_UP, camera->view);
}

void vx_camera_update_proj_matrix(vx_Camera* camera) {
    VX_NULL_ASSERT(camera)

    if (camera->type == VX_CAMERA_PERSPECTIVE) {
        printf("persp: %f %f %f %f\n", camera->p_fov, camera->screen_rateo, camera->near, camera->far);
        glm_perspective(camera->p_fov, camera->screen_rateo, camera->near, camera->far, camera->proj);
    } else {    /*  Orthographic    */
        glm_ortho(camera->o_left, camera->o_right, camera->o_bottom, camera->o_top, camera->near, camera->far, camera->proj);
    }
}

void vx_camera_set_screen_rateo(vx_Camera* camera, f32 rateo) {
    VX_NULL_ASSERT(camera)

    camera->screen_rateo = rateo;
    vx_camera_update_proj_matrix(camera);
}

void vx_camera_bind(vx_Camera* camera, vx_GlProgram* program) {
    VX_NULL_ASSERT(camera);
    VX_NULL_ASSERT(program);

    //vx_camera_update_proj_matrix(camera);
    vx_camera_update_view_matrix(camera);
    vx_glprogram_uniform_mat4(program, _VX_CAMERA_VIEW_UNIFORM_NAME, camera->view);
    vx_glprogram_uniform_mat4(program, _VX_CAMERA_PROJ_UNIFORM_NAME, camera->proj);
}

void vx_camera_bind_glsimpleprogram(vx_Camera* camera, vx_GlSimpleProgram* program) {
    VX_NULL_ASSERT(camera);
    VX_NULL_ASSERT(program);

    vx_camera_update_view_matrix(camera);
    vx_glsimpleprogram_uniform_mat4(program, _VX_CAMERA_VIEW_UNIFORM_NAME, camera->view);
    vx_glsimpleprogram_uniform_mat4(program, _VX_CAMERA_PROJ_UNIFORM_NAME, camera->proj);
}

void vx_camera_move_forward(vx_Camera* camera, f32 amount) {
    VX_NULL_ASSERT(camera);

    vec3 temp;
    glm_vec3_scale(camera->front, amount, temp);
    glm_vec3_add(camera->position, temp, camera->position);
}

void vx_camera_move_backward(vx_Camera* camera, f32 amount) {
    vx_camera_move_forward(camera, -amount);
}
void vx_camera_move_left(vx_Camera* camera, f32 amount) {
    VX_NULL_ASSERT(camera);

    vec3 temp;
    glm_vec3_cross(camera->front, (f32*)_WORLD_UP, temp);
    glm_vec3_scale(temp, amount, temp);
    glm_vec3_add(camera->position, temp, camera->position);
}
void vx_camera_move_right(vx_Camera* camera, f32 amount) {
    vx_camera_move_left(camera, -amount);
}
void vx_camera_set_position(vx_Camera* camera, vec3 position) {
    VX_NULL_ASSERT(camera);

    glm_vec3_copy(position, camera->position);
}

void vx_camera_rotate_x(vx_Camera* camera, f32 amount) {
    VX_NULL_ASSERT(camera);

    camera->rotation[VX_X] += amount;
    vx_camera_update_view_matrix(camera);
}
void vx_camera_rotate_y(vx_Camera* camera, f32 amount) {
    VX_NULL_ASSERT(camera);

    camera->rotation[VX_Y] += amount;
    vx_camera_update_view_matrix(camera);
}
void vx_camera_rotate_z(vx_Camera* camera, f32 amount) {
    VX_UNIMPLEMENTED();
}
