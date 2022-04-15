#include"matrixchain.h"
#include <QElapsedTimer>
#include<QDebug>
#include<QThread>
#include<QVector>
#include<QQueue>
#include<QTextStream>
MatrixChain::MatrixChain(QObject *parent): QObject{parent},QRunnable()
{
    setAutoDelete(false);
}

void MatrixChain::receive(QString i)
{
    this->i=i;
}

void MatrixChain::filter()
{
    QString s;
    QQueue<QString> q;
    QTextStream ss(&s);
    int t;
    for(int i=0;i<this->i.length();i++)
        q.enqueue(this->i.mid(i,1));

    while(!q.isEmpty())
    {
        if("0"<=q.front()&&q.front()<="9") ss<<q.dequeue();
        else {q.dequeue();ss>>t;p.push_back(t);ss.reset();}
    }
    ss>>t;p.push_back(t);
}

void MatrixChain::matrixChain()
{
    int n=p.size()-1;
    m.resize(n);s.resize(n);q.resize(n);
    for(int i=0; i<n; i++) {m[i].resize(n),s[i].resize(n);q[i].resize(n);}
    for (int i=0; i<n; i++) m[i][i]=0; //长度为 1 的链的最小代价
    for(int r=1; r<n; r++) //长度为 r 的链的最小代价
        for( int i=0; i<n-r; i++) // n-r+1 为对角线的长度
        {
            int j=i+r; //长度为 r 的链上第一个最优值 m[i][j]的列标
            m[i][j] = m[i+1][j]+p[i]*p[i+1]*p[j+1]; //省略了 m[i][i]
            s[i][j] = i;
            for (int k=i+1; k<j; k++)
            {
                int t = m[i][k]+m[k+1][j]+ p[i]*p[i+1]*p[j+1];
                if (t<m[i][j])
                {
                    m[i][j] = t;
                    s[i][j] = k;
                }
            }
        }

}

void MatrixChain::Traceback(int i, int j)
{
    if (i==j) return;
    Traceback(i, s[i][j]);
    Traceback(s[i][j]+1, j);
    if(i==s[i][j])
    {
        if(s[i][j]+1==j)
        {
            q[i][j]="(A"+QString::number(i+1)+"*"+"A"+QString::number(j+1)+")";
        }
        else
        {
            q[i][j]="(A"+QString::number(i+1)+"*"+q[s[i][j]+1][j]+")";
        }
    }
    else
    {
        if(s[i][j]+1==j)
        {
            q[i][j]="("+q[i][s[i][j]]+"*"+"A"+QString::number(j+1)+")";
        }
        else
        {
            q[i][j]="("+q[i][s[i][j]]+"*"+q[s[i][j]+1][j]+")";
        }
    }
}
void MatrixChain::run()
{
    filter();
    matrixChain();
    Traceback(0,p.size()-2);
    emit send(q[0][p.size()-2],m,s);
}
