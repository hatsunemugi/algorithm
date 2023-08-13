#ifndef EXTRA_HPP
#define EXTRA_HPP

#include <QCoreApplication>
#include <QEventLoop>
#include <QTime>
#include <QTimer>

static void sleep(unsigned int ms)
{
    QTime Timer = QTime::currentTime().addMSecs(ms);
    while( QTime::currentTime() < Timer )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

static void delay(unsigned int ms)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(ms, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}
static QString operator + (const QString& s,const int& i)
{
    return s + QString::number(i);
}
static QString operator + (const int& i, const QString& s)
{
    return s + QString::number(i);
}
#endif // EXTRA_HPP
