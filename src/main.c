//#include <stdio.h>
//#include "temp.h"

/*typedef struct {
    vx_GlProgram program;
} State;

void init(State* state, GLFWwindow* window) {
    //vx_GlShader vertex_shader = vx_glshader_new(GL_VERTEX_SHADER, "res/shaders/basic.vs");
    //vx_GlShader fragment_shader = vx_glshader_new(GL_FRAGMENT_SHADER, "res/shaders/basic.fs");

    //state->program = vx_glprogram_new_d(
    //    &fragment_shader,
    //    &fragment_shader,
    //    NULL,
    //    NULL
    //);
}

void logic(State* state, GLFWwindow* window, f64 delta) {
}

void draw(const State* state) {
    /*glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);*
}

void close(State* state, GLFWwindow* window) {
    vx_glprogram_free(&state->program);
}*/

/*int main() {
    fake_main();

    return 0;*/

    /*freopen("log.txt", "w", stdout);

    printf("aaa");

    vx_glfw_init();
    vx_glfw_window_hint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    vx_glfw_window_hint(GLFW_CONTEXT_VERSION_MINOR, 6);
    vx_glfw_window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    vx_WindowDescriptor descriptor = VX_DEFAULT(vx_WindowDescriptor);
    descriptor.title    = "OpenGL";
    descriptor.init     = (vx_Callback)init;
    descriptor.logic    = (vx_Callback)logic;
    descriptor.draw     = (vx_Callback)draw;

    vx_Window window = vx_window_new(descriptor);

    /*State state;
    vx_window_run(&window, (vx_UserStatePtr)&state);*/
//}


#include "os/os.h"
#include <stdio.h>

typedef struct {
    f32 idk;
} State;

void init(State* state, GLFWwindow* window) {
    state->idk = 0.0f;
}

void logic(State* state, GLFWwindow* window, f64 delta) {

}

void draw(const State* state) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void resize(State* state, GLFWwindow* window, u32 width, u32 height) {

}

void close(State* state, GLFWwindow* window) {

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
    descriptor.init     = (vx_Callback)init;
    descriptor.logic    = (vx_Callback)logic;
    descriptor.draw     = (vx_Callback)draw;
    descriptor.resize   = (vx_Callback)resize;
    descriptor.close    = (vx_Callback)close;

    vx_Window window = vx_window_new(descriptor);

    State state;
    vx_window_run(&window, (vx_UserStatePtr)&state);

    vx_glfw_terminate();
    return 0;
}
