#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QtQml>
#include <QObject>
#include "implement.h"

class Edge;

//using namespace::graph;
using std::vector;
using std::string;
class Node : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Type type READ getType WRITE setType NOTIFY typeChanged)
public:
    enum class Type : int
    {
        None = -1,
        Edge = 0,
        Vertex =1
    };
    Q_ENUM(Type)
public:
    Node():type(Type::None){}
signals:
    void typeChanged();
    void visited();
    void left();
    void position(int,int);
    void deleted();
public slots:
//    virtual void destroy(){deleteLater();}
    virtual void visit(int delay){
        emit visited();
        qDebug()<<"visit"<<toString();
        if(delay == -1)
            return;
        QTimer* timer = new QTimer;
        timer->setInterval(delay);
        connect(timer,&QTimer::timeout,this,[&,timer](){
            leave();
            disconnect(timer,0,0,0);
            timer->deleteLater();
        },Qt::QueuedConnection);
        timer->start();
    }
    void visit(){visit(-1); qDebug()<<"visit"<<toString();}
    virtual void leave(){emit left(); qDebug()<<"leave"<<toString();}
    virtual QString toString(){return "";};
    Type getType(){return type;}
    void setType(Type type_){type = type_;}
    void setPosition(int x_,int y_){x = x_;y = y_;qDebug()<<x<<y;}
public:
    Type type;
    int x;
    int y;
};

class Vertex : public Node
{
    Q_OBJECT
    QML_ELEMENT
public:
    Vertex(){ type=Node::Type::Vertex; }
    Vertex(QString);
    ~Vertex(){qDebug()<<"vertex"<<name<<"destructed";}
    friend class Context;
    static Vertex* root(Vertex* source){
        while(source->parent !=source)
            source = source->parent;
        return source;
    }
signals:

public slots:
    void visit(int);
    QString toString();
    void add(Edge*);
    void set(QString);
    QVariant get(){return name;}
public:
    QVector<Edge*> edges;
//    Vertex* root;
    Vertex* parent;
    int height = 0;
    int degree = 0;//出度
    QString name;
    bool visited;
};

class Edge : public Node
{
    Q_OBJECT
    QML_ELEMENT
public:
    Edge(){ type=Node::Type::Edge; }
    Edge(Vertex*,Vertex*,int);
    ~Edge(){qDebug()<<"edge"<<buffer<<"destructed";}
signals:
    void reversed();
public slots:
    QString toString();
    void set(Vertex*,Vertex*);
    void set(int);
    void reverse();
    bool contains(Vertex*,Vertex*);
    QVariant get(){return weight;}
public:
    Vertex* source;
    Vertex* target;
    int weight = 0;
private:
    QString buffer;
};

class Graph : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    Graph(){Context::graph = this;}
    void persist();
    void dijkstra();
    void print(QQueue<Node*>);
    static vector<string> print(vector<vector<int>>);
    static vector<string> print(vector<vector<string>>);
public slots:
    void clear();
    void load(QString);
    void save(QString);
    void exec(QVariant);
    void set(Vertex*);
    void set(Vertex*,Vertex*);
    void addVertex(Vertex*);
    void set(Vertex*,Vertex*,Edge*);
    Node* get(QString);
    QColor color(QString);
    QColor color(int);
    Vertex* guess();
signals:
    void paint(Node*);
    void sync(int);
private:
    QQueue<Node*> queue;
    static QVector<QString> names;
    bool persistent = false;
};

#endif // GRAPH_HPP
