#include "gfx/gfx.h"
#include "utilis/utilis.h"
#include "os/os.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stb_image.h>
#include <HandmadeMath.h>

typedef struct {
    vx_GlSimpleProgram sprogram;
    vx_GlProgram program;
    vx_GlBuffer  vbuffer;
    vx_GlBuffer  ibuffer;
    vx_GlLayout  layout;
    vx_GlTexture texture;
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
    VX_GL_CHECK_ERRORS()

    state->sprogram = vx_glsimpleprogram_new(&(vx_GlSimpleProgramDescriptor){
        .vertex_shader      = &vertex_shader,
        .fragment_shader    = &fragment_shader,
        .geometry_shader    = NULL,
        .compute_shader     = NULL,
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

    state->program = vx_glprogram_new_d(&(vx_GlProgramDescriptor){
        .vertex_shader      = &vertex_shader,
        .fragment_shader    = &fragment_shader,
        .geometry_shader    = NULL,
        .compute_shader     = NULL,
        .states             = &(vx_GlProgramStates){
            .depth_test           = VX_GL_LEQUAL,
            .culling.culling_face = VX_GL_BACK,
            .culling.front_face   = VX_GL_CCW,
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

    state->layout = vx_gllayout_new(&(vx_GlLayoutDescriptor){
        .element_number = 2,
        .elements = (vx_GlLayoutElement[]){
            { .count = 3, .type = VX_GL_F32, .normalized = false },
            { .count = 2, .type = VX_GL_F32, .normalized = false }
        }
    });
}

void gm_logic(gm_State* state, GLFWwindow* window, f64 delta) {
    if (glfwGetKey(window, glfwGetKeyScancode(GLFW_KEY_ESCAPE)) == GLFW_PRESS) {
        glfwWindowShouldClose(window);
    }
}

void gm_draw(gm_State* state) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vx_glsimpleprogram_bind(&state->sprogram);
    vx_glbuffer_bind(&state->vbuffer);
    vx_glbuffer_bind(&state->ibuffer);
    vx_gllayout_bind(&state->layout);
    vx_gltexture_bind(&state->texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    VX_GL_CHECK_ERRORS()
}

void gm_resize(gm_State* state, GLFWwindow* window, u32 width, u32 height) {
    glViewport(0, 0, width, height);
}

void gm_close(gm_State* state, GLFWwindow* window) {
    vx_glsimpleprogram_free(&state->sprogram);
    vx_glprogram_free(&state->program);
    vx_glbuffer_free(&state->vbuffer);
    vx_glbuffer_free(&state->ibuffer);
    vx_gllayout_free(&state->layout);
    vx_gltexture_free(&state->texture);
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
