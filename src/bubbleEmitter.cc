#include "bubbleEmitter.hh"

void BubbleEmitter::init_emitter_vxo()
{
    int max_nb_vbo = 2;
    
    GLint position_location = glGetAttribLocation(pid, "position");TEST_OPENGL_ERROR();
    GLint size_location = glGetAttribLocation(pid, "size");TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &vao_id); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();

    glGenBuffers(max_nb_vbo, vbo_id); TEST_OPENGL_ERROR();
    //POSITION
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]); TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, nparticles *  sizeof(glm::vec3), pos_buffer, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(position_location); TEST_OPENGL_ERROR();

    //SIZE
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]); TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, nparticles *  sizeof(float), size, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(size_location, 1, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(size_location); TEST_OPENGL_ERROR();

    glBindVertexArray(0); TEST_OPENGL_ERROR();

}

void BubbleEmitter::render()
{
    glUseProgram(pid); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();
    glDrawArrays(GL_POINTS, 0, curr_nparticles); TEST_OPENGL_ERROR();
    glBindVertexArray(0); TEST_OPENGL_ERROR();
}

void BubbleEmitter::update_vbo(unsigned dt)
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
            pos_buffer[i].x = random_range(-0.25, 0.25f);
            pos_buffer[i].y = 0.f;
            pos_buffer[i].z = random_range(-0.25f, 0.25f);
            life_buffer[i] = random_range(500.f, 1500.f);
            speed_buffer[i] = random_range(0.25f, 0.75f);
            n_frames_dir[i] = 0;
            dir[i] = rand() % 2;
            size[i] = random_range(16, 48);
        }
    }

    for (GLuint i = 0; i < curr_nparticles; ++i)
    {
        if (dt > life_buffer[i])
        {
            pos_buffer[i].x = random_range(-0.25, 0.25f);
            pos_buffer[i].y = 0.f;
            pos_buffer[i].z = random_range(-0.25f, 0.25f);
            life_buffer[i] = random_range(500.f, 1500.f);
            speed_buffer[i] = random_range(0.25f, 0.75f);
            n_frames_dir[i] = 0;
            dir[i] = rand() % 2;
            size[i] = random_range(16, 48);
        }
        else
        {
            if (++n_frames_dir[i] > rand() % 50 + 10) {
                dir[i] = (dir[i] + 1) % 2;
                n_frames_dir[i] = 0;
            }

            pos_buffer[i].x += (dir[i] == 0 ? -1 : 1) * random_range(0.4f) * deltatime;
            pos_buffer[i].z += (dir[i] == 0 ? -1 : 1) * random_range(0.4f) * deltatime;
            pos_buffer[i].y += speed_buffer[i] * deltatime;
            life_buffer[i] -= dt;
        }
    }

    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();

    //POSITION
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]); TEST_OPENGL_ERROR();
    glBufferSubData(GL_ARRAY_BUFFER, 0, curr_nparticles * sizeof(glm::vec3), pos_buffer); TEST_OPENGL_ERROR();

    //SIZE
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]); TEST_OPENGL_ERROR();
    glBufferSubData(GL_ARRAY_BUFFER, 0, curr_nparticles * sizeof(float), size); TEST_OPENGL_ERROR();

    glBindVertexArray(0); TEST_OPENGL_ERROR();
}
