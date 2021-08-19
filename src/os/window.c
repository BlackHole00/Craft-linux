#include "window.h"
#include <stdio.h>
#include <string.h>

static bool _is_glfw_initialized = false;
static struct {
    bool resized;
    u32 width;
    u32 height;
} _resize_data;
static struct {
    bool grabbed;
    bool moved;
    f64 offset_x;
    f64 offset_y;
    f64 pos_x;
    f64 pos_y;
} _mouse_data;
static vx_WindowKeyState _keys[GLFW_KEY_LAST];
static void _init_mouse_data(u32 window_width, u32 window_height, bool grab_cursor) {
    _mouse_data.grabbed     = grab_cursor;
    _mouse_data.moved       = false;
    _mouse_data.offset_x    = 0.0f;
    _mouse_data.offset_y    = 0.0f;
    _mouse_data.pos_x       = window_width / 2.0f;
    _mouse_data.pos_y       = window_height / 2.0f;      
}
static void _init_keys() {
    memset(_keys, 0, sizeof(_keys));
}
static void _internal_glfw_resize(GLFWwindow* window, i32 width, i32 height) {
    _resize_data.resized    = true;
    _resize_data.width      = (u32)width;
    _resize_data.height     = (u32)height;
}
static void _internal_glfw_mouse(GLFWwindow* window, f64 pos_x, f64 pos_y) {
    _mouse_data.moved       = true;
    _mouse_data.offset_x    = _mouse_data.pos_x - pos_x;
    _mouse_data.offset_y    = _mouse_data.pos_y - pos_y;
    _mouse_data.pos_x       = pos_x;
    _mouse_data.pos_y       = pos_y;
}
static void _internal_glfw_keys(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    if (action == GLFW_PRESS) {
        _keys[key].just_pressed = true;
        _keys[key].pressed = true;
    } else if (action == GLFW_RELEASE) {
        _keys[key].pressed = false;
        _keys[key].just_released = true;
    }
}
static void _check_resize(vx_Window* window) {
    if (_resize_data.resized) {
        window->descriptor.resize(window->user_state, window->glfw_window, _resize_data.width, _resize_data.height);
        _resize_data.resized = false;
    }
}
static void _apply_mouse_to_helper(vx_WindowInputHelper* helper) {
    helper->mouse.moved     = _mouse_data.moved;
    helper->mouse.offset_x  = (_mouse_data.moved ? _mouse_data.offset_x : 0.0f);
    helper->mouse.offset_y  = (_mouse_data.moved ? _mouse_data.offset_y : 0.0f);
    helper->mouse.pos_x     = _mouse_data.pos_x;
    helper->mouse.pos_y     = _mouse_data.pos_y;

    _mouse_data.moved       = false;
}
static void _update_keys() {
    for (u32 i = 0; i < GLFW_KEY_LAST; i++) {
        _keys[i].just_pressed = false;
        if (_keys[i].just_released == true) {
            _keys[i].just_released = false;
        }
    }
}

void vx_glfw_init() {
    if (!_is_glfw_initialized) {
        VX_ASSERT("Could not initialize glfw!", glfwInit());
        _resize_data.resized = false;
        _resize_data.width   = 0;
        _resize_data.height  = 0;

        _is_glfw_initialized = true;
    }
}

void vx_glfw_terminate() {
    if (_is_glfw_initialized) {
        glfwTerminate();
        _is_glfw_initialized = false;
    }
}

vx_Window vx_window_new(vx_WindowDescriptor* descriptor) {
    VX_NULL_ASSERT(descriptor);

    vx_Window window;

    /* Initialize the library and set glfw hints.   */
    vx_glfw_init();
    vx_glfw_window_hint(GLFW_CONTEXT_VERSION_MAJOR, descriptor->gl_version.major);
    vx_glfw_window_hint(GLFW_CONTEXT_VERSION_MINOR, descriptor->gl_version.minor);
    vx_glfw_window_hint(GLFW_OPENGL_PROFILE,        descriptor->gl_version.profile);
    vx_glfw_window_hint(GLFW_DECORATED,             descriptor->decorated);
    vx_glfw_window_hint(GLFW_RESIZABLE,             descriptor->resizable);
    vx_glfw_window_hint(GLFW_TRANSPARENT_FRAMEBUFFER, descriptor->transparent_framebuffer);

    /* Create a window */
    window.glfw_window = glfwCreateWindow(descriptor->width, descriptor->height, descriptor->title, descriptor->fullscreen ? glfwGetPrimaryMonitor(): NULL, NULL);

    /* Crash if the window is NULL */
    VX_ASSERT_EXIT_OP("Could not open glfw window!", window.glfw_window, vx_glfw_terminate());

    /*  Set window callbacks    */
    glfwSetWindowSizeCallback(window.glfw_window, _internal_glfw_resize);
    glfwSetCursorPosCallback(window.glfw_window, _internal_glfw_mouse);
    glfwSetKeyCallback(window.glfw_window, _internal_glfw_keys);

    /* Make the window's context current */
    glfwMakeContextCurrent(window.glfw_window);

    /*  Initialize GLAD so we can use modern OpenGL */
    VX_ASSERT("Could not Initialize GLAD!", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
    printf("Using opengl %s\n", glGetString(GL_VERSION));

    /* Copy the descriptor inside the struct */
    window.descriptor = *descriptor;

    /* make sure that the function pointers are not NULL. If they are NULL we can't call them. */
    window.descriptor.init   = VX_SAFE_FUNC_PTR(window.descriptor.init);
    window.descriptor.logic  = VX_SAFE_FUNC_PTR(window.descriptor.logic);
    window.descriptor.draw   = VX_SAFE_FUNC_PTR(window.descriptor.draw);
    window.descriptor.resize = VX_SAFE_FUNC_PTR(window.descriptor.resize);
    window.descriptor.close  = VX_SAFE_FUNC_PTR(window.descriptor.close);

    glfwSetInputMode(window.glfw_window, GLFW_CURSOR, descriptor->grab_cursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    _init_mouse_data(descriptor->width, descriptor->height, descriptor->grab_cursor);
    _init_keys();

    glfwSwapInterval(descriptor->swap_interval);

    return window;
}

void vx_window_run(vx_Window* self, vx_UserStatePtr user_state) {
    VX_NULL_ASSERT(self);

    vx_WindowInputHelper input_helper;
    input_helper.keys = _keys;
    input_helper.mouse.grabbed = _mouse_data.grabbed;

    f64 last_time       = glfwGetTime();
    f64 current_time    = 0.0f;
    f64 delta           = 0.0f;
    f64 counter         = 0.0f; /*  Frames per second.  */
    u64 frames          = 0;

    self->user_state = user_state;

    self->descriptor.init(self->user_state, self->glfw_window);

    while (!glfwWindowShouldClose(self->glfw_window)) {
        _check_resize(self);
        _apply_mouse_to_helper(&input_helper);

        current_time = glfwGetTime();
        delta = current_time - last_time;
        last_time = current_time;

        counter += delta;
        frames++;
        if (counter >= 1.0f) {
            char title[256];
            snprintf(title, 255, "%s (%ld fps - %4.2lf ms)", self->descriptor.title, frames, counter / (f64)frames);
            glfwSetWindowTitle(self->glfw_window, title);

            frames = 0;
            counter = 0.0f;
        }

        self->descriptor.logic(self->user_state, self->glfw_window, &input_helper, delta);
        self->descriptor.draw(self->user_state);

        _update_keys();
        glfwSwapBuffers(self->glfw_window);
        glfwPollEvents();
    }

    self->descriptor.close(self->user_state, self->glfw_window);
    glfwDestroyWindow(self->glfw_window);

    vx_glfw_terminate();
}
