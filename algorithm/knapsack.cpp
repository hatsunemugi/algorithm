#include "knapsack.h"
#include<QElapsedTimer>
#include<QDebug>
#include<QThread>
#include<QVector>
#include<QQueue>
KnapSack::KnapSack(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
}

void KnapSack::receive(QString i)
{
    if(i.isEmpty())
        i="C=10,W={2,2,6,5,4},V={6,3,5,4,6}";
    this->i=i;
}

void KnapSack::knapSack()
{
    int i,j;
    for ( j = 0 ; j< w[n-1]; j++)
     m[n-1][j] = 0;
    for ( j = w[n-1]; j <= C; j++)
     m[n-1][j] = v[n-1];
    for ( i = n-2 ; i > 0 ; i--)
    {
        for ( j=0 ; j < w[i]; j++)
            m[i][j] = m[i+1][j];
        for (j=w[i] ; j<=C ; j++)
            m[i][j]=m[i+1][j]>m[i+1][j-w[i]]+v[i]?m[i+1][j]:m[i+1][j-w[i]]+v[i];
    }
    if (C < w[0] ) m[0][C]=m[1][C];
    else m[0][C]=m[1][C]> m[1][C-w[0]]+v[0]?m[1][C]:m[1][C-w[0]]+v[0];
}

void KnapSack::traceBack()
{
    for ( int i = 0; i < n-1; i++)
    if (m[i][C] == m[i+1][C]) x[i] =0,qDebug()<<0;
    else x[i] =1,C=C-w[i],qDebug()<<1;
    x[n-1] = m[n-1][C] ? 1 : 0 ;qDebug()<<x[n-1];
}

void KnapSack::filter()
{
    QString t,s;
    QQueue<QString> q;
    number.setPattern("\\d+(.\\d+)*");
    for(int i=0;i<this->i.length();i++)
        q.enqueue(this->i.mid(i,1));
    while(!q.isEmpty()){
        s=q.dequeue();
        if(s!="."&&("0">s||s>"9")){
            if(!t.isEmpty()){
                match=number.match(t);
                if(match.hasMatch()) w.push_back(match.captured(0).toInt());
                t.clear();}}
        t+=s;}
    n=w.size();
    C=w[0];
    v.resize(n/2);
    for(int i=n/2+1;i<n;i++)
    {
        w[i-n/2-1]=w[i-n/2];
        v[i-n/2-1]=w[i];
    }
    w.resize(n/2);
    n=w.size();
    x.resize(n);
    m.resize(n);
    for(int i=0;i<n;i++) m[i].resize(C+1);
    qDebug()<<"end\n";
}

void KnapSack::o()
{
    p.resize(3);
    for(int i=0;i<n;i++)
    {
        p[0]+=QString::number(x[i])+" ";
        p[1]+=QString::number(w[i])+" ";
        p[2]+=QString::number(v[i])+" ";
    }
    qDebug()<<"done";
}

void KnapSack::run()
{
    filter();
    knapSack();
    traceBack();
    o();
    emit send(p,m);
    x.clear();p.clear();m.clear();w.clear();v.clear();
}
