#include "emitter.hh"

float random_range(float up)
{
    return (float)(rand()) / (float)(RAND_MAX/up);
}

void Emitter::init_emitter_vxo()
{
    glGenVertexArrays(1, &vao_id); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();

    glGenBuffers(1, &vbo_id); TEST_OPENGL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id); TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, nparticles *  sizeof(glm::vec3), pos_buffer, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();

    glBindVertexArray(0); TEST_OPENGL_ERROR();

    glUseProgram(pid);
}

void Emitter::update_vbo(unsigned dt)
{
    bool need_init = false;
    if (nparticles > curr_nparticles)
    {
        if (dt > timer)
        {
            curr_nparticles += wave_size;
            curr_nparticles = std::min(curr_nparticles, nparticles);
            timer = dt - 1;
            need_init = true;
        }
        else
            timer -= dt;
    }
    float deltatime = (float)(dt) / 1000.f;

    if (need_init)
    {
        for (GLuint i = curr_nparticles - wave_size; i < curr_nparticles; ++i)
        {
            pos_buffer[i].x = 0.25f - random_range(0.5f);
            pos_buffer[i].y = random_range(0.1f);
            pos_buffer[i].z = 0.25f - random_range(0.5f);
            life_buffer[i] = 1000.f + 500.f - random_range(1000.f);
            speed_buffer[i] = 0.5f + 0.25f - random_range(0.5f);
            n_frames_dir[i] = 0;
            dir[i] = rand() % 2;
        }
    }

    for (GLuint i = 0; i < curr_nparticles; ++i)
    {
        if (dt > life_buffer[i])
        {
            pos_buffer[i].x = 0.25f - random_range(0.5f);
            pos_buffer[i].y = random_range(0.1f);
            pos_buffer[i].z = 0.25f - random_range(0.5f);
            life_buffer[i] = 1000.f + 500.f - random_range(1000.f);
            speed_buffer[i] = 0.5f + 0.25f - random_range(0.5f);
            n_frames_dir[i] = 0;
            dir[i] = rand() % 2;
        }
        else
        {
            if (++n_frames_dir[i] > rand() % 50 + 10) {
                dir[i] = (dir[i] + 1) % 2;
                n_frames_dir[i] = 0;
            }

            pos_buffer[i].x += (dir[i] == 0 ? -1 : 1) * random_range(0.4f) * deltatime;
            pos_buffer[i].x += random_range(0.5f) * deltatime * pos_buffer[i].y;
            pos_buffer[i].x -= random_range(0.5f) * deltatime * pos_buffer[i].y;
            pos_buffer[i].y += speed_buffer[i] * deltatime;
            life_buffer[i] -= dt;
        }
    }

    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id); TEST_OPENGL_ERROR();
    glBufferSubData(GL_ARRAY_BUFFER, 0, curr_nparticles * sizeof(glm::vec3), pos_buffer);
    glBindVertexArray(0); TEST_OPENGL_ERROR();
}
