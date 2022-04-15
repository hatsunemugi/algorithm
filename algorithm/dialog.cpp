#include "dialog.h"
#include "ui_dialog.h"
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QGraphicsItem>
//#include <QGraphicsRectitem>
void Dialog::iniGraphicsSystem()
{
    QRectF rect (-200,-100,400,200); //左上角坐标，宽度，高度
    scene=new QGraphicsScene(rect); //sc ene 逻辑坐标系定义
    ui->view->setScene(scene);
    //画一个矩形框,大小等于scene
    QGraphicsRectItem *item=new QGraphicsRectItem(rect);
    item->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable); //设置 flags
    QPen pen ;
    pen.setWidth(2) ;
    item->setPen(pen) ;
    scene->addItem(item);
    //画一个位于scene中心的椭圆,测试局部坐标
    QGraphicsEllipseItem *item2=new QGraphicsEllipseItem(-100,-50,200,100);
    item2->setPos(0,0);
    item2->setBrush(QBrush(Qt::blue));
    item2->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
    scene->addItem(item2);
    //画一个圆,中心位于scene的边缘
    QGraphicsEllipseItem *item3=new QGraphicsEllipseItem(-50,-50,100,100);
    item3->setPos(rect.right(),rect.bottom());
    item3->setBrush(QBrush(Qt::red));
    item3->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem :: ItemIsSelectable
    | QGraphicsItem :: ItemIsFocusable) ;
    scene->addItem(item3);
    scene->clearSelection();
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    labViewCord=new QLabel("View 坐标: ");
    labViewCord->setMinimumWidth(150) ;
    ui->VL->addWidget(labViewCord) ;
    labSceneCord=new QLabel("Scene 坐标: ");
    labSceneCord->setMinimumWidth (150) ;
    ui->VL->addWidget(labSceneCord) ;
    labItemCord=new QLabel("Item 坐标: ");
    labItemCord->setMinimumWidth (150) ;
    ui->VL->addWidget(labItemCord);
    labItemInfo=new QLabel("Iteminfo : ");
    labItemInfo->setMinimumWidth(200);
    ui->VL->addWidget(labItemInfo) ;
    scene=new QGraphicsScene(-300,-200,600,200); //创建 QGraphicsScene
    ui->view->setScene(scene) ; // view 关联
    ui->view->setCursor(Qt::CrossCursor); // 设置鼠标光标形状
    ui->view->setMouseTracking(true),
    ui->view->setDragMode(QGraphicsView::RubberBandDrag);
    //this->setCentralWidget(ui->view);
    ui->view->setCursor(Qt::CrossCursor);
    ui->view->setMouseTracking(true);
    ui->view->setDragMode(QGraphicsView::RubberBandDrag);
    QObject::connect(ui->view,SIGNAL(mouseMovePoint(QPoint)),
    this,SLOT(on_mouseMovePoint(QPoint)));
    QObject::connect(ui->view,SIGNAL(mouseClicked (QPoint)),
    this, SLOT(on_mouseClicked(QPoint)));
    iniGraphicsSystem();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    int w=this->width();
    int h=this->height();
    rect=new QRect(0,0,w,h);
    pen=new QPen;
    pen->setWidth(4);
    pen->setColor(Qt::gray);
    pen->setStyle(Qt::SolidLine);
    pen->setCapStyle(Qt::FlatCap);
    pen->setJoinStyle(Qt::BevelJoin);
    painter->setPen(*pen);
    brush=new QBrush;
    brush->setColor(Qt::white);// 画刷颜色
    brush->setStyle(Qt::SolidPattern);//填充样式
    painter->setBrush(*brush);
    painter->drawRoundedRect(*rect,10,10);
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    ui->labViewSize->setText((QString::asprintf("Graphics View 坐标: 左上角总是(0,0),宽度=%d,高度=%d",ui->view->width(),ui->view->height())));
    QRectF rectF=ui->view->sceneRect(); //Scene 的矩形区
    ui->labSceneRect->setText((QString::asprintf("QGraphicsView::sceneRect=(Left,Top,Width,Height)=%.0f,%.0f,%.0f,%.0f",rectF.left(),rectF.top(),rectF.width(),rectF.height())));
}

void Dialog::on_mouseMovePoint(QPoint point)
{
    labViewCord->setText(QString::asprintf("View 坐标: %d,%d",point.x(),point.y()));
    QPointF pointScene=ui->view->mapToScene(point); //转换到Scene坐标
    labSceneCord->setText(QString::asprintf("Scene 坐标: %.0f,%.0f",pointScene.x(),pointScene.y()));
}

void Dialog::on_mouseClicked(QPoint point)
{
    QPointF pointScene=ui->view->mapToScene(point); // 换到 Scene 坐标
    QGraphicsItem *item=NULL;
    item=scene->itemAt(pointScene,ui->view->transform()); //获取光标下的图形项
    if(item!=NULL) //有图形项
    {
        QPointF pointItem=item->mapFromScene(pointScene); // 形项局部坐标
        labItemCord->setText(QString::asprintf("Item 坐标: %.0f%.0f",pointItem.x(),pointItem.y()));
    }

}

void Dialog::on_mouseDoubleClick(QPoint point)
{

}

void Dialog::on_keyPress(QKeyEvent *event)
{

}
