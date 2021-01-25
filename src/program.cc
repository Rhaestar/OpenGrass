#include "program.hh"

void link_program(GLuint* pid, GLuint* shader_id, int nb_shaders)
{
    *pid = glCreateProgram();
    GLint link_status = GL_TRUE;
    if (pid == 0) return;
    for (int i = 0; i < nb_shaders; ++i)
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

GLuint make_program(std::string& v, std::string& f, std::string tc,
    std::string te, std::string g)
{
    int nb_shaders = 2;
    GLuint pid = 0;
    char* vertex_src = (char*)malloc(v.length()*sizeof(char));
    char* fragment_src = (char*)malloc(f.length()*sizeof(char));
    v.copy(vertex_src,v.length());
    f.copy(fragment_src,f.length());

    char* tess_control_src;
    char* tess_eval_src;
    char* geometry_src;

    if (!tc.empty() && !te.empty()) // Tesselation (control+evaluation) shaders
    {
        tess_control_src = (char*)malloc(tc.length()*sizeof(char));
        tess_eval_src = (char*)malloc(te.length()*sizeof(char));
        tc.copy(tess_control_src, tc.length());
        te.copy(tess_eval_src, te.length());
        nb_shaders += 2;
    }

    if (!g.empty()) // Geometry shader
    {
        geometry_src = (char*)malloc(g.length()*sizeof(char));
        g.copy(geometry_src, g.length());
        nb_shaders++;
    }

    GLuint shader_ids[nb_shaders];

    shader_ids[0] = glCreateShader(GL_VERTEX_SHADER);
    shader_ids[1] = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader_ids[0], 1, (const GLchar**)&(vertex_src), 0);
    glShaderSource(shader_ids[1], 1, (const GLchar**)&(fragment_src), 0);

    if (nb_shaders > 2)
    {
        shader_ids[2] = glCreateShader(GL_TESS_CONTROL_SHADER);
        shader_ids[3] = glCreateShader(GL_TESS_EVALUATION_SHADER);
        //shader_ids[4] = glCreateShader(GL_GEOMETRY_SHADER);
        
        glShaderSource(shader_ids[2], 1, (const GLchar**)&(tess_control_src), 0);
        glShaderSource(shader_ids[3], 1, (const GLchar**)&(tess_eval_src), 0);
        //glShaderSource(shader_ids[4], 1, (const GLchar**)&(geometry_src), 0);

    }

    GLint compile_status = GL_TRUE;
    for (int i = 0; i < nb_shaders; ++i)
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

            if (nb_shaders > 2)
            {
                free(tess_control_src);
                free(tess_eval_src);
                //free(geometry_src);
                glDeleteShader(shader_ids[2]);
                glDeleteShader(shader_ids[3]);
                //glDeleteShader(shader_ids[4]);
            }
            return 0;
        }
    }
    free(vertex_src);
    free(fragment_src);
    if (nb_shaders > 2)
    {
        free(tess_control_src);
        free(tess_eval_src);
        //free(geometry_src);
    }
    link_program(&pid, shader_ids, nb_shaders);
    return pid;
}
