#include "shader.h"
#include "gl_error.h"

vx_GlShader vx_glshader_new(const vx_GlShaderDescriptor* descriptor) {
    VX_NULL_ASSERT(descriptor);
    vx_GlShader shader;

    char* shader_source = vx_filepath_get_content(descriptor->shader_path);
    VX_ASSERT("Could not read shader source!", shader_source != NULL);

    shader.id = glCreateShader(descriptor->type);
    glShaderSource(shader.id, 1, (const GLchar**)&shader_source, NULL);
    glCompileShader(shader.id);

    free(shader_source);

    i32 success;
    glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);

    if (!success) {
        char info_log[1024];
        glGetShaderInfoLog(shader.id, 1024, NULL, info_log);
        printf("Could not compile shader %s:\n%s\n", descriptor->shader_path, info_log);
    }

    VX_GL_CHECK_ERRORS()

    return shader;
}

void vx_glshader_free(const vx_GlShader* shader) {
    VX_NULL_ASSERT(shader);
    glDeleteShader(shader->id);
    VX_GL_CHECK_ERRORS()
}
