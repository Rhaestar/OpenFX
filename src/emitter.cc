#include "emitter.hh"

void Emitter::init_emitter_vxo()
{
    float obj_vertices[9] = { 0.5f, 0.f, 0.f, 0.f, 0.5f, 0.f, 0.f,-0.5f, 0.f };

    glGenVertexArrays(1, &vao_id); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();

    GLuint vbo_id; TEST_OPENGL_ERROR();
    glGenBuffers(1, &vbo_id); TEST_OPENGL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id); TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), obj_vertices, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();

    glBindVertexArray(0); TEST_OPENGL_ERROR();

    glUseProgram(pid);
}


