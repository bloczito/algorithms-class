#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <stack>
#include <queue>
#include <queue>
#include <optional>
#include <iostream>

using namespace std;

// Uwaga! Kod powinien być odporny na błędy i każda z metod jeżeli zachodzi niebezpieczeństwo wywołania z niepoprawnymi parametrami powinna zgłaszac odpowiednie wyjątki!

// klasa reprezentująca graf skierowany oparty na MACIERZY SĄSIEDZTWA
// V - dane przechowywane przez wierzcholki
// E - dane przechowywane przez krawedzie (etykiety)
template <typename V, typename E>
class Graph
{
public:
// iterator po wierzchołkach (rosnąco po id wierzchołków)
    class VerticesIterator
    {
    private:
        Graph<V, E> &_graph;
        int index;
    public:

        explicit VerticesIterator(Graph<V, E> &graph, size_t current_vertex_id = 0u)
                : _graph(graph), index(current_vertex_id) {}
        bool operator==(const VerticesIterator &vi2) const { return (index == vi2.index) ; }
        bool operator!=(const VerticesIterator &vi2) const { return (index != vi2.index) ; }
        VerticesIterator& operator++() { ++index; return *this; }
        VerticesIterator operator++(int) { VerticesIterator it(*this); ++(*this); return it; }
        V& operator*() const;
        V* operator->() const { return &_graph._vertices[index]; }
        explicit operator bool () const { return index < _graph._vertices.size(); }
        // zwraca id aktualnego wierzchołka
        std::size_t id() const { return index; }

    };

// iterator po istniejących krawędziach
    class EdgesIterator
    {
    private:
        std::size_t _row, _column;
        Graph<V, E> &_graph;
    public:

        EdgesIterator(Graph<V, E> &graph, std::size_t row, std::size_t col)
                : _graph(graph), _row(row), _column(col) {}
        bool operator==(const EdgesIterator &ei) const { return (_row == ei._row) && (_column == ei._column); }
        bool operator!=(const EdgesIterator &ei) const { return (_row != ei._row) && (_column != ei._column); }
        EdgesIterator& operator++();
        EdgesIterator operator++(int);
        E& operator*() const { return _graph._nbMatrix[_row][_column].value(); }
        E* operator->() const { return &_graph.nbMatrix[_row][_column].value(); }
        explicit operator bool() const {if(_row < _graph._nbMatrix.size() && _column < _graph._nbMatrix[_graph._nbMatrix.size()-1].size()) return true; else return false; }
// zwraca id wierzchołka startowego
        std::size_t v1id() const { return _row; }
// zwraca id wierzchołka koncowego
        std::size_t v2id() const { return _column; }
    };


public:
    Graph() : _edges(0) {}
    Graph(const Graph<V, E> &source) = default;
    Graph(Graph<V, E> &&source) = default;
    Graph& operator=(const Graph<V, E> &source) = default;
    Graph& operator=(Graph<V, E> &&source) = default;
    ~Graph() = default;

// dodaje nowy wierzchołek z danymi przyjętymi w argumencie (wierzchołek powinien posiadać kopie danych) i zwraca "VerticesIterator" na nowo utworzony wierzchołek
    VerticesIterator insertVertex(const V &vertex_data);
// dodaje nową krawędź między wierzchołkami o id "vertex1_id" i "vertex2_id" i zwraca "EdgesIterator" na nowo dodaną krawędź, oraz informację o tym czy została dodana nowa krawędź, czy nie
// jeśli krawędź między podanymi wierzchołkami już istnieje to działanie funkcji zależy od ostatniego argumentu
// jeśli ostatni argument przyjmuje wartość "true" to krawędź zostaje zastąpiona, jeśli "false" to nie
    std::pair<EdgesIterator, bool> insertEdge(std::size_t vertex1_id, std::size_t vertex2_id, const E &label = E(), bool replace = true);
// usuwa wierzchołek o podanym id i zwraca "VerticesIterator" na kolejny wierzchołek, lub to samo co "endVertices()" w przypadku usunięcia ostatniego wierzchołka, lub braku wierzchołka o podanym id
    VerticesIterator removeVertex(std::size_t vertex_id);
// usuwa krawedź między dwoma wierzchołkami o podanych id i zwraca "EdgesIterator" na kolejną krawędź, lub to samo co "endEdges()" w przypadku usunięcia ostatniej krawedzi, lub braku krawędzi między wierzchołkami o podanych id
    EdgesIterator removeEdge(std::size_t vertex1_id, std::size_t vertex2_id);
// zwraca true jeśli istnieje krawędź między wierzchołkami o podanych id, false w przeciwnym razie
// O(1)
    bool edgeExist(std::size_t vertex1_id, std::size_t vertex2_id) const;
// zwraca ilość wierzchołków w grafie
// O(1)
    std::size_t nrOfVertices() const { return _vertices.size(); }
// zwraca ilość krawędzi w grafie
// O(1)
    std::size_t nrOfEdges() const { return _edges; }
// drukuje macierz sąsiedztwa na konsoli (debug)
    void printNeighborhoodMatrix() const;
// zwraca "VerticesIterator" do wierzchołka o podanym id, lub to samo co "endVertices()" w przypadku braku wierzchołka o podanym id
    VerticesIterator vertex(std::size_t vertex_id);
// zwraca referencję do danych wierzchołka o podanym id
    const V& vertexData(std::size_t vertex_id) const;
// zwraca referencję do danych wierzchołka o podanym id
    V& vertexData(std::size_t vertex_id);
// zwraca "EdgesIterator" do krawędzi pomiędzy wierzchołkami o podanych id, lub to samo co "endEdges()" w przypadku braku krawędzi między wierzchołkami o podanych id
    EdgesIterator edge(std::size_t vertex1_id, std::size_t vertex2_id);
// zwraca referencję do danych (etykiety) krawędzi pomiędzy wierzchołkami o podanych id
    const E& edgeLabel(std::size_t vertex1_id, std::size_t vertex2_id) const;
// zwraca referencję do danych (etykiety) krawędzi pomiędzy wierzchołkami o podanych id
    E& edgeLabel(std::size_t vertex1_id, std::size_t vertex2_id);
    VerticesIterator begin() { return beginVertices(); }
    VerticesIterator end() { return endVertices(); }
// zwraca "VerticesIterator" na pierwszy wierzchołek (o najmniejszym id)
    VerticesIterator beginVertices();
// zwraca "VerticesIterator" "za ostatni" wierzchołek
    VerticesIterator endVertices();
// zwraca "EdgesIterator" na pierwszą krawędz
    EdgesIterator beginEdges();
// zwraca "EdgesIterator" "za ostatnią" krawędz
    EdgesIterator endEdges();

    vector<vector<optional<E>>>  _nbMatrix;
    vector<V> _vertices;
    int _edges;
};

template<typename V, typename E>
V &Graph<V, E>::VerticesIterator::operator*() const {
//    if(_graph._vertices.size() > index)
//        if(_graph._vertices[index])
            return _graph._vertices[index];
//        else
//            return "X";
//    else
//        return (V) nullptr;
}


template<typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::insertVertex(const V &vertex_data) {
    _vertices.push_back(vertex_data);

    for(int i = 0; i < _nbMatrix.size(); ++i) {
        _nbMatrix[i].resize(_nbMatrix[i].size() + 1);
    }

    _nbMatrix.push_back(vector<optional<E>>(_nbMatrix.size() + 1));
    return VerticesIterator(*this, _vertices.size() - 1);
}

template<typename V, typename E>
pair<typename Graph<V, E>::EdgesIterator, bool>
Graph<V, E>::insertEdge(std::size_t vertex1_id, std::size_t vertex2_id, const E &label, bool replace) {
    if(vertex1_id < _vertices.size() && vertex2_id < _vertices.size() && replace) {
        if(!_nbMatrix[vertex1_id][vertex2_id]) {
            ++_edges;
        }
        _nbMatrix[vertex1_id][vertex2_id] = label;
        return make_pair(EdgesIterator(*this, vertex1_id, vertex2_id), true);
    } else {
        return make_pair(EdgesIterator(*this, vertex1_id, vertex2_id), false);
    }
}

template<typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::removeVertex(std::size_t vertex_id) {

    if(vertex_id < _vertices.size()) {
        _vertices.erase(_vertices.begin() + vertex_id);

        for(int i = 0; i < _nbMatrix.size(); ++i) {
            if(_nbMatrix[i][vertex_id]) {
                --_edges;
            }
            if(_nbMatrix[vertex_id][i] && i != vertex_id) {
                --_edges;
            }
            _nbMatrix[i].erase(_nbMatrix[i].begin() + vertex_id);
        }
        _nbMatrix.erase(_nbMatrix.begin() + vertex_id);
        return  VerticesIterator(*this, vertex_id);
    } else {
        return endVertices();
    }
}

template<typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::removeEdge(std::size_t vertex1_id, std::size_t vertex2_id) {

    if (vertex1_id < _vertices.size() && vertex2_id < _vertices.size()
        && _nbMatrix[vertex1_id][vertex2_id]) {
//        ++tmp;
        _nbMatrix[vertex1_id][vertex2_id].reset();
        --_edges;
        EdgesIterator tmp(*this, vertex1_id, vertex2_id+1);
        return tmp;
    } else {
//        cout << "ASD" << endl;
        return endEdges();
    }
}

template<typename V, typename E>
bool Graph<V, E>::edgeExist(std::size_t vertex1_id, std::size_t vertex2_id) const {
    return  vertex1_id < _vertices.size() && vertex2_id < _vertices.size()
            && _nbMatrix[vertex1_id][vertex2_id];
}

template<typename V, typename E>
void Graph<V, E>::printNeighborhoodMatrix() const {
    for (int i = 0; i < _nbMatrix.size(); ++i) {
        for (int j = 0; j < _nbMatrix[i].size(); ++j) {
            if (_nbMatrix[i][j]) {
                std::cout << _nbMatrix[i][j].value() << " ";
            } else {
                std::cout << "X ";
            }
        }
        std::cout << std::endl;
    }
}

template<typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::vertex(std::size_t vertex_id) {
    return Graph::VerticesIterator();
}

template<typename V, typename E>
const V &Graph<V, E>::vertexData(std::size_t vertex_id) const {
    if(vertex_id < _vertices.size()) {
        return _vertices[vertex_id];
    } else {
        throw invalid_argument("invalid vertex id");
    }
}

template<typename V, typename E>
V &Graph<V, E>::vertexData(std::size_t vertex_id) {
    if(vertex_id < _vertices.size()) {
        return _vertices[vertex_id];
    } else {
        throw invalid_argument("invalid vertex id");
    }
}

template<typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::edge(std::size_t vertex1_id, std::size_t vertex2_id) {
    return EdgesIterator(*this, vertex1_id, vertex2_id);
}

template<typename V, typename E>
const E &Graph<V, E>::edgeLabel(std::size_t vertex1_id, std::size_t vertex2_id) const {
    EdgesIterator it(*this, vertex1_id, vertex2_id);
    return *it;
}

template<typename V, typename E>
E &Graph<V, E>::edgeLabel(std::size_t vertex1_id, std::size_t vertex2_id) {
    EdgesIterator it(*this, vertex1_id, vertex2_id);
    return *it;
}

template<typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::beginVertices() {
    return VerticesIterator(*this, 0);
}

template<typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::endVertices() {
    return VerticesIterator(*this, _vertices.size());
}

template<typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::beginEdges() {
    for(int i = 0; i < _nbMatrix.size(); ++i) {
        for(int j = 0; j < _nbMatrix[i].size(); ++j) {
            if(_nbMatrix[i][j]) {
                return EdgesIterator(*this, i, j);
            }
        }
    }
    return EdgesIterator(*this, _nbMatrix.size(), _nbMatrix.size());
}

template<typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::endEdges() {
    return Graph::EdgesIterator(*this, _nbMatrix.size(), _nbMatrix.size());
}


//  =============EDGES ITERATOR ===================
template<typename V, typename E>
typename Graph<V, E>::EdgesIterator &Graph<V, E>::EdgesIterator::operator++() {
    ++_column;
    for(int i = _row; i < _graph._nbMatrix.size(); ++i) {
        for(int j = _column; j < _graph._nbMatrix[i].size(); ++j) {
            if(_graph._nbMatrix[i][j]) {
                _row = i;
                _column = j;
                return *this;
            }
        }
        _column = 0;
    }
    _row = _graph._vertices.size();
    _column - _graph._vertices.size();
    return *this;
}

template<typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::EdgesIterator::operator++(int) {
    EdgesIterator it(*this);
    ++(*this);
    return it;
}


