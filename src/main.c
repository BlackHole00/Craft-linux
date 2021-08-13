#include "gfx/gfx.h"
#include "os/os.h"
#include <stdio.h>

typedef struct {
    vx_GlProgram program;
    vx_GlBuffer  buffer;
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

    state->buffer = vx_glbuffer_new(&(vx_GlBufferDescriptor){
        .type = VX_GL_VERTEX_BUFFER,
        .usage = VX_GL_STATIC_DRAW
    });

    float data[] = {
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
    vx_glbuffer_data(&state->buffer, data, VX_SIZE_OF_CVECTOR(data));
    vx_glbuffer_unbind(VX_GL_VERTEX_BUFFER);
}

void gm_logic(gm_State* state, GLFWwindow* window, f64 delta) {}

void gm_draw(const gm_State* state) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void gm_resize(gm_State* state, GLFWwindow* window, u32 width, u32 height) {}

void gm_close(gm_State* state, GLFWwindow* window) {
    vx_glprogram_free(&state->program);
    vx_glbuffer_free(&state->buffer);
}

int main(void)
{
    freopen("log.txt", "w", stdout);

    vx_glfw_init();
    vx_glfw_window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    vx_glfw_window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    vx_glfw_window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    vx_WindowDescriptor descriptor = VX_DEFAULT(vx_WindowDescriptor);
    descriptor.title    = "OpenGL";
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
