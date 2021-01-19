#include "program.hh"

void link_program(GLuint* pid, GLuint* shader_id)
{
    *pid = glCreateProgram();
    GLint link_status = GL_TRUE;
    if (pid == 0) return;
    for (int i = 0; i < 2; ++i)
        glAttachShader(*pid, shader_id[i]);
    glLinkProgram(*pid);
    glGetProgramiv(*pid, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(*pid, GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)malloc(log_size+1);
        glGetProgramInfoLog(*pid,log_size, &log_size, program_log);
        std::cerr << "Program " << program_log << "\n";
        return;
    }
}

GLuint make_program(std::string& vertex_shader_src, std::string f)
{
    GLuint pid = 0;
    char* vertex_src = (char*)malloc(vertex_shader_src.length()*sizeof(char));
    char* fragment_src = (char*)malloc(f.length()*sizeof(char));
    vertex_shader_src.copy(vertex_src,vertex_shader_src.length());
    f.copy(fragment_src,f.length());
    GLuint shader_ids[2];

    shader_ids[0] = glCreateShader(GL_VERTEX_SHADER);
    shader_ids[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_ids[0], 1, (const GLchar**)&(vertex_src), 0);
    glShaderSource(shader_ids[1], 1, (const GLchar**)&(fragment_src), 0);

    GLint compile_status = GL_TRUE;
    for (int i = 0; i < 2; ++i)
    {
        glCompileShader(shader_ids[i]);
        glGetShaderiv(shader_ids[i], GL_COMPILE_STATUS, &compile_status);
        if(compile_status != GL_TRUE)
        {
            GLint log_size;
            glGetShaderiv(shader_ids[i], GL_INFO_LOG_LENGTH, &log_size);
            char *shader_log = (char*)malloc(log_size+1);
            if (shader_log != 0)
            {
                glGetShaderInfoLog(shader_ids[i], log_size, &log_size, shader_log);
                std::cerr << "SHADER " << i << ": " << shader_log << "\n";
                free(shader_log);
            }
            free(vertex_src);
            free(fragment_src);
            glDeleteShader(shader_ids[0]);
            glDeleteShader(shader_ids[1]);
            return 0;
        }
    }
    free(vertex_src);
    free(fragment_src);
    link_program(&pid, shader_ids);
    return pid;
}
