#pragma once

#include "test.hh"
#include "program.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>
#include <cstdlib>

class Emitter
{
public:
    Emitter(GLuint npart, std::string& vertex, std::string& fragment)
        : nparticles(npart)
        , curr_nparticles(0)
    {
        pid = make_program(vertex, fragment);
        pos_buffer = new glm::vec3[nparticles];
        life_buffer = new GLuint[nparticles];
        speed_buffer = new float[nparticles];
        n_frames_dir = new int[nparticles];
        dir = new int[nparticles];
        timer = 100;
        wave_size = 2;
    }

    ~Emitter()
    {
        delete []pos_buffer;
        delete []life_buffer;
        delete []speed_buffer;
        delete []n_frames_dir;
        delete []dir;
    }

    void init_emitter_vxo();
    void update_vbo(unsigned dt);
    
    GLuint nparticles;
    GLuint curr_nparticles;
    GLuint timer;
    GLuint wave_size;
    GLuint pid;
    GLuint vao_id;
    GLuint vbo_id;
    glm::vec3* pos_buffer;
    GLuint *life_buffer;
    float* speed_buffer;
    int* n_frames_dir;
    int* dir; //0: right, 1: left
};
