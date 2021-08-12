#include "window.h"
#include <stdio.h>

static bool is_glfw_initialized = false;

void vx_glfw_init() {
    if (!is_glfw_initialized) {
        VX_ASSERT("Could not initialize glfw!", glfwInit());
        is_glfw_initialized = true;
    }
}

void vx_glfw_terminate() {
    if (is_glfw_initialized) {
        glfwTerminate();
        is_glfw_initialized = false;
    }
}

vx_Window vx_window_new(vx_WindowDescriptor descriptor) {
    vx_Window window;

    /* Initialize the library */
    vx_glfw_init();

    printf("bbbb");

    /* Create a window */
    //window.glfw_window = glfwCreateWindow(descriptor.width, descriptor.height, descriptor.title, descriptor.fullscreen ? glfwGetPrimaryMonitor(): NULL, NULL);
    window.glfw_window = glfwCreateWindow(descriptor.width, descriptor.height, descriptor.title, NULL, NULL);

    /* Crash if the window is NULL */
    VX_ASSERT_EXIT_OP("Could not open glfw window!", window.glfw_window, vx_glfw_terminate());

    /* Make the window's context current */
    glfwMakeContextCurrent(window.glfw_window);

    /*  Initialize GLAD so we can use modern OpenGL */
    VX_ASSERT("Could not Initialize GLAD!", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
    printf("Using opengl %s\n", glGetString(GL_VERSION));

    /* Copy the descriptor inside the struct */
    window.descriptor = descriptor;

    /* make sure that the function pointers are not NULL. If they are NULL we can't call them. */
    window.descriptor.init   = VX_SAFE_FUNC_PTR(window.descriptor.init);
    window.descriptor.logic  = VX_SAFE_FUNC_PTR(window.descriptor.logic);
    window.descriptor.draw   = VX_SAFE_FUNC_PTR(window.descriptor.draw);
    window.descriptor.resize = VX_SAFE_FUNC_PTR(window.descriptor.resize);
    window.descriptor.close  = VX_SAFE_FUNC_PTR(window.descriptor.close);

    return window;
}

void vx_window_run(vx_Window* self, vx_UserStatePtr user_state) {
    self->user_state = user_state;

    self->descriptor.init(self->user_state, self->glfw_window);

    while (!glfwWindowShouldClose(self->glfw_window)) {
        self->descriptor.logic(self->user_state, self->glfw_window, 0.0f);
        self->descriptor.draw(self->user_state);

        glfwSwapBuffers(self->glfw_window);
        glfwPollEvents();
    }

    self->descriptor.close(self->user_state, self->glfw_window);

    vx_glfw_terminate();
}

int fake_main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))) {
        printf("Could not Initialize GLAD!");
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
