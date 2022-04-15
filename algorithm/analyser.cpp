#include "analyser.h"
#include<QElapsedTimer>
#include<QDebug>
#include<QThread>
#include <QFile>
#include <QMessageBox>
#define STATE_START 1

#define STATE_IDENTIFIERorKEYWORD 2 /*   IDENTIFIER： 标识符   */

#define STATE_IDENTIFIER 3 /*   IDENTIFIER： 标识符   */

#define STATE_OPERATOR 4 /*   OPERATOR:    运算符   */

#define STATE_SEPARATOR 5  /*   SEPARATOR:   分隔符   */

#define STATE_NUMBER 6      /*   NUMBER：     数字     */

#define STATE_DONE  7       /*   DONE：       完成     */

Lexical::Lexical(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
    Operators  = {  "+","-","*","/","%","=","==","!","!=","<","<=",">",">=" };
    Separators = {  ",",";",".","\'","\"","(",")","[","]","{","}","//","/*","*/","#"};
    Keywords   = {  "include","define","auto","bool","break","case","catch","char","class",

                    "const","const_cast","continue","default","delete","do","double",

                    "dynamic_cast","else","enum","explicit","extern","false","float","for",

                    "friend","goto","if","inline","int","long","mutable","namespace","new",

                    "operator","private","protected","public","register","reinterpret_cast",

                    "return","short","signed","sizeof","static","static_cast","struct",

                    "switch","template","this","throw","true","try","typedef","typeid",

                    "typename","union","unsigned","using","virtual","void","volatile","while"};
    vt.push_back(Operators);
    vt.push_back(Separators);
    vt.push_back(Keywords);
}

void Lexical::receive(QString i)
{
    filepath=i;
}

int Lexical::isDigit(QString s)
{
    if("0"<=s&&s<="9")
        return true;
    return false;
}

int Lexical::isOperator(QString s)
{
    for(int i=0;i<vt[0].size();i++)
        if(vt[0][i]==s)
            return true;
    return false;
}

int Lexical::isSeparator(QString s)
{
    for(int i=0;i<vt[1].size();i++)
        if(vt[1][i]==s)
            return true;
    return false;
}

int Lexical::isKeyword(QString s)
{
    for(int i=0;i<vt[2].size();i++)
        if(vt[2][i]==s)
            return true;
    return false;
}


int Lexical::isLetter(QString s)
{
    if(("a"<=s&&s<="z")||("A"<=s&&s<="Z"))
        return true;
    return false;
}

void Lexical::filter()
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)||filepath.isEmpty())//只读
    {
        qDebug()<<"empty filepath!";
    }
    QTextStream readStream(&file);
    while(!readStream.atEnd())	//没有读到文件末尾
    {
        //readLine 读取一行
        line=readStream.readLine()+"$";
        analyse();
    }
}

void Lexical::analyse()
{
    for(int i=0;i<line.length();i++)
        queue.enqueue(line.mid(i,1));
    QString s,t;
    bool save;
    int Dot=0,Separator=0,Operator=0;
    state=STATE_START;
    while(!queue.isEmpty()&&state != STATE_DONE){
        t=queue.front();
        save=true;
        switch(state){
        case STATE_START:
            if(isDigit(t))
                state=STATE_NUMBER;
            else if(isLetter(t))
                state=STATE_IDENTIFIERorKEYWORD;
            else if(t==" "||t=="\t"||t=="\n")
                save=false,queue.dequeue();
            else if(isOperator(t))
                state=STATE_OPERATOR;
            else if(isSeparator(t))
                state=STATE_SEPARATOR;
            break;
        case STATE_NUMBER:
            if(isDigit(t)||(t=="."&&Dot<2))
                state=STATE_NUMBER,Dot+=1,qDebug()<<s;
            else
                cookie=state,state=STATE_DONE,save=false;
            break;
        case STATE_IDENTIFIERorKEYWORD:
            if(isDigit(t))
                state=STATE_IDENTIFIER;
            else if(isLetter(t)||t=="_")
                state=STATE_IDENTIFIERorKEYWORD;
            else
                cookie=state,state=STATE_DONE,save=false;
            break;
        case STATE_IDENTIFIER:
            if(isDigit(t)||isLetter(t)||t=="_")
                state=STATE_IDENTIFIER;
            else
                cookie=state,state=STATE_DONE,save=false;
            break;
        case STATE_SEPARATOR:
            if(isSeparator(s+t)&&Separator==0)
                state=STATE_SEPARATOR,Separator=1;
            else
                cookie=state,state=STATE_DONE,save=false;
            break;
        case STATE_OPERATOR:
            if(isOperator(t)&&Operator==0)
                state=STATE_OPERATOR,Operator=1;
            else
                cookie=state,state=STATE_DONE,save=false;
            break;
        case STATE_DONE:
            break;
        }
        if(save==true)
            s+=t,queue.dequeue();
        if(state==STATE_DONE)
            o(cookie,s),s.clear(),Dot=0,Separator=0,Operator=0,state=STATE_START;

    }
}
void Lexical::o(int cookie, QString s)
{
    QString type;
    if(cookie==STATE_NUMBER)
        type="数值";
    if(cookie==STATE_IDENTIFIER)
        type="标识符";
    if(cookie==STATE_OPERATOR)
        type="操作符";
    if(cookie==STATE_SEPARATOR){
        if(s=="/*"||s=="//"||s=="*/") type="注释";
        else type="界符";}
    if(cookie==STATE_IDENTIFIERorKEYWORD){
        if(isKeyword(s)) type="关键字";
        else type="标识符";}
    listt.push_back(type);
    listo.push_back(s);
}

void Lexical::organise()
{
    int n=listt.size();
    w.resize(n);
    for(int i=0;i<n;i++) w[i].resize(2);
    for(int i=0;i<n;i++)
        for(int j=0;j<1;j++)
            w[i][j]=listt[i],w[i][j+1]=listo[i];
}
void Lexical::run()
{
    filter();
    organise();
    emit send(w);
    listt.clear();
    listo.clear();
}

LL1::LL1(QObject *parent)
    : QObject{parent},QRunnable()
{
    setAutoDelete(false);
}

void LL1::receive(QString i)
{
    filepath=i;
}

void LL1::filter()
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)||filepath.isEmpty())//只读
    {
        qDebug()<<"empty filepath!";
    }
    QTextStream readStream(&file);
    while(!readStream.atEnd())	//没有读到文件末尾
    {
        //readLine 读取一行
        line=readStream.readLine()+"$";
    }
}

void LL1::run()
{

}
