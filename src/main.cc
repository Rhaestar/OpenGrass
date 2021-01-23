#include "program.hh"
#include "plane.hh"
#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

Plane *plane;

void window_resize(int width, int height) {
    glViewport(0,0,width,height);TEST_OPENGL_ERROR();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
//  RENDER HERE
    plane->render();
    glutSwapBuffers();
}

void timer(int) {
// UPDATEHERE (IF NEEDED)
// vfx->update_vbo(unsigned(1000/60));
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
}

void init_glut(int &argc, char *argv[]) {
    //glewExperimental = GL_TRUE;
    glutInit(&argc, argv);
    glutInitContextVersion(4,5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition ( 100, 100 );
    glutCreateWindow("Grassy grass");
    glutDisplayFunc(display);
    glutTimerFunc(1000/60, timer, 0);
    glutReshapeFunc(window_resize);
}

bool init_glew() {
    if (glewInit()) {
        std::cerr << " Error while initializing glew";
        return false;
    }
    return true;
}

void init_GL() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);TEST_OPENGL_ERROR();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.4,0.4,0.4,1.0);TEST_OPENGL_ERROR();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
}

std::string load(const std::string &filename) {
    std::ifstream input_src_file(filename, std::ios::in);
    std::string ligne;
    std::string file_content="";
    if (input_src_file.fail()) {
        std::cerr << "FAIL\n";
        return "";
    }
    while(getline(input_src_file, ligne)) {
        file_content = file_content + ligne + "\n";
    }
    file_content += '\0';
    input_src_file.close();
    return file_content;
}


int main(int argc, char* argv[])
{
    srand(unsigned(time(0)));
    init_glut(argc, argv);
    init_glew();
    init_GL();
    std::string vertex_src = load("shaders/Vertex.shd");
    std::string fragment_src = load("shaders/Fragment.shd");
    std::string grass_vertex_src = "";
    std::string grass_fragment_src = "";
    std::string grass_tessel_src = "";
    std::string grass_geo_src = "";
    std::string texture_path = "textures/grass32.png";

    plane = new Plane{1, vertex_src, fragment_src, grass_vertex_src,
            grass_fragment_src, grass_tessel_src, grass_geo_src, texture_path};

    plane->init_vao();

    std::cout << glutGet(GLUT_WINDOW_WIDTH) << " " << glutGet(GLUT_WINDOW_HEIGHT) << "\n";
    glutMainLoop();
}
