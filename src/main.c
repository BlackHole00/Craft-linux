#include "gfx/gfx.h"
#include "utilis/utilis.h"
#include "os/os.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stb_image.h>
#include <cglm.h>

const u32 GM_WIDTH = 640;
const u32 GM_HEIGHT = 480;

void gm_process_camera_input(vx_Camera*, GLFWwindow*, f64, f64, f64);

typedef struct {
    vx_GlProgram program;
    vx_GlBuffer  vbuffer;
    vx_GlBuffer  ibuffer;
    vx_GlTexture texture;
    vx_Camera    camera;
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
        .position       = { -0.0f, -0.0f, -1.0f },
        .rotation       = { 90.0f,  0.0f,  0.0f }
    });
}

void gm_logic(gm_State* state, GLFWwindow* window, vx_WindowInputHelper* input, f64 delta) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    gm_process_camera_input(&state->camera, window, input->mouse.offset_x, input->mouse.offset_y, delta);
}

void gm_draw(gm_State* state) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vx_glprogram_bind(&state->program);
    vx_glbuffer_bind(&state->vbuffer);
    vx_glbuffer_bind(&state->ibuffer);
    vx_gltexture_bind(&state->texture);

    vx_camera_bind(&state->camera, &state->program);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    VX_GL_CHECK_ERRORS()
}

void gm_resize(gm_State* state, GLFWwindow* window, u32 width, u32 height) {
    printf("resize\n");
    glViewport(0, 0, width, height);
}

void gm_close(gm_State* state, GLFWwindow* window) {
    vx_glprogram_free(&state->program);
    vx_glbuffer_free(&state->vbuffer);
    vx_glbuffer_free(&state->ibuffer);
    vx_gltexture_free(&state->texture);
}

void gm_process_camera_input(vx_Camera* camera, GLFWwindow* window, f64 mouse_offset_x, f64 mouse_offset_y, f64 delta) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vx_camera_move_forward(camera, delta * 0.1f);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vx_camera_move_backward(camera, delta * 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vx_camera_move_right(camera, delta * 0.1f);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vx_camera_move_left(camera, delta * 0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        vx_camera_rotate_x(camera,  delta);
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        vx_camera_rotate_x(camera, -delta);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        vx_camera_rotate_y(camera, -delta);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        vx_camera_rotate_y(camera,  delta);
    }

    vx_camera_rotate_x(camera, mouse_offset_x);
    vx_camera_rotate_y(camera, -mouse_offset_y);
}

int main(void)
{
#ifdef _RELEASE
    freopen("log.txt", "w", stdout);
#endif

    vx_WindowDescriptor descriptor = VX_DEFAULT(vx_WindowDescriptor);
    descriptor.title    = "OpenGL";
    descriptor.transparent_framebuffer = true;
    descriptor.resizable = true;
    descriptor.grab_cursor = true;
    descriptor.init     = (vx_Callback)gm_init;
    descriptor.logic    = (vx_Callback)gm_logic;
    descriptor.draw     = (vx_Callback)gm_draw;
    descriptor.resize   = (vx_Callback)gm_resize;
    descriptor.close    = (vx_Callback)gm_close;

    vx_Window window = vx_window_new(&descriptor);

    gm_State state;
    vx_window_run(&window, (vx_UserStatePtr)&state);

    vx_glfw_terminate();
    return 0;
}
