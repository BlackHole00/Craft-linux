#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../utilis/utilis.h"

typedef void* vx_UserStatePtr;

typedef struct {
    char* title;
    i32 width;
    i32 height;
    bool fullscreen;

    VX_CALLBACK(vx_UserStatePtr, GLFWwindow*) init;
    VX_CALLBACK(vx_UserStatePtr, GLFWwindow*, f64) logic;
    VX_CALLBACK(const vx_UserStatePtr) draw;
    VX_CALLBACK(vx_UserStatePtr, GLFWwindow*, u32, u32) resize;
    VX_CALLBACK(vx_UserStatePtr, GLFWwindow*) close;
} vx_WindowDescriptor;

typedef struct {
    vx_UserStatePtr user_state;
    GLFWwindow* glfw_window;
    vx_WindowDescriptor descriptor;
} vx_Window;

VX_CREATE_DEFAULT(vx_WindowDescriptor,
    .title = "Window",
    .width = 640,
    .height = 480,
    .fullscreen = 0,
    .init = NULL,
    .logic = NULL,
    .draw = NULL,
    .resize = NULL,
    .close = NULL
)

void vx_glfw_init();
void vx_glfw_terminate();
#define vx_glfw_window_hint(_HINT, _VALUE) glfwWindowHint((_HINT), (_VALUE));

vx_Window vx_window_new(vx_WindowDescriptor*);
void vx_window_run(vx_Window*, vx_UserStatePtr);

int fake_main();
