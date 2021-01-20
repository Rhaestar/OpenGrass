#pragma once

#include "test.hh"
#include "program.hh"

#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>

class Plane
{
public:
    Plane(float scale_, std::string& vertex, std::string& fragment,
            std::string& grassVertex, std::string grassFragment,
            std::string& tesselation, std::string& geometry,
            std::string& texture);

    void render();
    void init_vao();

private:
    float  scale;
    GLuint vbo_id;
    GLuint vao_id;
    GLuint texture_id;
    GLuint plane_pid;
    GLuint grass_pid;

};
