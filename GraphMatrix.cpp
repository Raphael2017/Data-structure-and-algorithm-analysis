#include <vector>

template <typename TVData>
class ImplVertex : public IVertex<TVData> {
public:
    ImplVertex(TVData const& data, int index) : _data(data), _index(index), _in_degree(0), _out_degree(0), _dtime(-1),
                                                _ftime(-1), _status(IVertex<TVData>::UNDISCOVERED), _parent(nullptr) {}
    virtual TVData& Data() override { return _data; }
    virtual unsigned int  Index() override { return _index; }
    virtual unsigned int& InDegree() override { return _in_degree; }
    virtual unsigned int& OutDegree() override { return _out_degree; }
    virtual int& DTime() override { return _dtime; }
    virtual int& FTime() override { return _ftime; }
    virtual typename IVertex<TVData>::VStatus& Status() override { return _status; }
    virtual IVertex<TVData>*& Parent() override { return _parent; }

private:
    TVData _data;
    int _index;
    unsigned int _in_degree;
    unsigned int _out_degree;
    int _dtime;
    int _ftime;
    typename IVertex<TVData>::VStatus _status;
    IVertex<TVData> *_parent;
};

template <typename TEData>
class ImplEdge : public IEdge<TEData> {
public:
    ImplEdge(TEData const& data, int weight) : _data(data), _weight(weight), _status(IEdge<TEData>::UNDETERMINED) {}
    virtual TEData& Data() override { return _data; }
    virtual typename IEdge<TEData>::EStatus& Status() override { return _status; }
    virtual int& Weight() override { return _weight; }

private:
    TEData _data;
    int _weight;
    typename IEdge<TEData>::EStatus _status;
};

template <typename TVData, typename TEData>
class GraphMatrix : public IGraph<TVData, TEData> {
public:
    GraphMatrix() : _vsize(0), _esize(0) {}
    ~GraphMatrix() {
        for (auto it : _V) delete (it);
        for (int i = 0; i < _vsize; ++i) {
            for (int j = 0; j < _vsize; ++j) {
                delete (_E[i][j]);
            }
        }
    }

public:
    virtual unsigned int& VSize() override {
        return _vsize;
    }
    virtual IVertex<TVData> *VInsert(TVData const& data) override {
        for (int i = 0; i < _vsize; ++i) _E[i].push_back(nullptr);
        _vsize++;
        _E.push_back(std::vector<IEdge<TEData>*>(_vsize, nullptr));
        auto v = new ImplVertex<TVData>(data, _vsize-1);
        _V.push_back(v);
        return v;
    }
    virtual TVData VRemove(unsigned int index) override {
        for (int i = 0; i < _vsize; ++i) {
            if (Exists(index, i) != nullptr) {
                delete (_E[index][i]);
                _V[i]->InDegree()--;
            }
        }
        _E.erase(_E.begin()+index);
        _vsize--;
        for (int i = 0; i < _vsize; ++i) {
            if (Exists(i, index) != nullptr) {
                delete (_E[i][index]);
                _E[i].erase(_E[i].begin()+index);
                _V[i]->OutDegree()--;
            }
        }
        TVData data = _V[index]->Data();
        delete (_V[index]);
        _V.erase(_V.begin()+index);
        return data;
    }
    virtual IVertex<TVData> *VAt(unsigned int index) override {
        return _V[index];
    }
    virtual unsigned int FirstNbr(unsigned int u) override {
        int cur_nbr = -1;
        while ((++cur_nbr) < _vsize && !Exists(u, cur_nbr));
        return cur_nbr;
    }
    virtual unsigned int NextNbr(unsigned int u, unsigned int cur_nbr) override {
        while ((++cur_nbr) < _vsize && !Exists(u, cur_nbr));
        return cur_nbr;
    }
    virtual IEdge<TEData> *Exists(unsigned int u, unsigned int v) override {
        return 0 <= u && u <= _vsize && 0 <= v && v <= _vsize ? _E[u][v] : nullptr;
    }
    virtual void EInsert(TEData const& data, unsigned int u, unsigned int v, int weight) override {
        if (Exists(u, v) != nullptr) return;
        _E[u][v] = new ImplEdge<TEData>(data, weight);
        _V[u]->OutDegree()++;
        _V[v]->InDegree()++;
        ++_esize;
    }
    virtual TEData ERemove(unsigned int u, unsigned int v) override {
        TEData data = _E[u][v]->Data();
        delete (_E[u][v]);
        _V[u]->OutDegree()--;
        _V[v]->InDegree()--;
        return data;
    }
private:
    std::vector<IVertex<TVData>*> _V;
    std::vector<std::vector<IEdge<TEData>*>> _E;
    unsigned int _vsize;
    unsigned int _esize;
};

template <typename TVData, typename TEData>
IGraph<TVData, TEData> *make_graph_matrix() {
    return new GraphMatrix<TVData, TEData>;
}