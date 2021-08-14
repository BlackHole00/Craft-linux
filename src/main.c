#include "gfx/gfx.h"
#include "os/os.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
    vx_GlProgram program;
    vx_GlBuffer  buffer;
    vx_GlLayout  layout;
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

    state->program = vx_glprogram_new_d(
        &vertex_shader,
        &fragment_shader,
        NULL,
        NULL
    );
    vx_glprogram_uniform_f32(&state->program, "uAlpha", 1.0);

    float data[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    state->buffer = vx_glbuffer_new(&(vx_GlBufferDescriptor){
        .type = VX_GL_VERTEX_BUFFER,
        .usage = VX_GL_STATIC_DRAW
    }, &(vx_GlBufferData){
        .data = data,
        .data_size = sizeof(data)
    });

    state->layout = vx_gllayout_new(&(vx_GlLayoutDescriptor){
        .element_number = 2,
        .elements = (vx_GlLayoutElement[]){
            { .count = 3, .type = VX_GL_F32, .normalized = false },
            { .count = 3, .type = VX_GL_F32, .normalized = false }
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

    vx_glprogram_bind(&state->program);
    vx_glbuffer_bind(&state->buffer);
    vx_gllayout_bind(&state->layout);

    vx_glprogram_uniform_f32(&state->program, "uAlpha", (f32)sin(glfwGetTime()));

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void gm_resize(gm_State* state, GLFWwindow* window, u32 width, u32 height) {
    glViewport(0, 0, width, height);
}

void gm_close(gm_State* state, GLFWwindow* window) {
    vx_glprogram_free(&state->program);
    vx_glbuffer_free(&state->buffer);
    vx_gllayout_free(&state->layout);
}

int main(void)
{
    freopen("log.txt", "w", stdout);

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
