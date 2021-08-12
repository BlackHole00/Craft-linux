#include "shader.h"

vx_GlShader vx_glshader_new(vx_GlShader_Type gl_shader_type, char* shader_path) {
    vx_GlShader shader;

    char* shader_source = vx_filepath_get_content(shader_path);
    VX_ASSERT("Could not read shader source!", shader_source != NULL);

    shader.id = glCreateShader(gl_shader_type);
    glShaderSource(shader.id, 1, (const GLchar**)&shader_source, NULL);
    glCompileShader(shader.id);

    free(shader_source);

    {
        i32 success;
        char info_log[1024];
        glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader.id, 1024, NULL, info_log);
            printf("Could not compile shader %s:\n%s\n", shader_path, info_log);
        }
    }

    return shader;
}

void vx_glshader_free(vx_GlShader* shader) {
    glDeleteShader(shader->id);
}

vx_GlProgram vx_glprogram_new(vx_GlShader* vertex_shader, vx_GlShader* fragment_shader, vx_GlShader* geometry_shader, vx_GlShader* compute_shader) {
    VX_ASSERT("Vertex shader in program is NULL", vertex_shader != NULL);
    VX_ASSERT("Fragment shader in program is NULL", fragment_shader != NULL);

    vx_GlProgram program;
    program._uniform_locations = vx_vector_new();

    program.id = glCreateProgram();
    glAttachShader(program.id, vertex_shader->id);
    glAttachShader(program.id, fragment_shader->id);
    if (geometry_shader != NULL) { glAttachShader(program.id, geometry_shader->id); }
    if (compute_shader  != NULL) { glAttachShader(program.id, compute_shader->id);  }
    glLinkProgram(program.id);

    return program;
}

vx_GlProgram vx_glprogram_new_d(vx_GlShader* vertex_shader, vx_GlShader* fragment_shader, vx_GlShader* geometry_shader, vx_GlShader* compute_shader) {
    vx_GlProgram program = vx_glprogram_new(vertex_shader, fragment_shader, geometry_shader, compute_shader);

    /*  vx_glprogram_new() already checks is the vertex and fragment shaders are NULL   */
    vx_glshader_free(vertex_shader);
    vx_glshader_free(fragment_shader);
    if (geometry_shader != NULL) { vx_glshader_free(geometry_shader); }
    if (compute_shader  != NULL) { vx_glshader_free(compute_shader);  }

    return program;
}

void vx_glprogram_free(vx_GlProgram* shader) {
    glDeleteProgram(shader->id);

    vx_vector_free(&shader->_uniform_locations);
}

_VX_OPTION_CREATE_BODY_FOR_TYPE(vx_GlShader);
_VX_OPTION_CREATE_BODY_FOR_TYPE(vx_GlProgramUniformLocationRecord)
_VX_VECTOR_CREATE_BODY_FOR_TYPE(vx_GlProgramUniformLocationRecord)
