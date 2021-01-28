#pragma once

#include "test.hh"
#include "program.hh"
#include "lodepng.hh"

#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


class Plane
{
public:
    Plane(std::string& vertex, std::string& fragment,
            std::string& grassVertex, std::string grassFragment,
            std::string& tesselControl, std::string& tesselEval,
            std::string& geometry, std::string& noise);

    void render(float deltatime);
    void init_vao();
    void translate(unsigned char key);

private:
    GLuint vbo_id;
    GLuint vao_id;
    GLuint texture_id;
    GLuint noise_id;
    GLuint plane_pid;
    GLuint grass_pid;

    glm::vec3 cameraPos;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;
    glm::vec3 cameraRight;
    glm::mat4 projection;
    glm::mat4 view;

    double time;
    bool isWindy;
    float isWave;
};
