#include "plane.hh"

Plane::Plane(float scale_, std::string& vertex, std::string& fragment,
        std::string& grassVertex, std::string grassFragment,
        std::string& tesselation, std::string& geometry,
        std::string& texture)
    : scale(scale_)
{
    plane_pid = make_program(vertex, fragment);
    //    grass_pid = make_program(grassVertex, grassFragment);
    //
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, texture);
    std::cout << width << " " << height << "\n";
    std::cout << (int)*(&image[0]) << "\n";
    std::cout << (int)*(&image[1]) << "\n";
    std::cout << (int)*(&image[2]) << "\n";

    //unsigned char *texture_data = stbi_load(texture.c_str(), &width, &height, &channels, 0);

    glGenTextures(1, &texture_id); TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]); TEST_OPENGL_ERROR();


    //stbi_image_free(texture_data);
}

void render();
void init_vao();

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

void Plane::render()
{
    //Draw plane
    glUseProgram(plane_pid); TEST_OPENGL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture_id); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();
    glDrawArrays(GL_TRIANGLES, 0, 6); TEST_OPENGL_ERROR();

    //Draw grass
    //glUseProgram(grass_pid); TEST_OPENGL_ERROR();
    //glDrawArrays(GL_TRIANGLES, 0, 6); TEST_OPENGL_ERROR();

    glBindVertexArray(0); TEST_OPENGL_ERROR();

}
