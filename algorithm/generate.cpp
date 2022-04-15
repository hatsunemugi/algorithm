#include "generate.h"
#include <QElapsedTimer>
#include <QTime>
#include <QDebug>
#include <QThread>
#include<QQueue>
#include <QRandomGenerator>
Generate::Generate(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
}

void Generate::receive(QString i)
{
    this->i=i;
    n=0;
    left=0;
    right=0;
}

void Generate::ability()
{
    t = new QTimer(this);
    t->start(2000);
    connect(t, &QTimer::timeout, this, [=](){emit able(abled);});
}

void Generate::filter()
{
    QString t,s;
    QQueue<QString> q;
    i+="$";
    number.setPattern("-?\\d+(.\\d)*");
    for(int i=0;i<this->i.length();i++)
        q.enqueue(this->i.mid(i,1));
    while(!q.isEmpty()){
        s=q.dequeue();
        if(s!="."&&("0">s||s>"9")){
            if(!t.isEmpty()){
                match=number.match(t);
                if(match.hasMatch()) x.push_back(match.captured(0).toDouble()),qDebug()<<t;
                t.clear();}}
        t+=s;}
    if(x.size()<3)
        return ;
    n=x[0];
    left=x[1];
    right=x[2];
}

void Generate::run()
{
    filter();    
    QElapsedTimer time;
    time.start();
    for(int i=0;i<n;i++)
    {
        if(left>right)
            break;
        a.push_back(QRandomGenerator::global()->bounded(right-left+1)+left);
        qDebug()<<a[i];
        o+=QString::number(a[i])+" ";
    }
    if(left>right){emit send(o,a);return;}
    emit send(o,a);
    o.clear();
    a.clear();
    x.clear();
}
