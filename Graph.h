#ifndef GRAPH_H
#define GRAPH_H

#include <queue>

template <typename TVData>
class IVertex {
public:
    enum VStatus { UNDISCOVERED, DISCOVERED, VISITED };
    virtual ~IVertex() {}
    virtual TVData& Data() = 0;
    virtual unsigned int  Index() = 0;
    virtual unsigned int& InDegree() = 0;
    virtual unsigned int& OutDegree() = 0;
    virtual int& DTime() = 0;
    virtual int& FTime() = 0;
    virtual VStatus& Status() = 0;
    virtual IVertex*& Parent() = 0;
};

template <typename TEData>
class IEdge {
public:
    enum EStatus { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD };
    virtual ~IEdge() {}
    virtual TEData& Data() = 0;
    virtual EStatus& Status() = 0;
    virtual int& Weight() = 0;
};

template <typename TVData, typename TEData>
class IGraph {
public:
    virtual ~IGraph() {}
    virtual unsigned int& VSize() = 0;
    virtual IVertex<TVData> *VInsert(TVData const&) = 0;
    virtual TVData VRemove(unsigned int) = 0;
    virtual IVertex<TVData> *VAt(unsigned int) = 0;
    virtual unsigned int FirstNbr(unsigned int) = 0;
    virtual unsigned int NextNbr(unsigned int, unsigned int) = 0;
    virtual IEdge<TEData> *Exists(unsigned int, unsigned int) = 0;
    virtual void EInsert(TEData const&, unsigned int, unsigned int, int) = 0;
    virtual TEData ERemove(unsigned int, unsigned int) = 0;

public:
    void Bfs(unsigned int start) {
        Reset();
        int clock = 0;
        unsigned int v = start;
        do {
            if (VAt(v)->Status() == IVertex<TVData>::UNDISCOVERED) {
                Bfs(v, clock);
            }
        } while ((v = (++v) % VSize()) != start);
    }

    void Dfs(unsigned int start) {
        Reset();
        int clock = 0;
        unsigned int v = start;
        do {
            if (VAt(v)->Status() == IVertex<TVData>::UNDISCOVERED) {
                Dfs(v, clock);
            }
        } while ((v = (++v) % VSize()) != start);
    }
private:
    void Bfs(unsigned int start, int& clock) {
        std::queue<unsigned int> queue;
        VAt(start)->Status() = IVertex<TVData>::DISCOVERED;
        queue.push(start);
        while (!queue.empty()) {
            auto u = queue.front(); VAt(u)->DTime() = ++clock; queue.pop();
            for (auto it = FirstNbr(u); it < VSize(); it = NextNbr(u, it)) {
                if (VAt(it)->Status() == IVertex<TVData>::UNDISCOVERED) {
                    VAt(it)->Status() = IVertex<TVData>::DISCOVERED;
                    Exists(u, it)->Status() = IEdge<TEData>::TREE;
                    VAt(it)->Parent() = VAt(u);
                    queue.push(it);
                } else
                    Exists(u, it)->Status() = IEdge<TEData>::CROSS;
            }
            VAt(u)->Status() = IVertex<TVData>::VISITED;
        }
    }

    void Dfs(unsigned int start, int& clock) {
        VAt(start)->Status() = IVertex<TVData>::DISCOVERED;
        VAt(start)->DTime() = ++clock;
        for (auto it = FirstNbr(start); it < VSize(); it = NextNbr(start, it)) {
            switch (VAt(it)->Status()) {
                case IVertex<TVData>::UNDISCOVERED: {
                    Exists(start, it)->Status() = IEdge<TEData>::TREE;
                    VAt(it)->Parent() = VAt(start);
                    Dfs(it, clock);
                } break;
                case IVertex<TVData>::DISCOVERED: {
                    Exists(start, it)->Status() = IEdge<TEData>::BACKWARD;
                } break;
                default: {
                    Exists(start, it)->Status() = VAt(it)->DTime() > VAt(start)->DTime() ? IEdge<TEData>::FORWARD : IEdge<TEData>::CROSS;
                }
            }
        }
        VAt(start)->FTime() = ++clock;
        VAt(start)->Status() = IVertex<TVData>::VISITED;
    }

    void Reset() {
        for (unsigned int i = 0; i < VSize(); ++i) {
            VAt(i)->Status() = IVertex<TVData>::UNDISCOVERED;
            VAt(i)->DTime() = -1;
            VAt(i)->FTime() = -1;
            VAt(i)->Parent() = nullptr;
            for (unsigned int j = 0; j < VSize(); ++j) {
                IEdge<TEData> *edge = Exists(i, j);
                if (edge != nullptr) edge->Status() = IEdge<TEData>::UNDETERMINED;
            }
        }
    }
};

#include "GraphMatrix.cpp"

#endif