#pragma once

#include "test.hh"
#include "program.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>

class Emitter
{
public:
    Emitter(GLuint npart, std::string& vertex, std::string& fragment) :
        nparticles(npart)
    {
        pid = make_program(vertex, fragment);
    }

    void init_emitter_vxo();

    GLuint nparticles;
    GLuint pid;
    GLuint vao_id;
};
