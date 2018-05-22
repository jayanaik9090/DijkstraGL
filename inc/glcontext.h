//
// Created by ghostship on 4/30/2018.
//

#ifndef DIJKSTRA_GLCONTEXT_H
#define DIJKSTRA_GLCONTEXT_H

#include <list>
#include <vector>
#include "drawables.h"

namespace GLContext {

static void initWindow(int argc, char **argv, std::string title, int w, int h,
                       int x = 0, int y = 0) {
  glutInit(&argc, argv);
  glutInitWindowSize(w, h);
  glutInitWindowPosition(688 - 516, 80);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow(title.c_str());
  glEnable(GL_DEPTH_TEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glClearColor(1, 1, 1, 0);
}

static void clear() {
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
}

static void update() { glutSwapBuffers(); }

static void post() { glutPostRedisplay(); }

static void drawNodes(std::list<Node> &v) {
  for (auto i : v) {
    i.draw();
  }
}

static void drawEdges() { Graph::draw(); }
};

#endif // DIJKSTRA_GLCONTEXT_H
