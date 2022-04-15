#include "backtracking.h"
#include <QElapsedTimer>
#include <QTime>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QRandomGenerator>
backtracking::Queen::Queen(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
}

void backtracking::Queen::receive(QString i)
{
    this->i=i;
}

void backtracking::Queen::backtrack(int t)
{
    if (t > n){sum++;output.enqueue(x);emit update(1);}
    else{
        for (int i=1;i<=n;i++){
            x[t] = i;//t->放置第t个皇后
            if (place(t))//第t个皇后能否放到x[t]列里
                backtrack(t+1);
        }}
}
bool backtracking::Queen::place(int k)
{
    for (int i=1;i<k;i++)
                if (x[i] == x[k] || abs(x[i] - x[k]) == abs(k-i))
                    return false;
        return true;
}
void backtracking::Queen::filter()
{
    n=i.toInt();
    x.resize(n+1);
}

void backtracking::Queen::run()
{
    filter();
    backtrack(1);
    emit send(output);
    output.clear();
    x.clear();
}

backtracking::SubGroup::SubGroup(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
    found=false;
}

void backtracking::SubGroup::receive(QString i)
{
    if(i.isEmpty())
        i="M=9,S={7,5,1,2,10}";
    this->i=i;
}

void backtracking::SubGroup::backtrack(int t)
{
    if(t==n){//到达叶子节点 ，得到了可行解********************************
        o();
        found=true;
        qDebug()<<"one";
        return;
    }
    //左子树
    if(w[t]+currc<=c){
        currc+=w[t];
        x[t]=1;
        backtrack(t+1);
        x[t]=0;
        currc-=w[t];

    }
    //右子树
    x[t]=0;
    if(currc+rest(t+1)<c) //剪枝
        return;//回溯
    else
        backtrack(t+1);

}

int backtracking::SubGroup::rest(int t)
{
    int sum=0;
    for(int i=t;i<n;i++)
        sum+=w[i];
    return sum;
}

void backtracking::SubGroup::filter()
{
    QString t,s;
    QQueue<QString> q;
    i+="$";
    number.setPattern("-?\\d+");
    for(int i=0;i<this->i.length();i++)
        q.enqueue(this->i[i]);
    while(!q.isEmpty()){
        s=q.dequeue();
        if(s!="."&&("0">s||s>"9")){
            if(!t.isEmpty()){
                match=number.match(t);
                if(match.hasMatch()) w.push_back(match.captured(0).toInt());
                t.clear();}}
        t+=s;}
    c=w[0];
    n=w.size()-1;
    x.resize(n);
    w.removeFirst();
    currc=0;
}

void backtracking::SubGroup::o()
{
    QVector<int> t(n);
    for(int i=0;i<n;i++)
            if(x[i])
                t[i]=w[i];
    output.push_back(t);
    emit update(1);
}

void backtracking::SubGroup::run()
{
    filter();
    backtrack(0);
    emit send(output);
    w.clear();
    x.clear();
    output.clear();
}
