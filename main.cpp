#include<windows.h>
#include <ctime>
#include <cmath>
#include "inc/util.h"


int main(int argc, char**argv)
{

    cout << "Dijkstra";
    GLContext::initWindow(argc, argv, "Dijkstra" ,1032, 500); //glutInit & create window
    //set callbacks
    glutDisplayFunc(util::display); //glutDisplayFunc
    glutKeyboardFunc(util::keyboard); //glutKeyboardFunc
    glutReshapeFunc(util::reshape); //glutReshapeFunc
    glutMouseFunc(util::mouse);
    glutPassiveMotionFunc(util::motion);
    glutMotionFunc(util::mouseMotion);
  //  glutIdleFunc(util::idle);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //util::initRandomLines();
    glutMainLoop();
    return 0;
}

