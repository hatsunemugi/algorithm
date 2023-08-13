#ifndef PAINTEDITEM_H
#define PAINTEDITEM_H

#include <QtQml>
#include <QRectF>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QVector>
#include <QTimer>
#include <QQuickPaintedItem>

class PaintedItem : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
//    QML_EXTENDED_NAMESPACE(enums)
    Q_PROPERTY(QImage image READ getImage WRITE setImage NOTIFY imageChanged)
public:
    explicit PaintedItem(QQuickItem *parent = nullptr);
    void paint(QPainter *painter)override;
    QImage getImage(){return image;}
    void setImage(QImage image_){image=image_;emit imageChanged();}
public slots:
    void setSize(int,int);
    void setScale(double);
signals:
    void imageChanged();
    void updated();
    void aspectRatioChanged();
    void fps(int);
    void retarget(int,int);
    void another();
private:
    int width;
    int height;
    int retarget_width;
    int retarget_height;
    QTimer timer;
    double scale;
    QImage image;
    QVector<QImage> buffer;
};

inline PaintedItem::PaintedItem(QQuickItem *parent): QQuickPaintedItem{parent}
{
    connect(this,&PaintedItem::imageChanged,this,[&](){update();});
    scale=1;
}

inline void PaintedItem::paint(QPainter *painter)
{
//    qDebug()<<"paint";
    if (painter == nullptr)
    {
        qDebug()<<"nullptr";
        return;
    }
    QRectF target;
    QRectF source;

    scale=QString::number(scale, 'f', 1).toDouble();
    if(scale>1)
    {
        double origin=(1-1/scale)/2;
        target = QRectF(0, 0, width, height); //建立目标矩形，表示从0，0开始位置绘制图像
        source = QRectF(image.width()*origin, image.height()*origin, image.width()/scale, image.height()/scale); //建立源矩形，表示从图像x，y位置开始，取l大小的区域去绘制

    }
    else if(scale<1)
    {
        double origin=(1-scale)/2;
        target = QRectF(width*origin, height*origin, width*scale, height*scale); //建立目标矩形，表示从0，0开始位置绘制图像
        source = QRectF(0, 0, image.width(), image.height()); //建立源矩形，表示从图像x，y位置开始，取l大小的区域去绘制

    }
    painter->setRenderHint(QPainter::Antialiasing, true);//抗锯齿
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true );
    if(scale==1)
    {
        painter->scale((double)width/image.width(),(double)height/image.height()); painter->drawImage(0,0,image);



    }
    else
        painter->drawImage(target,image,source);
    emit updated();
}

inline void PaintedItem::setSize(int width_, int height_)
{
    width=width_;
    height=height_;

    qDebug()<<width<<height;
}

inline void PaintedItem::setScale(double scale_)
{
    scale = scale_;
}

#endif // PAINTEDITEM_H
