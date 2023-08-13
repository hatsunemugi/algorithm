#ifndef IMPLEMENT_H
#define IMPLEMENT_H

#include <QHash>
#include <QObject>
#include <QTextStream>
class Node;
class Edge;
class Graph;
class Vertex;
namespace graph{
    static QHash<QString,Vertex*> table;
    static QVector<Vertex*> history;
    static Vertex* root;
    static Graph* graph;
    static QTextStream qout(stdout);
    static int inf = -1;
    class algorithm
    {
    public:
        static void reverse (Vertex*);
        static QQueue<Node*> bfs(Vertex*);//广度优先搜索(breadth_first_search)
        static QQueue<Node*> dfs(Vertex*);//深度优先搜索(breadth_first_search)
        static QVector<Edge*> get_edge(Vertex*);
        static QVector<Vertex*> get_vertex(Vertex*);
        static QSet<Node*> get(Vertex*,int);
        static QQueue<Node*> prim(Vertex*);
        static QQueue<Node*> kruscal(Vertex*);
        static QQueue<Node*> floyd(Vertex*);
        static QQueue<Node*> dijkstra(Vertex*,Vertex*);
        static QQueue<Node*> topological_sort(QVector<Vertex*>,bool* is_dag=nullptr);//拓扑排序,有向无环图(DAG,directed_acyclic_graph)
    };
}



#endif // IMPLEMENT_H
