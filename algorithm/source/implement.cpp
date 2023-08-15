#include "graph.h"
#include "implement.h"
#include <iostream>
#include "format.h"

using algorithm = Context::algorithm;
QHash<QString,Vertex*> Context::table;
QHash<QString,QColor> Context::colors;
QVector<Vertex*> Context::history;
QTextStream Context::qout(stdout);
int Context::INF = -1;
Vertex* Context::root = nullptr;
Graph* Context::graph = nullptr;
void algorithm::reverse(Vertex *vertex)
{
    if(vertex->visited)
        return;
    vertex->visited = true;
    for(auto& edge: vertex->edges)
    {
        if(edge->target == vertex)
            continue;
        reverse(edge->target);
        edge->reverse();
    }

}

QQueue<Node*> algorithm::bfs(Vertex *root)
{
    QQueue<Node*> result;
    QQueue<Vertex*> queue;
    queue.enqueue(root);
    result.enqueue(root);
    while(!queue.isEmpty())
    {
        root = queue.dequeue();
        for(auto& edge: root->edges)
        {
            if(edge->target != root)
            {
                queue.enqueue(edge->target);
                result.enqueue(edge);
                result.enqueue(edge->target);//加入结果队列
            }
        }
    }
    return result;
}

QQueue<Node*> algorithm::dfs(Vertex *root)
{
    QQueue<Node*> result;
    result.enqueue(root);
    for(auto& edge: root->edges)
    {
        if(edge->target != root)
        {
            result.enqueue(edge);
            result.append(dfs(edge->target));
        }
    }
    return result;
}

QVector<Edge *> algorithm::get_edge(Vertex *source)
{
    QSet<Node*> nodes = get(source,(int)Node::Type::Edge);
    QVector<Edge*> result;
    for(auto n: nodes)
    {
        result<<(Edge*)n;
    }
    return result;
}

QVector<Vertex *> algorithm::get_vertex(Vertex *source)
{
    Vertex* vertex ;
    QSet<Node*> nodes = get(source,(int)Node::Type::Vertex);
    QVector<Vertex*> result;
    for(auto n: nodes)
    {
        vertex = (Vertex*)n;
        vertex -> visited = false;
//        vertex -> root = vertex;
        vertex -> parent = vertex;
        vertex -> height = 0;
        result<<vertex;
    }
    return result;
}

QSet<Node *> algorithm::get(Vertex *source, int type)
{
    QSet<Node*> result;
    for(auto n: dfs(source))//获取所有顶点
        if((int)n->type == type)
            result<<n;
    return result;
}


QQueue<Node *> algorithm::prim(Vertex *source)
{
    QVector<Vertex*> U = get_vertex(source);
    QSet<Vertex*> S;
    Edge* min;
    S<<source;
    QQueue<Node *> result;
    result<<source;
    int i=0;
    while(true)
    {
        min = nullptr;
        for(auto v: qAsConst(S))
        {
            for(auto e: v->edges)
            {
                if(!(S.contains(v)&&S.contains(e->target)))
                {
                    if(min != nullptr)
                        if(e->weight >= min->weight)
                            continue;
                    min = e;
                }
            }
        }
        if(min != nullptr)
        {
            Vertex* v = S.contains(min->source) ? min->target:min->source;
            S<<v;
            result<<min;
            result<<v;
        }
        else
            break;
        i++;
        if(i>U.size())
            break;
    }
    return result;
}

QQueue<Node *> algorithm::kruscal(Vertex *source)
{
    int size = get_vertex(source).size();//将parent指向自己
    QSet<Vertex*> S;
    QVector<Edge*> edges(get_edge(source));
    std::sort(edges.begin(),edges.end(),[](Edge* a, Edge* b){return a->weight<b->weight;});
    std::function<bool(Edge*)> find = [](Edge* edge)->bool{return Vertex::root(edge->source)==Vertex::root(edge->target);};
    std::function<void (Vertex*)> height = [&height](Vertex* vertex){
        if(Vertex::root(vertex) == vertex)
            return;
        vertex->parent->height = vertex->height + 1;
        height(vertex->parent);
    };
    std::function<void (Vertex*,Vertex*)> trace = [&trace,&height](Vertex* source,Vertex* target)
    {
        if(source->parent == source)
        {
            qDebug()<<"parent:"<<source->parent->toString()<<target->toString();
            source->parent = target;
            height(source);
            return;
        }
        trace(source->parent,target);
    };
    auto insert= [&S,&trace](Edge* edge)->QVector<Node*>{
        QVector<Node*> result;
        int root = Vertex::root(edge->source)->height- Vertex::root(edge->target)->height;
        int dy = edge->source->height-edge->target->height;
        Vertex* source = nullptr;
        Vertex* target = nullptr;
        if(root == 0)
        {
            if(dy == 0)
            {
                source = edge->target;
                target = edge->source;
            }
            if(dy > 0)
            {
                source = edge->target;
                target = edge->source;
            }
            if(dy < 0)
            {
                source = edge->source;
                target = edge->target;
            }
        }
        if(root > 0)
        {
            source = edge->target;
            target = edge->source;
        }
        if(root < 0)
        {
            source = edge->source;
            target = edge->target;
        }
        result<<target<<edge<<source;
        S<<target<<source;
        trace(source,target);
        return result;
    };
    QQueue<Node *> result;
    for(auto e: edges)
    {
        if(!find(e))
        {
            result.append(insert(e));
        }
    }

    return result;
}

QQueue<Node *> algorithm::topological_sort(QVector<Vertex *> table, bool *is_dag)
{
    QQueue<Vertex*> queue;
    QQueue<Node*> result;
    Vertex* vertex = nullptr;
    int size = table.size();
    int count = 0;
    int degree = 0;
    int i = 0;

    auto enqueue = [](QVector<Vertex *>&table,QQueue<Vertex*>&queue,int& degree,int size){
        for(;degree < size && queue.isEmpty();degree++)
        {
            int i = 0;
            while(i<table.size())
            {
                if(table[i]->edges.size() - table[i]->degree == degree)//入度为degree{0,1,2,3...}
                {
                    queue<<table[i];
                    table.remove(i);
                }
                else
                    i++;
            }
        }
    };

    enqueue(table,queue,degree,size);

    qDebug()<<queue.size()<<size;
    while (!queue.isEmpty()) {
        count++;
        vertex = queue.dequeue();
        qDebug()<<vertex->toString()<<degree;
        result<<vertex;
        for(auto e: vertex->edges)
        {
            if(e->source == vertex)
                result<<e;
        }
        if(queue.isEmpty())
            enqueue(table,queue,degree,size);
    }
    if(is_dag != nullptr)
    {
        *is_dag = count == size;
    }
    return result;
}

QQueue<Node *> algorithm::floyd(Vertex *source)
{
    QQueue<Node *> result;
    QVector<Vertex*> vertexs = get_vertex(source);
    std::sort(vertexs.begin(),vertexs.end(),[](Vertex* a, Vertex* b){return a->toString()<b->toString();});
    int size = vertexs.size();
    int index = vertexs.indexOf(source);
    using std::vector;
    using std::cout;
    using Qt::endl;
    vector<vector<int>> D(size,vector<int>(size));
    vector<vector<int>> Path(size,vector<int>(size));

    auto print = [&vertexs](vector<string> a){
        for(auto v: vertexs)
        {
            qout<<(fmt::format("{0: ^{1}}",v->toString().toStdString(),3)).c_str();
        }
        qout<<Qt::endl;
        for(int i = 0; i < vertexs.size(); i++)
        {
            qout<<vertexs[i]->toString()<<" ";
            qout.flush();
            qout<<a[i].c_str()<<endl;
        }
    };

    for(int i = 0;i < size; i++)
    {
        auto v = vertexs[i];
        for(auto e: v->edges)
        {
            int j;
            if(e->target == v){
                j = vertexs.indexOf(e->source);
                D[i][j] = INF;
            }
            else{
                j = vertexs.indexOf(e->target);
                D[i][j] = e->weight;
                Path[i][j] = i;
            }
        }
    }

    cout<<"distance:\n% ";
    cout.flush();
    print(Graph::print(D));

    for(int k = 0; k < size; k++)
    {
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                if(i == j ||  j == k || k == i)
                    continue;
                if(D[i][k] <= 0 || D[k][j] <= 0)
                    continue;
                if(D[i][k]+D[k][j] < D[i][j] || D[i][j] == 0)
                {
//                    qDebug()<<D[i][k]<<D[k][j]<<D[i][j];
                    D[i][j] = D[i][k]+D[k][j];
                    Path[i][j] = Path[k][j];
                    qout<<D[i][k]<<" "<<D[k][j]<<" "<<D[i][j]<<endl;
                }
            }
        }
    }

    cout<<"distance:\n% ";
    cout.flush();
    print(Graph::print(D));

    cout<<"path:\n% ";
    cout.flush();
    print(Graph::print(Path));

    std::function<QVector<Node*>(vector<vector<int>>,int,int)> walk= [&walk,&vertexs](vector<vector<int>> path, int x, int y)->QVector<Node*>{
        QVector<Node*> result;
        result<<vertexs[y];
        if(y == x)
            return result;
        result.append(walk(path,x,path[x][y]));
        return result;
    };

    for(int i = 0; i < size; i++)
    {
        auto a = walk(Path,index,i);
        std::reverse(a.begin(),a.end());
        int j = 0;
        while(true)
        {
            if(j >= a.size()-1)
                break;
            if(a[j]->type == Node::Type::Vertex)
            {
                Vertex* v = (Vertex*)a[j];
                for(auto e: v->edges)
                {
                    if(e->source == v && e->contains(v,(Vertex*)a[j+1])){
//                        qDebug()<<e->toString();
                        a.insert(j+1,e);
                        break;
                    }
                }
            }
            j++;
        }

        result.append(a);
    }
    return result;
}

QQueue<Node*> algorithm::dijkstra(Vertex *source, Vertex *target)
{
    struct Arc{
        int distance;
        Edge* edge;
    };
    QVector<Vertex*> vertexs = get_vertex(source);
    QHash<QString,Arc> arcs;
    QQueue<Node*> result;
    int i = 0;
    Edge* min = nullptr;

    for(auto v: vertexs)
        arcs.insert(v->name,{INF});

    for(auto e: source->edges)
    {
        if(e->target != source)
            arcs.insert(e->target->name,{e->weight,e});
    }
    source->visited = true;
    result<<source;
    while(true)
    {
        if(i > vertexs.size())//防止出现环
            return result;
        min = nullptr;
        int value = INF;
        for(auto arc: arcs.values())
        {
            if(arc.distance != INF)//顶点不可达,跳过
            {
                if(!arc.edge->target->visited)//顶点已访问,跳过
                {
                    if(arc.distance < value || value == INF)
                    {
                        min = arc.edge;
                        value = arc.distance;
                    }
                }
            }
        }//寻找指向未被访问的顶点的最短弧

        if(min!=nullptr)
        {
            min->target->visited = true;//最短弧指向的顶点设为已被访问
            result<<min;
            result<<min->target;
        }
        else
            break;//所有顶点已被访问过

        for(auto edge: min->target->edges)
        {
            auto name = edge->target->name;
            if(arcs.value(name).distance == INF)
            {
                arcs.insert(name,{edge->weight + value, edge});
            }
            else if(edge->weight + value < arcs.value(name).distance)
            {
                arcs.insert(name,{edge->weight + value, edge});
            }
        }//更新路径

        i++;
    }
    return result;
}

void Context::load_color()
{
    QString path = "values/colors.txt";
    QFile* file = new QFile(path);
    if(!file->open(QIODevice::ReadOnly)){
        qDebug()<<"fail to open"<<path;
        path="algorithm/"+path;
        file =new QFile(path);
        if(!file->open(QIODevice::ReadOnly)){
            qDebug()<<"fail to open"<<path;
            return;
        }
    }
    while (!file->atEnd()) {
        auto a = QString::fromUtf8(file->readLine()).split(" ");
        if(a.size()>1)
        {
            colors.insert(a[0],a[1]);
        }
    }
    qDebug()<<colors.size();
}
