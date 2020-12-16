#pragma once

#include <functional>
#include <vector>
#include <stack>
#include <queue>
#include <limits>
#include <optional>
#include "Graph.hpp"

using namespace std;

template <typename V, typename E>
void DFS(const Graph<V, E> &g, std::size_t start_id, std::function<void(const V&)> f) {

    if (start_id < 0 || g._vertices.size() <= 0) return;

    size_t index = start_id;
    vector<bool> visited(g._vertices.size());
    stack<int> vertexStack;

    for(size_t i = 0; i < g._vertices.size(); ++i) {
        visited.push_back(false);
    }

    vertexStack.push(index);
    visited[index] = true;

    do {
        index = vertexStack.top();
        vertexStack.pop();
        for (size_t i = 0; i < g._vertices.size(); ++i) {
            if (g._nbMatrix[index][i] && visited[i] == false) {
                vertexStack.push(i);
                visited[i] = true;
            }
        }
        f(g.vertexData(index));
    } while (!vertexStack.empty());
}

template <typename V, typename E>
void BFS(const Graph<V, E> &g, std::size_t start_id, std::function<void(const V&)> f) {

    if (start_id < 0 || g._vertices.size() <= 0) return;

    size_t index = start_id;
    vector<bool> visited(g._vertices.size());
    queue<int> vertexQueue;

    for(size_t i = 0; i < g._vertices.size(); ++i) {
        visited.push_back(false);
    }

    vertexQueue.push(index);
    visited[index] = true;

    do {
        index = vertexQueue.front();
        vertexQueue.pop();
        for (size_t i = 0; i < g._vertices.size(); ++i) {
            if (g._nbMatrix[index][i] && visited[i] == false) {
                vertexQueue.push(i);
                visited[i] = true;
            }
        }
        f(g.vertexData(index));
    } while (!vertexQueue.empty());
}

template<typename V, typename E>
std::pair<double, std::vector<std::size_t>> dijkstra(Graph<V, E>& graph, std::size_t start_idx, std::size_t end_idx, std::function<double(const E&)> getEdgeLength = nullptr) {


    std::vector<double > traverseCosts(graph._vertices.size());
    std::vector<bool>checkedVertices(graph._vertices.size());
    std::vector<int>predecessors(graph._vertices.size());

    std::fill(traverseCosts.begin(), traverseCosts.end(), std::numeric_limits<double>::max());
    std::fill(checkedVertices.begin(), checkedVertices.end(), false);
    std::fill(predecessors.begin(), predecessors.end(), -1);

    traverseCosts[start_idx] = 0;

    while(!checkedVertices[end_idx]) {
        double minimalCost = std::numeric_limits<double>::max();
        int minimalIndex = -1;
        for (int j = 0; j < graph._vertices.size(); ++j) {
            if (!checkedVertices[j] && traverseCosts[j] < minimalCost) {
                minimalIndex = j;
                minimalCost = traverseCosts[j];
            }
        }

        std::vector<std::size_t> path;
        if (minimalIndex == -1) {
            return std::make_pair(minimalIndex, path);
        }

        checkedVertices[minimalIndex] = true;

        for (int j = 0; j < graph._vertices.size(); ++j) {
            /// sprawdzanie sąsiadów (czy są i czy nie zostały rozpatrzone)
            if (graph._nbMatrix[minimalIndex][j] && !checkedVertices[j]) {
                /// jeśli zapisany koszt dojścia jest większy niż obecny
                if (traverseCosts[j] > (traverseCosts[minimalIndex]
                                        + graph._nbMatrix[minimalIndex][j].value()) ) {

                    if(getEdgeLength == nullptr) {
                        traverseCosts[j] = traverseCosts[minimalIndex]
                                           + graph._nbMatrix[minimalIndex][j].value();
                    } else {
                        traverseCosts[j] = traverseCosts[minimalIndex] + getEdgeLength(graph._nbMatrix[minimalIndex][j].value());
                    }

                    predecessors[j] = minimalIndex;
                }
            }
        }
    }

    std::vector<std::size_t> path;
    path.push_back(end_idx);

    int temp = predecessors[end_idx];
    while (temp != -1) {
        path.push_back(temp);
        temp = predecessors[temp];

    }
    std::reverse(path.begin(), path.end());

    return std::make_pair(traverseCosts[end_idx], path);

}


template<typename V, typename E>
std::tuple<double, std::vector<std::size_t>>
astar(const Graph<V, E> &graph, std::size_t startIndex, std::size_t endIndex,
                               std::function<double(const Graph<V, E> &, size_t, size_t)> func,
                               std::function<void(const E &)> label) {
    /// koszt rzeczywisty od startowego node'a
    std::vector<double > gCost(graph._vertices.size());
    /// łączny koszt: gCost + hCost
    std::vector<double > fCost(graph._vertices.size());
    /// nody, dla których nie znamy kosztu
    std::vector<bool>opened(graph._vertices.size());
    /// nody, dla których znamy koszt
    std::vector<bool>closed(graph._vertices.size());
    /// poprzednicy
    std::vector<int>predecessors(graph._vertices.size());

    std::fill(gCost.begin(), gCost.end(),
              std::numeric_limits<int>::max());
    std::fill(fCost.begin(), fCost.end(),
              std::numeric_limits<int>::max());
    std::fill(opened.begin(), opened.end(), false);
    std::fill(closed.begin(), closed.end(), false);
    std::fill(predecessors.begin(), predecessors.end(), -1);

    opened[startIndex] = true;
    gCost[startIndex] = 0;

    /// na początku fCost = hCost
    fCost[startIndex] = func(graph, startIndex, endIndex);

    int closedCounter = 0;
    ssize_t temp_index = 0;
    /// dopóki opened nie jest pusty
    while(std::find(opened.begin(), opened.end(), true) != opened.end()
          &&  temp_index != endIndex) {
        double minimalCost = std::numeric_limits<int>::max();
        int minimalIndex = -1;
        /// szukamy sąsiada z najmniejszym fCost
        for (int j = 0; j < graph._vertices.size(); ++j) {
            if (opened[j] && fCost[j] < minimalCost) {
                minimalCost = fCost[j];
                minimalIndex = j;
            }
        }

        /// mamy najniższy koszt od startowego node'a

        opened[minimalIndex] = false;
        closed[minimalIndex] = true;
        temp_index = minimalIndex;
        ++closedCounter;

        /// przeszukiwanie sąsiadów
        for (int j = 0; j < graph._vertices.size(); ++j) {

            /// ignorujemy już znane nody i te które nie są sąsiadami
            if (!closed[j] && graph._nbMatrix[minimalIndex][j]) {

                /// obliczamy nowy gCost dla sąsiada
                double temp = gCost[minimalIndex] +
                              graph._nbMatrix[minimalIndex][j].value();

                /// odkryto nowy node
                if (!opened[j]) {
                    opened[j] = true;
                    /// nie warto, pomijamy
                } else if (temp >= gCost[j]) {
                    continue;
                }

                /// zapisujemy najlepszą jak dotąd ścieżkę
                predecessors[j] = minimalIndex;
                gCost[j] = temp;
                fCost[j] = gCost[j] + func(graph, j, endIndex);
            }
        }

    }

    /// jeśli nie ma drogi
    std::vector<std::size_t> path;
    if (fCost[endIndex] == std::numeric_limits<int>::max()) {
        return std::make_pair(-1, path);
    }

    path.push_back(endIndex);
    int temp = predecessors[endIndex];
    while (temp != -1) {
        path.push_back(temp);
        temp = predecessors[temp];

    }
    std::reverse(path.begin(), path.end());

    return std::make_pair(fCost[endIndex], path);
}
