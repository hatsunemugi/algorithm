#include "graph.h"
#include<QThread>
#include<QMouseEvent>
Graph::Graph(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);

}

void Graph::call()
{

}

void Graph::run()
{

}

View::View(QObject *parent)
    :QGraphicsView()
{

}

Vertex *View::addVertex(QPointF center, qreal radius)
{
    Vertex *vertex = new Vertex(center, radius, vertexID++);
    this->scene()->addItem(vertex);
    vertex->connection(this);
    //vertex->showAnimation();
    //vexNum++;
    vertexes.push_back(vertex);
    emit vexAdded(vertex);
    //emit logAdded(new viewLog("[Vex] | Added \""+newVex->Text()+"\""));
    return vertex;
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point=event->pos(); //QGraphicsView 的坐标
    emit onMouseMove(point); // 发射信号
    QGraphicsView::mouseMoveEvent(event);
}

void View::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
    QPoint point=event->pos(); //QGraphicsView 的坐标
    emit onLeftClick(point); // 发射信号
    }
    if (event->button()==Qt::RightButton)
    {
    QPoint point=event->pos(); //QGraphicsView 的坐标
    emit onRightClick(point); // 发射信号
    }
    QGraphicsView::mousePressEvent(event);
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
    QPoint point=event->pos(); //QGraphicsView 的坐标
    emit onDoubleClick(point); // 发射信号
    }
    QGraphicsView::mousePressEvent(event);
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
    QPoint point=event->pos(); //QGraphicsView 的坐标
    emit onLeftClick(point); // 发射信号
    }
    if (event->button()==Qt::RightButton)
    {
    QPoint point=event->pos(); //QGraphicsView 的坐标
    emit onRightClick(point); // 发射信号
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void View::keyPressEvent(QKeyEvent *event)
{
    //按键事件
    emit keyPress(event); //发射信号
    QGraphicsView::keyPressEvent(event);
}



Vertex::Vertex(QPointF _center, qreal _r, int nameID, QGraphicsItem *parent)
    :QGraphicsEllipseItem(_center.x() - 5, _center.y() - 5, 10, 10, parent),center(_center),radius(_r)
{
        nameText = QString::asprintf("V%d", nameID);
        nameTag = new QGraphicsSimpleTextItem;
        nameTag->setPos(center + QPointF(radius, - radius - QFontMetrics(nameFont).height()));
        nameTag->setFont(nameFont);
        nameTag->setText(nameText);
        nameTag->setZValue(this->zValue());
        this->setPen(Qt::NoPen);
        this->setBrush(regBrush);
}

void Vertex::connection(View *view)
{
    view->scene()->addItem(nameTag);
    connect(view, SIGNAL(mouseMoved(QPointF)), this, SLOT(onMouseMove(QPointF)));
    connect(view, SIGNAL(mouseLeftClicked(QPointF)), this, SLOT(onLeftClick(QPointF)));
    connect(view, SIGNAL(mouseRightClicked(QPointF)), this, SLOT(onRightClick(QPointF)));
    connect(view, SIGNAL(mouseReleased()), this, SLOT(onMouseRelease()));
    //connect(this, SIGNAL(setHover(bool)), view, SLOT(setHover(bool)));
    //connect(this, SIGNAL(selected(QGraphicsItem*)), view, SLOT(setSel(QGraphicsItem*)));
    //connect(this, SIGNAL(lineFrom(MyGraphicsVexItem*)), view, SLOT(startLine(Vertex*)));
    //connect(this, SIGNAL(menuStateChanged(QGraphicsItem*, bool)), view, SLOT(setMenu(QGraphicsItem*, bool)));
    //connect(this, SIGNAL(addAnimation(QTimeLine*)), view, SLOT(addAnimation(QTimeLine*)));
    //connect(this, SIGNAL(logAdded(viewLog*)), view, SLOT(addLog(viewLog*)));
    //connect(this, SIGNAL(removed(MyGraphicsVexItem*)), view, SLOT(vexRemoved(Vertex*)));
}

void Vertex::onMouseMove(QPointF position)
{

}

void Vertex::onLeftClick(QPointF position)
{

}

void Vertex::onRightClick(QPointF position)
{

}

void Vertex::onMouseRelease()
{

}
