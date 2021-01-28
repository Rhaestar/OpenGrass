#include "plane.hh"

Plane::Plane(float scale_, std::string& vertex, std::string& fragment,
        std::string& grassVertex, std::string grassFragment,
        std::string& tesselControl, std::string& tesselEval,
        std::string& geometry, std::string& texture, std::string& noise)
    : scale(scale_)
{
    plane_pid = make_program(vertex, fragment, "", "", "");
    grass_pid = make_program(grassVertex, grassFragment, tesselControl,
      tesselEval, geometry);
    
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, texture);


    glGenTextures(1, &texture_id); TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]); TEST_OPENGL_ERROR();

    std::vector<unsigned char> image2;
    error = lodepng::decode(image2, width, height, noise);


    glGenTextures(1, &noise_id); TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, noise_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]); TEST_OPENGL_ERROR();

    cameraPos = glm::vec3(0.f, 0.3f, -1.2f);
    cameraFront = glm::normalize(glm::vec3(0.f, -1.f, 10.f));
    cameraUp = glm::normalize(glm::vec3(0.f, 20.f, 2.f));
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    
    time = 0.0;
    isWindy = false;
}

void Plane::translate(unsigned char key)
{
    switch(key)
    {
    case 'z':
        cameraPos += 0.02f * cameraFront;
        break;
    case 's':
        cameraPos -= 0.02f * cameraFront;
        break;
    case 'a':
        cameraPos += 0.02f * cameraUp;
        break;
    case 'e':
        cameraPos -= 0.02f * cameraUp;
        break;
    case 'q':
        cameraPos -= 0.02f * cameraRight;
        break;
    case 'd':
        cameraPos += 0.02f * cameraRight;
        break;
    case 'w':
        isWindy = !isWindy;
        break;
    default:
        break;
    }
}

void Plane::init_vao()
{
    static float quadpos[12] = {1.0f,  1.0f,
                               -1.0f,  1.0f,
                                1.0f, -1.0f,
                                1.0f, -1.0f,
                               -1.0f, -1.0f,
                               -1.0f,  1.0f,};


    glGenVertexArrays(1, &vao_id); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();

    glGenBuffers(1, &vbo_id); TEST_OPENGL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id); TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadpos), quadpos, GL_STATIC_DRAW); TEST_OPENGL_ERROR();
    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();
    glBindVertexArray(0); TEST_OPENGL_ERROR();
}

void Plane::render(float deltatime)
{
    if (isWindy)
        time += deltatime;
    projection = glm::perspective(glm::radians(60.f), glutGet(GLUT_WINDOW_WIDTH) / (float)(glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 100.f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::normalize(cameraUp));
    //Draw plane
    glUseProgram(plane_pid); TEST_OPENGL_ERROR();

    GLint viewid = glGetUniformLocation(plane_pid, "view"); TEST_OPENGL_ERROR();
    glUniformMatrix4fv(viewid, 1, GL_FALSE, glm::value_ptr(view)); TEST_OPENGL_ERROR();
    GLint projid = glGetUniformLocation(plane_pid, "projection"); TEST_OPENGL_ERROR();
    glUniformMatrix4fv(projid, 1, GL_FALSE, glm::value_ptr(projection)); TEST_OPENGL_ERROR();

    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture_id); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 6); TEST_OPENGL_ERROR();

    //Draw grass
    glUseProgram(grass_pid); TEST_OPENGL_ERROR();

    viewid = glGetUniformLocation(grass_pid, "view"); TEST_OPENGL_ERROR();
    glUniformMatrix4fv(viewid, 1, GL_FALSE, glm::value_ptr(view)); TEST_OPENGL_ERROR();
    projid = glGetUniformLocation(grass_pid, "projection"); TEST_OPENGL_ERROR();
    glUniformMatrix4fv(projid, 1, GL_FALSE, glm::value_ptr(projection)); TEST_OPENGL_ERROR();
    GLint windid = glGetUniformLocation(grass_pid, "time"); TEST_OPENGL_ERROR();
    glUniform1f(windid, time);

    glBindTexture(GL_TEXTURE_2D, noise_id); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //A enlever, juste pour afficher les arêtes
    glDrawArrays(GL_PATCHES, 0, 6); TEST_OPENGL_ERROR();
    glPatchParameteri(GL_PATCH_VERTICES, 3); TEST_OPENGL_ERROR();

    glBindVertexArray(0); TEST_OPENGL_ERROR();

}
