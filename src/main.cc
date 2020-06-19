#include "program.hh"
#include "emitter.hh"
#include "bubbleEmitter.hh"
#include "smokeEmitter.hh"
#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

Emitter* vfx = nullptr;

void window_resize(int width, int height) {
    glViewport(0,0,width,height);TEST_OPENGL_ERROR();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    vfx->render();
    glutSwapBuffers();
}

void timer(int) {
    vfx->update_vbo(unsigned(1000/60));
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
    glutCreateWindow("Particle systems");
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
    if (argc == 2 && strcmp(argv[1], "smoke") == 0)
    {
        std::string vertex_src = load("shaders/smokeVertex.shd");
        std::string fragment_src = load("shaders/smokeFragment.shd");
        std::string bvertex_src = load("shaders/blurVertex.shd");
        std::string bfragment_src = load("shaders/blurFragment.shd");
        vfx = new SmokeEmitter(20000, vertex_src, fragment_src,
                    bvertex_src, bfragment_src);
    }
    else
    {
        std::string vertex_src = load("shaders/bubbleVertex.shd");
        std::string fragment_src = load("shaders/bubbleFragment.shd");
        vfx = new BubbleEmitter(100, vertex_src, fragment_src);
    }
    std::cout << glutGet(GLUT_WINDOW_WIDTH) << " " << glutGet(GLUT_WINDOW_HEIGHT) << "\n";
    vfx->init_emitter_vxo();
    glutMainLoop();
}
