#include "graph.h"
#include "extra.hpp"
#include "format.h"
#include <QTextStream>

using namespace fmt;
using algorithm = Context::algorithm;
QVector<QString> Graph::names = {"鹿皮鞋","柠檬雪纺","浅青色","珊瑚","金","深橙色"};

Edge::Edge(Vertex *s, Vertex *t, int w):Edge()
{
    source = s;
    target =t;
    weight = w;
}

QString Edge::toString()
{
    buffer.clear();
    QTextStream ts(&buffer);
    ts<<(source != nullptr ? source->name : "nullptr") << "->" << weight << "->" << (target != nullptr ? target->name : "nullptr");
    return buffer;
}

void Edge::set(Vertex *source_, Vertex *target_)
{
    source = source_;
    target = target_;
}

void Edge::set(int value)
{
    weight = value;
    qDebug()<<value;
}

void Edge::reverse()
{
    Vertex* vertex = source;
    source = target;
    target = vertex;
    emit reversed();
    qDebug()<<toString();
}

bool Edge::contains(Vertex *source_, Vertex *target_)
{
    return source==source_ && target==target_;
}

Vertex::Vertex(QString value): Vertex()
{
    name = value;
    //    qDebug()<<toString();
}

void Vertex::visit(int delay)
{
    Node::visit(-1);
    visited = true;
    if(delay == -1)
        return;
    QTimer* timer = new QTimer;
    timer->setInterval(delay);
    connect(timer,&QTimer::timeout,this,[&,timer](){
            bool all_visited = true;
            for(auto e: edges)
            {
                if(e->target != this && e->target->visited == false)
                    all_visited = false;
            }
            if(all_visited){
                leave();
                disconnect(timer,0,0,0);
                timer->deleteLater();
            }
        },Qt::QueuedConnection);
    timer->start();
}

QString Vertex::toString()
{
    return name;
}

void Vertex::add(Edge *edge)
{
    edges.push_back(edge);
    if(edge->target != this)
        degree++;
}

void Vertex::set(QString value)
{
    name =value;
    qDebug()<<value;
}

void Graph::persist()
{
    persistent = !persistent;
    if(!persistent)
    {
        for(auto v: Context::table.values())
        {
            for(auto e: v->edges)
            {
                emit e->left();
            }
            emit v->left();
        }
    }
}

void Graph::dijkstra()
{
    Vertex* source;
    Vertex* target;
    bool is_dag;
    algorithm::topological_sort(Context::table.values(),&is_dag);
    qDebug()<<is_dag;
    if(is_dag)
    {
        if(Context::root == nullptr)
            return;
        queue = algorithm::dijkstra(Context::root,Context::root);
        print(queue);
    }
}

void Graph::print(QQueue<Node*> view)
{
    if(view.isEmpty())
        return;
    qDebug()<<"view";
    for(auto v: Context::table.values())
        v->visited = false;
    queue = view;
    QTimer* timer = new QTimer;
    timer->setInterval(500);
    connect(timer,&QTimer::timeout,this,[this,timer](){
        if(!queue.isEmpty())
        {
            auto node = queue.dequeue();
            int time = -1;
            if(node->type == Node::Type::Vertex)
                time = 500;
            if(node->type == Node::Type::Edge)
                time = 250;
            if(persistent)
                node->visit(-1);
            else
                node->visit(time);
        }
        else
        {
            disconnect(timer,&QTimer::timeout,0,0);
            timer->deleteLater();
            return;
        }
    },Qt::QueuedConnection);
    timer->start();
}

vector<string> Graph::print(vector<vector<int>> m)
{
    int width = 0;
    vector<string> result;
    for(auto a: m)
    {
        for(auto i: a)
        {
            if(width < sizeof(i)/sizeof(int))
                width = sizeof(i)/sizeof(int)+1;
        }
    }
    if(width < 3)
        width = 3;
    for(auto a: m)
    {
        string s;
        for(auto i: a)
        {
            s += fmt::format("{0: ^{1}}",i,width);
        }
        result.push_back(s);
    }
    return result;
}

vector<string> Graph::print(vector<vector<string>> m)
{
    int width = 0;
    vector<string> result;
    for(auto a: m)
    {
        for(auto i: a)
        {
            if(width < i.size());
                width = i.size();
        }
    }
    for(auto a: m)
    {
        string s;
        for(auto i: a)
        {
            s += fmt::format("{0: ^{1}} ",i,width);
        }
        result.push_back(s);
    }
    return result;
}

void Graph::clear()
{
    Context::root = nullptr;
    auto a = Context::table.values();
    Context::table.clear();
    for(auto v: a){
        for(auto e: v->edges)
        {
            if(e == nullptr)
                continue;
            if(e->target == v)
                continue;
            e->deleteLater();
            emit e->deleted();
        }
        emit v->deleted();
        v->deleteLater();
    }
    emit sync(Context::table.size());
}

void Graph::load(QString path)
{
    if(path.startsWith("file:"))
        path = path.split("///").back();
    qDebug()<<path;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"fail to open"<<path;
        return;
    }
    int size = file.readLine().toInt();
    QString s = file.readLine();
    s = s.split("\r\n").front();
    auto& table = Context::table;
    for(auto i: s.split(";"))
    {
        auto a = i.split(":");
        Vertex* vertex = new Vertex(a[0]);
        table.insert(a[0],vertex);
        emit paint(table.value(a[0]));
        if(a.size() >= 3)
        {
            emit vertex->position(a[1].toInt(),a[2].toInt());
        }

    }
    while (!file.atEnd()) {
        s = file.readLine();
        auto a =  s.split(" ");
        Edge* edge = new Edge(table.value(a[0]),table.value(a[1]),a[2].toInt());
        table.value(a[0])->add(edge);
        table.value(a[1])->add(edge);
//        qDebug()<<edge->toString();
        emit paint(edge);
    }
}

void Graph::save(QString path)
{
    if(path.startsWith("file:"))
        path = path.split("///").back();
    qDebug()<<path;
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"fail to open"<<path;
        return;
    }
    auto& table = Context::table;
    QTextStream ts(&file);
    QString s;
    auto vertexs = table.values();
    ts<<vertexs.size()<<"\r\n";
    QSet<Edge*> edges;
    for(auto v: vertexs)
    {
        s += v->toString() + ":" + QString::number(v->x) + ":" + QString::number(v->y) + ";";
        for(auto e: v->edges)
            edges<<e;
    }
    s.chop(1);
    ts<<s<<"\r\n";
    for(auto e: edges)
    {
        auto a = e->toString().split("->");
        ts<<a[0]<<" "<<a[2]<<" "<<a[1]<<"\r\n";
    }
}

void Graph::exec(QVariant value)
{
    auto& root = Context::root;
    if(root == nullptr)
        root = guess();
    if(root == nullptr)
        return;
    for(auto v: Context::table.values())
        v->visited = false;
    int i = value.toInt();
    switch (i) {
    case 0:
        persist();
        break;
    case 1:
        algorithm::reverse(root);
        break;
    case 2:
        print(algorithm::topological_sort(Context::table.values()));
        break;
    case 3:
        print(algorithm::bfs(root));
        break;
    case 4:
        print(algorithm::dfs(root));
        break;
    case 5:
        print(algorithm::prim(root));
        break;
    case 6:
        print(algorithm::kruscal(root));
        break;
    case 7:
        print(algorithm::floyd(root));
        break;
    case 8:
        dijkstra();
        break;
    default:
        break;
    }
}

void Graph::set(Vertex *vertex)
{
    Context::root = vertex;
    auto history = Context::history;
    history.push_back(vertex);
    while (history.size() > 3)
        history.pop_front();
    qDebug()<<vertex->name;
}

void Graph::set(Vertex *source, Vertex *target)
{

}

void Graph::addVertex(Vertex *)
{

}

void Graph::set(Vertex *source, Vertex *target, Edge *edge)
{
    if(edge->contains(source,target))
        return;
    edge->set(source,target);
    source->add(edge);
    target->add(edge);
    auto& table = Context::table;
    table.insert(source->name,source);
    table.insert(target->name,target);
    qDebug()<<edge->toString();
}

Node *Graph::get(QString name)
{
    if(name == "Edge")
        return new Edge();
    if(name == "Vertex")
        return new Vertex();
    return new Vertex();
}

QColor Graph::color(QString name)
{
    auto& colors = Context::colors;
    QColor color(0xffffff);
    if(colors.isEmpty())
        Context::load_color();
    if(colors.contains(name))
        color = colors.value(name);
    else
        qDebug()<<"no"<<colors.size();
    return color;
}

QColor Graph::color(int index)
{
    if(Context::colors.isEmpty())
        Context::load_color();
    return Context::colors.value(names[index%names.size()]);
}

Vertex *Graph::guess()
{
    for(auto v: Context::table.values()){
        if(v->edges.size() - v->degree == 0)
            return v;
    }
    return nullptr;
}
