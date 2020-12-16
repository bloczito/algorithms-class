#pragma once

#include <functional>
#include <vector>
#include <stack>
#include <queue>
#include <limits>
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


    std::vector<double> traverseCosts(graph._vertices.size());
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
            if (graph._nbMatrix[minimalIndex][j] && !checkedVertices[j]) {
                if (traverseCosts[j] > (traverseCosts[minimalIndex] + graph._nbMatrix[minimalIndex][j].value()) ) {
                    if(getEdgeLength == nullptr) {
                        traverseCosts[j] = traverseCosts[minimalIndex] + graph._nbMatrix[minimalIndex][j].value();
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
astar(const Graph<V, E> &graph, std::size_t start_idx, std::size_t end_idx,
      std::function<double(const Graph<V, E> &, size_t actual_vertex_id, size_t end_vertex_id)> heuristics,
      std::function<double (const E &)> getEdgeLenght = nullptr) {

    std::vector<double > gCost(graph._vertices.size());
    std::vector<double > fCost(graph._vertices.size());
    std::vector<bool>notKnownCost(graph._vertices.size());
    std::vector<bool>knownCost(graph._vertices.size());
    std::vector<int>predecessors(graph._vertices.size());

    std::fill(gCost.begin(), gCost.end(), numeric_limits<double>::max());
    std::fill(fCost.begin(), fCost.end(), numeric_limits<double>::max());
    std::fill(notKnownCost.begin(), notKnownCost.end(), true);
    std::fill(knownCost.begin(), knownCost.end(), false);
    std::fill(predecessors.begin(), predecessors.end(), -1);

    notKnownCost[start_idx] = true;
    gCost[start_idx] = 0;
    fCost[start_idx] = heuristics(graph, start_idx, end_idx);

    size_t currentIndex = 0;

    while(std::find(notKnownCost.begin(), notKnownCost.end(), true) != notKnownCost.end() && currentIndex != end_idx) {
        double minimalCost = std::numeric_limits<double>::max();
        int minimalIndex = -1;

        for (int j = 0; j < graph._vertices.size(); ++j) {
            if (notKnownCost[j] && fCost[j] < minimalCost) {
                minimalCost = fCost[j];
                minimalIndex = j;
            }
        }

        notKnownCost[minimalIndex] = false;
        knownCost[minimalIndex] = true;
        currentIndex = minimalIndex;

        for (int j = 0; j < graph._vertices.size(); ++j) {
            if (!knownCost[j] && graph._nbMatrix[minimalIndex][j]) {
                double tempCost = -1;
                if(getEdgeLenght == nullptr)
                    tempCost = gCost[minimalIndex] + graph._nbMatrix[minimalIndex][j].value();
                else
                    tempCost = gCost[minimalIndex] + getEdgeLenght(graph._nbMatrix[minimalIndex][j].value());


                if (!notKnownCost[j])
                    notKnownCost[j] = true;
                else if (tempCost >= gCost[j])
                    continue;

                predecessors[j] = minimalIndex;
                gCost[j] = tempCost;
                fCost[j] = gCost[j] + heuristics(graph, j, end_idx);
            }
        }
    }

    std::vector<std::size_t> path;
    if (fCost[end_idx] == std::numeric_limits<int>::max()) {
        return std::make_pair(-1, path);
    }

    path.push_back(end_idx);
    int tmp = predecessors[end_idx];

    while (tmp != -1) {
        path.push_back(tmp);
        tmp = predecessors[tmp];

    }
    std::reverse(path.begin(), path.end());
    return std::make_pair(fCost[end_idx], path);
}
