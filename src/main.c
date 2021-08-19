#include "gfx/gfx.h"
#include "utilis/utilis.h"
#include "os/os.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stb_image.h>
#include <cglm.h>

const u32 GM_WIDTH = 1080;
const u32 GM_HEIGHT = 800;

void gm_process_camera_input(vx_Camera*, GLFWwindow*, vx_WindowInputHelper*, f64);

typedef struct {
    vx_GlProgram    program;
    vx_GlBuffer     vbuffer;
    vx_GlBuffer     ibuffer;
    vx_GlTexture    texture;
    vx_Camera       camera;
    vx_Mesh         mesh;
} gm_State;

void gm_init(gm_State* state, GLFWwindow* window) {
    vx_GlShader vertex_shader   = vx_glshader_new(&(vx_GlShaderDescriptor){ 
        .type = VX_GL_VERTEX_SHADER,
        .shader_path = "res/shaders/basic.vs"
    });
    vx_GlShader fragment_shader = vx_glshader_new(&(vx_GlShaderDescriptor){
        .type = VX_GL_FRAGMENT_SHADER,
        .shader_path = "res/shaders/basic.fs"
    });

    f32 data[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    };
    state->vbuffer = vx_glbuffer_new(&(vx_GlBufferDescriptor){
        .type = VX_GL_VERTEX_BUFFER,
        .usage = VX_GL_STATIC_DRAW
    }, &(vx_GlBufferData){
        .data = data,
        .data_size = sizeof(data)
    });
    
    u32 indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    state->ibuffer = vx_glbuffer_new(&(vx_GlBufferDescriptor){
        .type = VX_GL_INDEX_BUFFER,
        .usage = VX_GL_STATIC_DRAW
    }, &(vx_GlBufferData){
        .data = indices,
        .data_size = sizeof(indices)
    });

    state->mesh = vx_mesh_new(&(vx_MeshDescriptor){
        .vertex_usage = VX_GL_STATIC_DRAW,
        .index_usage  = VX_GL_STATIC_DRAW,
        .model_uniform_name = VX_DEFAULT_MODEL_UNIFORM_NAME
    }, &(vx_GlBufferData){
        .data = data,
        .data_size = sizeof(data)
    }, &(vx_GlBufferData){
        .data = indices,
        .data_size = sizeof(indices)
    }, NULL);

    state->texture = vx_gltexture_from_path(&(vx_GlTextureDescriptor){
        .type = VX_GL_TEXTURE_2D,
        .format = VX_GL_RGB,
        .mag_filter = VX_GL_LINEAR,
        .min_filter = VX_GL_NEAREST,
        .texture_unit = 0,
        .warp_s = VX_GL_CLAMP_TO_BORDER,
        .warp_t = VX_GL_CLAMP_TO_BORDER
    }, "res/textures/container.jpg");

    state->program = vx_glprogram_new(&(vx_GlProgramDescriptor){
        .vertex_shader      = &vertex_shader,
        .fragment_shader    = &fragment_shader,
        .geometry_shader    = NULL,
        .compute_shader     = NULL,
        .states             = &(vx_GlProgramStates){
            .depth_test           = VX_DEPTHTEST_NONE,
            .culling.culling_face = VX_CULL_NONE,
            .blending.enabled     = false,
        },
        .layout             = &(vx_GlLayoutDescriptor){
            .element_number = 2,
            .elements = (vx_GlLayoutElement[]){
                { .count = 3, .type = VX_GL_F32, .normalized = false },
                { .count = 2, .type = VX_GL_F32, .normalized = false }
            }
        }
    });

    state->camera = vx_camera_new(&(vx_CameraDescriptor){
        .type           = VX_CAMERA_PERSPECTIVE,
        .screen_rateo   = GM_WIDTH / (f32)GM_HEIGHT,
        .far            = 100.0f,
        .near           = 0.01f,
        .p_fov          = 100.0f,
        .limit_rotation = true,
        .position       = { -0.0f, -0.0f, -5.0f },
        .rotation       = { 90.0f,  0.0f,  0.0f },
        .view_uniform_name = VX_DEFAULT_VIEW_UNIFORM_NAME,
        .proj_uniform_name = VX_DEFAULT_PROJ_UNIFORM_NAME
    });
}

void gm_logic(gm_State* state, GLFWwindow* window, vx_WindowInputHelper* input, f64 delta) {
    if (input->keys[GLFW_KEY_ESCAPE].just_released) {
        glfwSetWindowShouldClose(window, 1);
    }

    gm_process_camera_input(&state->camera, window, input, delta);
}

void gm_draw(gm_State* state) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vx_glprogram_bind(&state->program);
    //vx_glbuffer_bind(&state->vbuffer);
    //vx_glbuffer_bind(&state->ibuffer);
    vx_gltexture_bind(&state->texture);
    vx_mesh_bind(&state->mesh, &state->program);

    vx_camera_bind(&state->camera, &state->program);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    VX_GL_CHECK_ERRORS()
}

void gm_resize(gm_State* state, GLFWwindow* window, u32 width, u32 height) {
    glViewport(0, 0, width, height);
    vx_camera_set_screen_rateo(&state->camera, width / (f32)height);
}

void gm_close(gm_State* state, GLFWwindow* window) {
    vx_glprogram_free(&state->program);
    vx_glbuffer_free(&state->vbuffer);
    vx_glbuffer_free(&state->ibuffer);
    vx_gltexture_free(&state->texture);
    vx_mesh_free(&state->mesh);
}

void gm_process_camera_input(vx_Camera* camera, GLFWwindow* window, vx_WindowInputHelper* input, f64 delta) {
    if (input->keys[GLFW_KEY_W].pressed) {
        vx_camera_move_forward(camera, delta * 5.0f);
    } else if (input->keys[GLFW_KEY_S].pressed) {
        vx_camera_move_backward(camera, delta * 5.0f);
    }
    if (input->keys[GLFW_KEY_D].pressed) {
        vx_camera_move_right(camera, delta * 5.0f);
    } else if (input->keys[GLFW_KEY_A].pressed) {
        vx_camera_move_left(camera, delta * 5.0f);
    }
    if (input->keys[GLFW_KEY_SPACE].pressed) {
        camera->position[VX_Y] -= delta * 5.0f;
    } else if (input->keys[GLFW_KEY_LEFT_SHIFT].pressed) {
        camera->position[VX_Y] += delta * 5.0f;
    }

    if (input->keys[GLFW_KEY_LEFT].pressed) {
        vx_camera_rotate_x(camera,  delta * 75.0f);
    } else if (input->keys[GLFW_KEY_RIGHT].pressed) {
        vx_camera_rotate_x(camera, -delta * 75.0f);
    }
    if (input->keys[GLFW_KEY_DOWN].pressed) {
        vx_camera_rotate_y(camera, -delta * 75.0f);
    } else if (input->keys[GLFW_KEY_UP].pressed) {
        vx_camera_rotate_y(camera,  delta * 75.0f);
    }

    if (input->mouse.grabbed) {
        vx_camera_rotate_x(camera, input->mouse.offset_x * 0.25f);
        vx_camera_rotate_y(camera, -input->mouse.offset_y * 0.25f);
    }
}

int main(void)
{
#ifdef _RELEASE
    freopen("log.txt", "w", stdout);
#endif

    vx_WindowDescriptor descriptor = VX_DEFAULT(vx_WindowDescriptor);
    descriptor.title        = "OpenGL";
    descriptor.transparent_framebuffer = true;
    descriptor.resizable    = true;
    descriptor.grab_cursor  = false;
    descriptor.width        = GM_WIDTH;
    descriptor.height       = GM_HEIGHT;
    descriptor.init         = (vx_Callback)gm_init;
    descriptor.logic        = (vx_Callback)gm_logic;
    descriptor.draw         = (vx_Callback)gm_draw;
    descriptor.resize       = (vx_Callback)gm_resize;
    descriptor.close        = (vx_Callback)gm_close;

    vx_Window window = vx_window_new(&descriptor);

    gm_State state;
    vx_window_run(&window, (vx_UserStatePtr)&state);

    vx_glfw_terminate();
    return 0;
}
