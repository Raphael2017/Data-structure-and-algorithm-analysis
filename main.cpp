#include <iostream>
#include "Graph.h"
#include <microtest.h>

TEST(BFS_TEST) {
    auto graph = make_graph_matrix<char, int>();
    auto s = graph->VInsert('S');
    auto a = graph->VInsert('A');
    auto b = graph->VInsert('B');
    auto c = graph->VInsert('C');
    auto d = graph->VInsert('D');
    auto e = graph->VInsert('E');
    auto f = graph->VInsert('F');
    auto g = graph->VInsert('G');
    graph->EInsert(0, s->Index(), a->Index(), 1);
    graph->EInsert(0, s->Index(), c->Index(), 1);
    graph->EInsert(0, s->Index(), d->Index(), 1);
    graph->EInsert(0, a->Index(), c->Index(), 1);
    graph->EInsert(0, a->Index(), e->Index(), 1);
    graph->EInsert(0, c->Index(), b->Index(), 1);
    graph->EInsert(0, d->Index(), b->Index(), 1);
    graph->EInsert(0, e->Index(), f->Index(), 1);
    graph->EInsert(0, e->Index(), g->Index(), 1);
    graph->EInsert(0, g->Index(), b->Index(), 1);
    graph->EInsert(0, g->Index(), f->Index(), 1);
    graph->Bfs(s->Index());

    ASSERT_TRUE(graph->Exists(s->Index(), a->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(s->Index(), c->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(s->Index(), d->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(a->Index(), c->Index())->Status() == IEdge<int>::CROSS);
    ASSERT_TRUE(graph->Exists(a->Index(), e->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(c->Index(), b->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(d->Index(), b->Index())->Status() == IEdge<int>::CROSS);
    ASSERT_TRUE(graph->Exists(e->Index(), f->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(e->Index(), g->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(g->Index(), b->Index())->Status() == IEdge<int>::CROSS);
    ASSERT_TRUE(graph->Exists(g->Index(), f->Index())->Status() == IEdge<int>::CROSS);
    delete (graph);
}

TEST(DFS_TEST) {
    auto graph = make_graph_matrix<char, int>();
    auto a = graph->VInsert('A');
    auto b = graph->VInsert('B');
    auto c = graph->VInsert('C');
    auto d = graph->VInsert('D');
    auto e = graph->VInsert('E');
    auto f = graph->VInsert('F');
    auto g = graph->VInsert('G');

    graph->EInsert(0, a->Index(), b->Index(), 1);
    graph->EInsert(0, a->Index(), c->Index(), 1);
    graph->EInsert(0, a->Index(), f->Index(), 1);
    graph->EInsert(0, b->Index(), c->Index(), 1);
    graph->EInsert(0, d->Index(), a->Index(), 1);
    graph->EInsert(0, d->Index(), e->Index(), 1);
    graph->EInsert(0, e->Index(), f->Index(), 1);
    graph->EInsert(0, f->Index(), g->Index(), 1);
    graph->EInsert(0, g->Index(), a->Index(), 1);
    graph->EInsert(0, g->Index(), c->Index(), 1);

    graph->Dfs(a->Index());

    ASSERT_TRUE(graph->Exists(a->Index(), b->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(a->Index(), c->Index())->Status() == IEdge<int>::FORWARD);
    ASSERT_TRUE(graph->Exists(a->Index(), f->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(b->Index(), c->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(d->Index(), a->Index())->Status() == IEdge<int>::CROSS);
    ASSERT_TRUE(graph->Exists(d->Index(), e->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(e->Index(), f->Index())->Status() == IEdge<int>::CROSS);
    ASSERT_TRUE(graph->Exists(f->Index(), g->Index())->Status() == IEdge<int>::TREE);
    ASSERT_TRUE(graph->Exists(g->Index(), a->Index())->Status() == IEdge<int>::BACKWARD);
    ASSERT_TRUE(graph->Exists(g->Index(), c->Index())->Status() == IEdge<int>::CROSS);
}

TEST_MAIN();
