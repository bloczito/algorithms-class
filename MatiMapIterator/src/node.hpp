//
// Created by matt on 2020-03-05.
//

#pragma once

#include <sstream>
#include <string>

template <typename T> struct Node;
template <typename T> std::ostream& operator<<(std::ostream&, const Node<T>*);
template <typename T> using Ptr = Node<T>*;


template <typename T>
struct Node {

    using SPtr = Node<T>*;

    SPtr parent;
    SPtr left;
    SPtr right;
    T elem;

    Node(SPtr parent, const T& elem) : parent(parent),
                                       left(nullptr),
                                       right(nullptr),
                                       elem(elem) {};

    explicit Node(const T& elem) : Node(nullptr, elem) {};
    friend std::ostream& operator<<<T>(std::ostream&, const Node<T>*);
    std::string debug();
};

template<typename T>
std::string Node<T>::debug() {
    std::stringstream ss;
    ss << "{" << parent << "} {" << left << "} (" << elem << ") {" << this << "} {" << right << "}";
    return ss.str();
}


template <typename T>
std::ostream &operator<<(std::ostream& out, const Ptr<T> node) {
    if (node) return out << "[" << node->elem << "]";
    else return out << "[*]";
}

