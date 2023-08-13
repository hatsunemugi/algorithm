# algorithm for graph
图算法可视化实现


# 逆序

就地逆序:需要三个指针分别保存prev,this,next
    prev = head;
    this = head->next;
    if(this == nullptr)//case a->nullptr
        return
    next = this->next;
    if(next == nullptr)//case a->b->nullptr
        this->next = prev;
    while(next != nullptr)
    {
        this->next = prev;
        prev = this;
        this = next;
        next = next->next;
    }
    this->next = prev;

递归法:使用栈
    stack<Node*> stack;
    void reverse(Node* head)
    {
        stack.push(head);
        if(head->next != nullptr)
            reverse(head->next);
        auto node = stack.pop();
        node->next = stack.size() != 0 ? stack.front() : nullptr;
    }

# dfs

    迭代法:使用栈

    递归法:等价于前序遍历

# bfs

    迭代法:使用队列

    递归法:使用深度进行标记



# 拓扑排序

    从 DAG 图中选择一个没有前驱(即入度为0)的顶点并输出。

    从图中删除该顶点和所有以它为起点的有向边。

    重复 1 和 2 直到当前的 DAG 图为空或当前图中不存在无前驱的顶点为止。

    后一种情况说明有向图中必然存在环。

# 最小生成树

  # Prim

    对顶点进行操作,每次从已选集合中选择指向未发现的顶点的最短弧,并将该弧指向的顶点加入已选集合

  # Kruscal

    对边进行操作,对按权值排序的弧进行遍历,若其两端的顶点属于不同集合,则将这两个集合合并

    为减少时间复杂度,使用并查集:维护一颗树

# 最短路径

  # Floyd

    暴力遍历,若经过一个中转节点可以缩短路径,则更新路径长度与前驱节点,结果路径可以通过递归进行访问

  # dijkstra

    从源点出发,在未访问过的可达顶点中选择最短边,更新可达顶点直到所有节点已被访问
