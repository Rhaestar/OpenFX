#pragma once

#include "test.hh"
#include "program.hh"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>

class Emitter
{
public:
    Emitter(GLuint npart, std::string& vertex, std::string& fragment)
        : nparticles(npart)
        , curr_nparticles(0)
    {
        pid = make_program(vertex, fragment);
    }

    virtual void init_emitter_vxo() = 0;
    virtual void update_vbo(unsigned dt) = 0;

    GLuint nparticles;
    GLuint curr_nparticles;
    GLuint pid;
    GLuint vao_id;
};

float random_range(float up);

float random_range(float min, float up);
