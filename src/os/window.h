#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../utilis/utilis.h"

typedef void* vx_UserStatePtr;

typedef enum {
    VX_GL_CORE_PROFILE = GLFW_OPENGL_CORE_PROFILE,
    VX_GL_COMPAT_PROFILE = GLFW_OPENGL_COMPAT_PROFILE
} vx_WindowGlProfile;
VX_CREATE_DEFAULT(vx_WindowGlProfile, VX_GL_CORE_PROFILE)

typedef struct {
    u8 major;
    u8 minor;
    vx_WindowGlProfile profile;
} vx_WindowGlVersion;
VX_CREATE_DEFAULT(vx_WindowGlVersion, .major = 3, .minor = 3, .profile = VX_DEFAULT(vx_WindowGlProfile))

typedef struct {
    char* title;
    i32 width;
    i32 height;
    bool fullscreen;
    bool decorated;
    bool transparent_framebuffer;
    bool resizable;

    vx_WindowGlVersion gl_version;

    VX_CALLBACK(vx_UserStatePtr, GLFWwindow*) init;
    VX_CALLBACK(vx_UserStatePtr, GLFWwindow*, f64) logic;
    VX_CALLBACK(vx_UserStatePtr) draw;
    VX_CALLBACK(vx_UserStatePtr, GLFWwindow*, u32, u32) resize;
    VX_CALLBACK(vx_UserStatePtr, GLFWwindow*) close;
} vx_WindowDescriptor;

typedef struct {
    vx_UserStatePtr user_state;
    GLFWwindow* glfw_window;
    vx_WindowDescriptor descriptor;
} vx_Window;

VX_CREATE_DEFAULT(vx_WindowDescriptor,
    .title      = "Window",
    .width      = 640,
    .height     = 480,
    .fullscreen = 0,
    .resizable  = false,
    .decorated  = true,
    .gl_version = VX_DEFAULT(vx_WindowGlVersion),
    .transparent_framebuffer = false,
    .init       = NULL,
    .logic      = NULL,
    .draw       = NULL,
    .resize     = NULL,
    .close      = NULL
)

void vx_glfw_init();
void vx_glfw_terminate();
#define vx_glfw_window_hint(_HINT, _VALUE) glfwWindowHint((_HINT), (_VALUE));

vx_Window vx_window_new(vx_WindowDescriptor*);
void vx_window_run(vx_Window*, vx_UserStatePtr);

int fake_main();
