#pragma once

#include "test.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>

void link_program(GLuint* pid, GLuint* shader_id);

GLuint make_program(std::string& vertex, std::string fragment);
