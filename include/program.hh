#pragma once

#include "test.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>

void link_program(GLuint* pid, GLuint* shader_id, int nb_shaders);

GLuint make_program(std::string& v, std::string& f, std::string tc, 
        std::string te, std::string g);
