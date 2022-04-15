#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QRunnable>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QTimeLine>
class Vertex;
class Graph : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit Graph(QObject *parent = nullptr);
    void call();
protected:
    void run() override;
signals:

};
class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);
    Vertex *addVertex(QPointF center, qreal radius);
    QVector<Vertex*> vertexes;
    //QVector<MyGraphicsLineItem*> lines;
private:
    int vertexID = 0;
protected :
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent (QKeyEvent *event);
signals :
    void onMouseMove(QPoint point);
    void onLeftClick(QPoint point);
    void onRightClick(QPoint point);
    void onDoubleClick(QPoint point); //双击事件
    void onMouseReleas(QPoint point);
    void keyPress(QKeyEvent *event);
    void vexAdded(Vertex *vertex);
};
class Vertex :public QObject,public QGraphicsEllipseItem
{
    Q_OBJECT
private:
    QBrush regBrush = QBrush(QColor(58, 143, 192));
    QBrush selBrush = QBrush(QColor(208, 90, 110));
    QBrush visitedBrush = QBrush(QColor(0, 137, 108));
    QBrush accessBrush = QBrush(QColor(152, 109, 178));
    QGraphicsSimpleTextItem *nameTag = nullptr;
    QString nameText = "";
    QFont nameFont = QFont("Corbel", 13, QFont::Normal, true);
    QGraphicsSimpleTextItem *tag = nullptr;
    QString hintText = "";
    QFont hintFont = QFont("Corbel", 12);
    QPointF center;
    qreal radius;
    int state;
public:
    int id;
    explicit Vertex(QPointF _center, qreal _r, int nameID = 0, QGraphicsItem *parent = nullptr);
    void setText(const QString & text){nameTag->setText(text);nameText = text;}
    void connection(View*);
signals:
    void setHover(bool in = true);
    void selected(QGraphicsItem *sel);
    void lineFrom(Vertex *start);
    void menuStateChanged(QGraphicsItem *item, bool display = true);
    //void logAdded(viewLog *log);
    void removed(Vertex *vex);
    void addAnimation(QTimeLine *ani);

public slots:
    void onMouseMove(QPointF position);
    void onLeftClick(QPointF position);
    void onRightClick(QPointF position);
    void onMouseRelease();
};
#endif // GRAPH_H
