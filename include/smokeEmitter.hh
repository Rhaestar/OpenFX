#pragma once

#include "program.hh"
#include "test.hh"
#include "emitter.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>

class SmokeEmitter : public Emitter
{
public:
    SmokeEmitter(GLuint npart, std::string& vertex, std::string& fragment, 
            std::string& blurVertex, std::string& blurFragment)
        : Emitter(npart, vertex, fragment)
    {
        texture_pid = make_program(blurVertex, blurFragment);
        pos_buffer = new glm::vec3[nparticles];
        color_buffer = new glm::vec3[nparticles];
        life_buffer = new GLuint[nparticles];
        speed_buffer = new float[nparticles];
        n_frames_dir = new int[nparticles];
        dir = new int[nparticles];
        timer = 100;
        wave_size = 2;
    }

    ~SmokeEmitter()
    {
        delete []pos_buffer;
        delete []color_buffer;
        delete []life_buffer;
        delete []speed_buffer;
        delete []n_frames_dir;
        delete []dir;
    }

    void init_emitter_vxo() override;
    void update_vbo(unsigned dt) override;
    void render() override;
    void init_fbo();
    
    static float quadpos[12];

    GLuint timer;
    GLuint vbo_id[2];
    GLuint fbo_id;
    GLuint quad_vao_id;
    GLuint quad_vbo_id;
    GLuint texture_id;
    GLuint texture_pid;
    GLuint wave_size;
    glm::vec3* pos_buffer;
    glm::vec3* color_buffer;
    GLuint *life_buffer;
    float* speed_buffer;
    int* n_frames_dir;
    int* dir; //0: right, 1: left
};
