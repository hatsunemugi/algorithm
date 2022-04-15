#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QQueue>
#include <QGraphicsDropShadowEffect>
#include "slidepage.h"
#include "mycanvas.h"
QT_BEGIN_NAMESPACE
class Generate;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class SubWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event){mousePressed = false;if(event->globalPosition().y() < 2)  controlWindowScale();}
    void resizeEvent(QResizeEvent *event);
    void selectCanvas(MyCanvas *canvas);
    void deleteCanvas(MyCanvas *canvas);
    void controlWindowScale();
    MyCanvas* loadCanvas(const QString &path);
    void Init();
    void analyse();
    void lexical();
    void grammar();
    void divideAndConquer();
    void dynamicProgramming();
    void greedy();
    void backtracking();
    void graph();
    void maxsum();
    void queen();
    void open();
    void emitline(int);
    void subgroup();
    void grammar_ll1();
    int cornerRadius = 20,queueSize=0;
    QObject *autoinput,*autooutput,*autolog,*autobutton,*autoobject;
    QString filepath,buffer,linebuffer;
    QWidget *border = nullptr;
    QWidget *defaultPage;
    QGraphicsDropShadowEffect *windowShadow;
    QColor mainBackGround = QColor(251, 251, 251);
    QQueue<QVector<int>> *queueData;
    QLineEdit *canvasTitle = nullptr;
    QLineEdit *canvasDesc = nullptr;
    customIcon *settingsIcon = nullptr;
    customIcon *layersIcon = nullptr;
    QWidget *canvasDisplay = nullptr;

    QVector<SlidePage*> pageList;
    SlidePage *createNewPage = nullptr;
    SlidePage *defaultSettingsPage = nullptr;
    SlidePage *curSettingsPage = nullptr;
    SlidePage *layersPage = nullptr;
    singleSelectGroup *layerSel = nullptr;

    QVector<MyCanvas*> canvasList;
    MyCanvas *curCanvas = nullptr;



    enum {AT_LEFT = 1,
          AT_TOP = 2,
          AT_RIGHT = 4,
          AT_BOTTOM = 8,
          AT_TOP_LEFT = 3,
          AT_TOP_RIGHT = 6,
          AT_BOTTOM_LEFT = 9,
          AT_BOTTOM_RIGHT = 12};
    bool mousePressed = false;
    int mouseState;
    QPoint lastPos;

    Generate *generator;
    bool maximized = false;
    bool generated =false ;
    QRect lastGeometry;
signals:
    void start(QString);
    void read(QString);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SubWindow *w;
};
#endif // MAINWINDOW_H
