#ifndef SLIDEPAGE_H
#define SLIDEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "customScrollContainer.h"
#include "customWidgets.h"

class SheildLayer : public QWidget{
    Q_OBJECT

private:
    bool pressed = false;
    bool enabled = true;
    QWidget *bg;
    void mousePressEvent(QMouseEvent *event){if(enabled)pressed = true;}
    void mouseReleaseEvent(QMouseEvent *event){if(enabled && pressed)emit clicked();pressed = false;}
    void resizeEvent(QResizeEvent *event){bg->resize(this->parentWidget()->size());}
public:
    SheildLayer(QWidget *parent = nullptr) : QWidget(parent){
        bg = new QWidget(this);
        bg->resize(parent->size());
        bg->setStyleSheet("background-color:#5a000000;border-radius:20px");
        bg->setAttribute(Qt::WA_TransparentForMouseEvents);
        bg->show();
    }
    void setEnabled(bool e){enabled = e;}
signals:
    void clicked();
};

class SlidePage : public QWidget
{
    Q_OBJECT
private:
    int cornerRadius;
    QString pageName;
    QQueue<QWidget*> buffer;
    ScrollAreaCustom *pageContentContainer;
    QLabel *nameLabel;
    customIcon *backIcon;
    SheildLayer *sheildLayer;
    QWidget *bgWidget,*mainparent;
    QFont textFont = QFont("Corbel Light", 24);

    bool onShown = false;
    QParallelAnimationGroup *curAni = nullptr;
    QGraphicsOpacityEffect *opacity;

    void resizeEvent(QResizeEvent *event);

public:
    const int preferWidth = 350;
    explicit SlidePage(int radius, QString name, QWidget *parent = nullptr);
    SlidePage(SlidePage*&);
    void SetRadius(int radius);
    void SetName(QString name);
    void InsertContent(QWidget* widget,int n=0);
    void AddContent(QWidget* widget);
    void AddContents(QVector<QWidget*> widgets);
    void RemoveContents(QVector<QWidget*> widgets);
    void UpdateContents();
    void ScrollToTop();

signals:
    void sizeChange();

public slots:
    void slideIn();
    void slideOut();

};

#endif // SLIDEPAGE_H
