#pragma once

#include <math.h>
#include "../utilis/utilis.h"
#include "base/base.h"
#include "base/program.h"
#include <cglm.h>

typedef enum {
    VX_CAMERA_PERSPECTIVE = 0,
    VX_CAMERA_ORTHOGRAPHIC = 1
} vx_CameraType;

typedef struct {
    vx_CameraType type;

    f32 near;
    f32 far;
    bool limit_rotation;
    union {
        struct { 
            f32 p_fov; 
            f32 screen_rateo;
        };
        struct { 
            f32 o_left;
            f32 o_right;
            f32 o_top;
            f32 o_bottom;
        };
    };

    const char* view_uniform_name;
    const char* proj_uniform_name;

    vec3 position;
    vec3 rotation;
} vx_CameraDescriptor;

typedef struct {
    vx_CameraType type;

    f32 screen_rateo;
    f32 near;
    f32 far;
    bool limit_rotation;
    union {
        f32 p_fov;
        f32 o_left, o_right, o_top, o_bottom;
    };
    const char* view_uniform_name;
    const char* proj_uniform_name;

    vec3 position;
    vec3 rotation;
    vec3 front;
    
    mat4 proj;
    mat4 view;
} vx_Camera;

vx_Camera vx_camera_new(const vx_CameraDescriptor*);
void vx_camera_update_view_matrix(vx_Camera*);
void vx_camera_update_proj_matrix(vx_Camera*);
void vx_camera_set_screen_rateo(vx_Camera*, f32);
void vx_camera_bind(vx_Camera*, vx_GlProgram*);
void vx_camera_bind_glsimpleprogram(vx_Camera*, vx_GlSimpleProgram*);

void vx_camera_move_forward(vx_Camera*, f32);
void vx_camera_move_backward(vx_Camera*, f32);
void vx_camera_move_right(vx_Camera*, f32);
void vx_camera_move_left(vx_Camera*, f32);
void vx_camera_set_position(vx_Camera*, vec3);

void vx_camera_rotate_x(vx_Camera*, f32);
void vx_camera_rotate_y(vx_Camera*, f32);
void vx_camera_rotate_z(vx_Camera*, f32);
