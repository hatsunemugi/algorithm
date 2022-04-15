#ifndef OPENGL_H
#define OPENGL_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
class OpenGL : public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGL(QWidget *parent = nullptr);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
signals:

};

#endif // OPENGL_H
