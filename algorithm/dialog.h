#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "opengl.h"
#include "graph.h"
class QEvent;
class QResizeEvent;
class QLabel;
class QPainterPath;
class QPainter;
class QBrush;
class QRect;
class QPen;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
private:
    static const int ItemId=1; //图形项自定义数据的 key
    static const int ItemDesciption = 2 ; // 图形项自定义数据的 key
    int seqNum=0; // 用于图形项的编号,每个图形项有一个编号
    int frontZ=0; //用于 bring to front
    int backZ=0;  //用于 bring to back
    QGraphicsScene *scene;
    QLabel *labViewCord;
    QLabel *labSceneCord;
    QLabel *labItemCord;
    QLabel *labItemInfo;
    QPainterPath *path;
    QPainter *painter;
    QBrush *brush;
    QRect *rect;
    QPen *pen;
    void iniGraphicsSystem();
public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots :
    void on_mouseMovePoint(QPoint point);
    void on_mouseClicked(QPoint point);
    void on_mouseDoubleClick(QPoint point); // 鼠标双击
    void on_keyPress(QKeyEvent *event);
private:
    Ui::Dialog *ui;

};

#endif // DIALOG_H
