#include "lcs.h"
#include<QElapsedTimer>
#include<QDebug>
#include<QThread>
#include<QVector>
#include<QQueue>
lcs::lcs(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
}

void lcs::LCSLength()
{
    int i,j,m=x.size(), n=y.size();
    c.resize(m+1);b.resize(m+1);
    for(i=0; i<m+1; i++) {c[i].resize(n+1),b[i].resize(n+1);}
    for(i=0; i<=m; i++) c[i][0]=0;
    for(j=0; j<=n; j++) c[0][j]=0;
    for(i=1; i<=m; i++)
    for(j=1; j<=n; j++){
        if (x[i-1]==y[j-1]){c[i][j]= c[i-1][ j-1]+1;b[i][j]="↖";}
        else if (c[i-1][j]>= c[i][j-1]){c[i][j]= c[i-1][j];b[i][j]="↑";}
        else{c[i][j]= c[i][j-1];b[i][j]="←";}}
}

void lcs::LCS(int i, int j)
{
    if( (i==0) || (j==0) ) return ;
    if (b[i][j]=="↖")
    { LCS(i-1, j-1); output+=x[i-1];}
    else if (b[i][j]=="↑") LCS(i-1, j);
    else LCS(i, j-1);
}

void lcs::LCS()
{
    LCS(x.size(),y.size());
}
void lcs::receive(QString i)
{
    this->i=i;
}
void lcs::partition()
{
    re.setPattern("[^\\n]+");
    QRegularExpressionMatch match;
    match=re.match(i);
    if(match.hasMatch()) x=match.captured(0);
    i=i.mid(x.size(),i.size()-x.size());
    match=re.match(i);
    if(match.hasMatch()) y=match.captured(0);
}
void lcs::filter()
{
    filter(x);
    filter(y);
}

void lcs::o()
{
    p.resize(x.size()*2+2);
    for(int i=0;i<x.size()*2+2;i++)  p[i].resize(y.size()*2+2);
    for(int i=1;i<c.size()+1;i++)
        for(int j=1;j<c[0].size()+1;j++)
        {
            p[2*i-1][2*j-1]=QString::number(c[i-1][j-1]);
            if(b[i-1][j-1]=="↖") p[2*i-2][2*j-2]=b[i-1][j-1];
            if(b[i-1][j-1]=="↑") p[2*i-2][2*j-1]=b[i-1][j-1];
            if(b[i-1][j-1]=="←") p[2*i-1][2*j-2]=b[i-1][j-1];
        }
    for(int i=1;i<x.size()+1;i++) p[2*i+1][0]=x[i-1];
    for(int i=1;i<y.size()+1;i++) p[0][2*i+1]=y[i-1];
    emit send(output,p);
}
void lcs::filter(QString &p)
{
    QQueue<QString> q;
    QString t,f;
    for(int i=0;i<p.length();i++)
        q.enqueue(p[i]);
    while(!q.isEmpty())
    {
        t=q.dequeue();
        if(t!=","&&t!=" "&&t!="<"&&t!=">"&&t!="="&&t!="，"&&t!="."&&t!="。")
        {
            f=f+t;
        }
    }
    p=f;
}

void lcs::run()
{
    partition();
    filter();
    LCSLength();
    LCS();
    o();
    output.clear();
    b.clear();
    c.clear();
    p.clear();
}
