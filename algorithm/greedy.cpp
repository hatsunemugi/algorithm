#include "greedy.h"
#include<QElapsedTimer>
#include<QDebug>
#include<QThread>
#include<QVector>
#include<QQueue>
using namespace greedy;
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
    for(int i=0;i<vw.size();i++)
    {
        if(C>=vw[index[i]][0]){
            C=C-vw[index[i]][0];x[index[i]]=1;vw[index[i]][3]=100;}
        else{
            x[index[i]]=C/vw[index[i]][0];vw[index[i]][3]=x[index[i]]*100;return ;}
    }
}
void KnapSack::filter()
{
    QString t,s;
    QQueue<QString> q;
    number.setPattern("\\d+(.\\d+)*");
    for(int i=0;i<this->i.length();i++)
        q.enqueue(this->i[i]);
    while(!q.isEmpty()){
        s=q.dequeue();
        if(s!="."&&("0">s||s>"9")){
            if(!t.isEmpty()){
                match=number.match(t);
                if(match.hasMatch()) x.push_back(match.captured(0).toInt());
                t.clear();}}
        t+=s;}
    n=x.size();
    C=x[0];
    cookie=C;
    vw.resize(n/2);
    index.resize(n/2);
    for(int i=0;i<n/2;i++)
    {
        vw[i].resize(4);
        vw[i][0]=x[i+1];//weight
        vw[i][1]=x[i+n/2+1];//value
        vw[i][2]=x[i+n/2+1]/x[i+1];//value per weight
        index[i]=i;
    }
    n=vw.size();
    x.clear();
    x.resize(n);
    qDebug()<<"end\n";
}

void KnapSack::generateIndex()
{
    int i,j,p;
     for(i=1;i<vw.size();i++)
     {
         p=index[i];
         j=i;
         while(0<j&&vw[p][2]>vw[index[j-1]][2])
         {
             index[j]=index[j-1];
             j--;
         }
         index[j]=p;
     }
}

void KnapSack::o()
{
    double value=0;
    p.resize(5);
    p[0]+="负载:"+QString::number(cookie);
    for(int i=0;i<n;i++)
    {
        if(x[i]!=1&&x[i]>0)
            p[1]+=QString::number(x[i],'f',2)+" ";
        else
            p[1]+=QString::number(x[i])+" ";
        p[2]+=QString::number(vw[i][0])+" ";
        p[3]+=QString::number(vw[i][1])+" ";
        if(x[i]>0)
        p[4]+=QString::number(x[i])+" * "+QString::number(vw[i][1])+" + ";
        value+=vw[i][1]*x[i];
    }
    p[4]=p[4].mid(0,p[4].size()-3);
    p[4]="value : "+p[4]+" = "+QString::number(value);
    qDebug()<<"done";
}

void KnapSack::run()
{
    filter();
    generateIndex();
    knapSack();
    o();
    emit send(p,vw);
    x.clear();
    vw.clear();
    p.clear();
}

Selector::Selector(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
}

void Selector::receive(QString i)
{
    if(i.isEmpty())
        i="s[i]={1,3,0,5,3,5,6,8,8,2,12},f[i]={4,5,6,9,8,7,10,11,12,13,14}";
    this->i=i;
}

void Selector::selector()
{
    sf[index[0]][2] = true;
    int j=0;
    for (int i=1; i<n; i++){
        if (sf[index[i]][0] >= sf[index[j]][1]){//开始[i]>=结束[j]
            sf[index[i]][2]=true;
            j=i;}
    else sf[index[i]][2]= false;}
}

void Selector::filter()
{
    QString t,s;
    QQueue<QString> q;
    number.setPattern("\\d+(.\\d+)*");
    for(int i=0;i<this->i.length();i++)
        q.enqueue(this->i[i]);
    while(!q.isEmpty()){
        s=q.dequeue();
        if(s!="."&&("0">s||s>"9")){
            if(!t.isEmpty()){
                match=number.match(t);
                if(match.hasMatch()) x.push_back(match.captured(0).toInt());
                t.clear();}}
        t+=s;}
    n=x.size();
    sf.resize(n/2);
    index.resize(n/2);
    for(int i=0;i<n/2;i++)
    {
        sf[i].resize(4);
        sf[i][0]=x[i];//start
        sf[i][1]=x[i+n/2];//fianl
        sf[i][2]=0;
        index[i]=i;
    }
    n=sf.size();
    x.clear();
    x.resize(n);
    qDebug()<<"end\n";
}

void Selector::generateIndex()
{
    int i,j,p;
     for(i=1;i<sf.size();i++)
     {
         p=index[i];
         j=i;
         while(0<j&&sf[p][1]<sf[index[j-1]][1])
         {
             index[j]=index[j-1];
             j--;
         }
         index[j]=p;
     }
}
void Selector::o()
{
    QString s;
    for(int i=0;i<n;i++)
    {
        if(sf[index[i]][2]==1)
            s="活动"+QString::number(index[i]+1)+":"+QString::number(sf[index[i]][0])+"-"+QString::number(sf[index[i]][1]),p.push_back(s);
        sf[i][3]=index[i]+1;
    }
}

void Selector::run()
{
    filter();
    for(int i=1;i<n;i++)
        if(sf[i-1][1]>sf[i][1])//出现递减子数组
            generateIndex();//生成下标
    selector();
    o();
    emit send(p,sf);
    x.clear();
    sf.clear();
    p.clear();
}
