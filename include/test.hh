#pragma once

#include <GL/glew.h>

#define TEST_OPENGL_ERROR() \
    do { \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR!"<< __FILE__ << " " << __LINE__ << " " << err << std::endl; \
    } while (0)
