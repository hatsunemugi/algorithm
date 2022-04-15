#include "maxsum.h"
#include<QElapsedTimer>
#include<QDebug>
#include<QThread>
#include<QVector>
#include<QQueue>
MaxSum::MaxSum(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
}

void MaxSum::receive(QString i)
{
    if(i.isEmpty())
        i="-2，11，-4，13，-5，-2";
    this->i=i;
}

void MaxSum::maxSum()
{
    int i=0;
    QVector<int> b(n);
    p.resize(n);
    for(int i=0;i<n;i++) p[i].resize(7);
    sum=0;
    besti=0,bestj=0;
    b[0]=0;
    for(int i=0;i<7;i++) p[0][i]="0";
    for (int j=1; j<n; j++){
        if (b[j-1]>0)
            b[j] =b[j-1]+ a[j];//若以a[j]为终点,左侧的最大子段和b[j-1]>0,则更新b[j]+=
        else{
            b[j]=a[j]; i=j;}//若以a[j]为终点,左侧的最大子段和b[j-1]<0,则舍去左侧数据,更新b[j]=a[j],用j更新i(即当前的besti)
        if (b[j] > sum){
            sum=b[j]; besti=i; bestj=j;}//b[j]>sum 说明暂时找到了最大子区间，更新besti，bestj
        p[j][0]=QString::number(j);p[j][1]=QString::number(b[j]);p[j][2]=QString::number(i);p[j][3]=QString::number(j);p[j][4]=QString::number(besti);p[j][5]=QString::number(bestj);p[j][6]=QString::number(sum);
    }
}

void MaxSum::filter()
{
    number.setPattern("-?\\d+(.\\d+)*");
    QString t,s;
    QQueue<QString> q;
    a.push_back(0);
    for(int i=0;i<this->i.length();i++)
        q.enqueue(this->i.mid(i,1));
    while(!q.isEmpty()){
        s=q.dequeue();
        if(s!="."&&s!="-"&&("0">s||s>"9")){
            if(!t.isEmpty()){
                QRegularExpressionMatch match=number.match(t);
                if(match.hasMatch()) a.push_back(match.captured(0).toInt());
                t.clear();}}
        t+=s;}
    if(!t.isEmpty()){
        QRegularExpressionMatch match=number.match(t);
        if(match.hasMatch()) a.push_back(match.captured(0).toInt());
        t.clear();}
    n=a.size();
    qDebug()<<n;
}

void MaxSum::o()
{
    q.resize(3);
    q[0]="sum="+QString::number(sum);
    q[1]="["+QString::number(besti)+","+QString::number(bestj)+"]";
    for(int i=besti;i<=bestj;i++)
        q[2]+=QString::number(a[i])+" ";
    a.pop_front();
}

void MaxSum::run()
{
    filter();
    maxSum();
    o();
    emit send(q,a,p);
    a.clear();
    q.clear();
    p.clear();
}
