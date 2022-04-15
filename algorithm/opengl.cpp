#include "opengl.h"

OpenGL::OpenGL(QWidget *parent)
    : QOpenGLWidget{parent}
{

}

void OpenGL::initializeGL()
{
    initializeOpenGLFunctions();
}

void OpenGL::resizeGL(int w, int h)
{

}

void OpenGL::paintGL()
{
    glClearColor(0.2f,0.3f,0.4f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
