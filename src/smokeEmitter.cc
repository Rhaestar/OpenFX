#include "smokeEmitter.hh"

float SmokeEmitter::quadpos[12] = {1.f,  1.f,
                                  -1.f,  1.f,
                                   1.f, -1.f,
                                   1.f, -1.f,
                                  -1.f, -1.f,
                                  -1.f,  1.f,};

void SmokeEmitter::init_emitter_vxo()
{
    int max_nb_vbo = 2;

    GLint position_location = glGetAttribLocation(pid, "position");TEST_OPENGL_ERROR();
    GLint color_location = glGetAttribLocation(pid, "color");TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &vao_id); TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();

    glGenBuffers(max_nb_vbo, vbo_id); TEST_OPENGL_ERROR();
    //POSITION
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]); TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, nparticles *  sizeof(glm::vec3), pos_buffer, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(position_location); TEST_OPENGL_ERROR();

    //COLOR
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]); TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, nparticles *  sizeof(glm::vec3), color_buffer, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(color_location); TEST_OPENGL_ERROR();
    glBindVertexArray(0); TEST_OPENGL_ERROR();

    //QUAD
    glGenVertexArrays(1, &quad_vao_id); TEST_OPENGL_ERROR();
    glBindVertexArray(quad_vao_id); TEST_OPENGL_ERROR();

    glGenBuffers(1, &quad_vbo_id); TEST_OPENGL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_id); TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadpos), quadpos, GL_STATIC_DRAW); TEST_OPENGL_ERROR();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();
    glBindVertexArray(0); TEST_OPENGL_ERROR();

    init_fbo();

}

void SmokeEmitter::init_fbo()
{
    glGenTextures(1, &texture_id);  TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_id); TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); TEST_OPENGL_ERROR();
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); TEST_OPENGL_ERROR();
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); TEST_OPENGL_ERROR();
    glObjectLabel(GL_TEXTURE, texture_id, -1, "Example Texture");
    glUseProgram(texture_pid);
    glBindTexture(GL_TEXTURE_2D, 0); TEST_OPENGL_ERROR();

    glGenFramebuffers(1, &fbo_id); TEST_OPENGL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id); TEST_OPENGL_ERROR();

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0); TEST_OPENGL_ERROR();
    glDrawBuffer(GL_COLOR_ATTACHMENT0); TEST_OPENGL_ERROR();

    glBindFramebuffer(GL_FRAMEBUFFER, 0); TEST_OPENGL_ERROR();

    GLint texID = glGetUniformLocation(texture_pid , "renderedTexture" ); TEST_OPENGL_ERROR();
    glUniform1i(texID, 0); TEST_OPENGL_ERROR();
    GLint wID = glGetUniformLocation(texture_pid , "width" ); TEST_OPENGL_ERROR();
    glUniform1i(wID, glutGet(GLUT_WINDOW_WIDTH)); TEST_OPENGL_ERROR();
    GLint hID = glGetUniformLocation(texture_pid , "height" ); TEST_OPENGL_ERROR();
    glUniform1i(hID, glutGet(GLUT_WINDOW_HEIGHT)); TEST_OPENGL_ERROR();
}

void SmokeEmitter::render()
{
    //Draw into texture
    glUseProgram(pid); TEST_OPENGL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id); TEST_OPENGL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();
    glDrawArrays(GL_POINTS, 0, curr_nparticles); TEST_OPENGL_ERROR();
    glBindVertexArray(0); TEST_OPENGL_ERROR();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id); TEST_OPENGL_ERROR();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); TEST_OPENGL_ERROR();

    //glClearColor(0.4f, 0.4f, 0.4f, 1.f);
//    glBlitFramebuffer(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT),GL_COLOR_BUFFER_BIT,GL_NEAREST);


    glUseProgram(texture_pid); TEST_OPENGL_ERROR();
    glActiveTexture(GL_TEXTURE0); TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_id); TEST_OPENGL_ERROR();
    //glGenerateMipmap(GL_TEXTURE_2D); TEST_OPENGL_ERROR();

    //Draw texture

    glBindVertexArray(quad_vao_id); TEST_OPENGL_ERROR();
    glDrawArrays(GL_TRIANGLES, 0, 6); TEST_OPENGL_ERROR();
    glBindVertexArray(0); TEST_OPENGL_ERROR();
}

float color = 0.5f;
void SmokeEmitter::update_vbo(unsigned dt)
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
#pragma omp parallel for
        for (GLuint i = curr_nparticles - wave_size; i < curr_nparticles; ++i)
        {
            pos_buffer[i].x = 0.25f - random_range(0.5f);
            pos_buffer[i].y = 0.f;
            pos_buffer[i].z = 0.25f - random_range(0.5f);
            life_buffer[i] = 1000.f + 500.f - random_range(1000.f);
            speed_buffer[i] = 0.5f + 0.25f - random_range(0.5f);
            n_frames_dir[i] = 0;
            dir[i] = rand() % 2;
            color_buffer[i].x = color;
            color_buffer[i].y = color;
            color_buffer[i].z = color;
        }
    }

#pragma omp parallel for
    for (GLuint i = 0; i < curr_nparticles; ++i)
    {
        if (dt > life_buffer[i])
        {
            pos_buffer[i].x = 0.25f - random_range(0.5f);
            pos_buffer[i].y = 0.f;
            pos_buffer[i].z = 0.25f - random_range(0.5f);
            life_buffer[i] = 1000.f + 500.f - random_range(1000.f);
            speed_buffer[i] = 0.5f + 0.25f - random_range(0.5f);
            n_frames_dir[i] = 0;
            dir[i] = rand() % 2;
            color_buffer[i].x = color;
            color_buffer[i].y = color;
            color_buffer[i].z = color;
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

            float dist = pos_buffer[i].y;

            color_buffer[i].x = color;
            color_buffer[i].y = color;
            color_buffer[i].z = color;
        }
    }

    glBindVertexArray(vao_id); TEST_OPENGL_ERROR();

    //POSITION
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]); TEST_OPENGL_ERROR();
    glBufferSubData(GL_ARRAY_BUFFER, 0, curr_nparticles * sizeof(glm::vec3), pos_buffer);

    //COLOR
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]); TEST_OPENGL_ERROR();
    glBufferSubData(GL_ARRAY_BUFFER, 0, curr_nparticles * sizeof(glm::vec3), color_buffer);

    glBindVertexArray(0); TEST_OPENGL_ERROR();
}
