//
// Created by matt on 2020-03-05.
//

#pragma once

#include <iostream>
#include <initializer_list>
#include <functional>
#include "node.hpp"
#include "iterator.hpp"

using namespace std;

template <typename T> class BST;
template <typename T> std::ostream& operator<<(std::ostream&, const BST<T>&);

template <typename T>
class BST{

    using Ptr = Node<T>*;


public:

    using iterator = BSTIterator<T>;
    using const_iterator = BSTIterator<const T>;
    using reverse_iterator = BSTIterator<T, true>;
    using const_reverse_iterator = BSTIterator<const T, true>;

    BST() : root(nullptr), n(0) {};
    BST(std::initializer_list<T>);
    BST(const BST&);
    BST(BST&&) noexcept;
    BST& operator=(const BST&);
    BST& operator=(BST&&) noexcept;
    ~BST();

    Ptr insert(const T&);
    T remove(Ptr);
    T remove(const T&);
    Ptr find(const T&);
    const Ptr find(const T&) const;
    Ptr minimum() const;
    Ptr maximum() const;
    static Ptr predecessor(Ptr);
    static Ptr successor(Ptr);
    std::size_t size() const { return n; };
    bool is_empty() { return !root; };

    void draw();
    void debug();
    void inorder(std::function<void(const T&)> f) const { recursive_inorder(root, f); }
    friend std::ostream& operator<<<T>(std::ostream&, const BST&);

    iterator begin() { return iterator(minimum()); }
    iterator end() { return iterator(nullptr); }

    const_iterator begin() const { return const_iterator(minimum()); }
    const_iterator end() const { return const_iterator(nullptr); }

    const_iterator cbegin() { return const_iterator(minimum()); }
    const_iterator cend() { return const_iterator(nullptr); }

    reverse_iterator rbegin() { return reverse_iterator(maximum()); }
    reverse_iterator rend() { return reverse_iterator(nullptr); }

    const_reverse_iterator rbegin() const { return const_reverse_iterator(maximum()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(nullptr); }

    const_reverse_iterator crbegin() { return const_reverse_iterator(maximum()); }
    const_reverse_iterator crend() { return const_reverse_iterator(nullptr); }

private:
    Ptr root;
    std::size_t n;

    void recursive_remove(Ptr);
    void recursive_insert(Ptr*, Ptr, Ptr);
    void recursive_draw(const std::string&, Ptr, bool, bool=false);
    void replace_child(Ptr, Ptr);
    void recursive_inorder(Ptr, std::function<void(const T&)>) const ;
};

template <typename T>
BST<T>::BST(std::initializer_list<T> il) : BST() {
    for (const T& elem : il) {
        insert(elem);
    }
}

template <typename T>
BST<T>::BST(const BST<T>& bst) : BST() {
    n = bst.n;
    recursive_insert(&root, nullptr, bst.root);
}

template <typename T>
BST<T>::BST(BST<T>&& bst) noexcept : BST() {
    n = bst.n;
    root = bst.root;
    bst.root = nullptr;
    bst.n = 0;
}

template <typename T>
BST<T>& BST<T>::operator=(const BST<T>& bst) {
    if (this != &bst) {
        recursive_remove(root);
        n = bst.n;
        recursive_insert(&root, nullptr, bst.root);
    }
    return *this;
}

template <typename T>
BST<T>& BST<T>::operator=(BST&& bst) noexcept {
    if (this != &bst) {
        recursive_remove(root);
        root = bst.root;
        n = bst.n;
        bst.root = nullptr;
        bst.n = 0;
    }
    return *this;
}

template<typename T>
BST<T>::~BST() {
    recursive_remove(root);
}

template<typename T>
Ptr<T> BST<T>::insert(const T& elem) {
    if (!root) {
        root = new Node<T>(nullptr, elem);
        n++;
        return root;
    } else {
        Ptr *ptr = &root;
        Ptr parent = root;
        while (*ptr) {
            parent = *ptr;
            if ((*ptr)->elem < elem) ptr = &((*ptr)->right);
            else if ((*ptr)->elem > elem) ptr = &((*ptr)->left);
            else return nullptr;
        }
        *ptr = new Node(parent, elem);
        n++;
        return *ptr;
    }
}

template<typename T>
T BST<T>::remove(Ptr node) {
    if (!node || !n) return T();
    T result = node->elem;
    if (!node->left) {
        replace_child(node, node->right);
    } else if (!node->right) {
        replace_child(node, node->left);
    } else {
        Ptr succ = node->right;
        while (succ->left) succ = succ->left;
        if (succ->parent != node) {
            replace_child(succ, succ->right);
            succ->right = node->right;
            succ->right->parent = succ;
        }
        replace_child(node, succ);
        succ->left = node->left;
        cout << "CHECK : left " << succ->left << " right " << succ->right << endl;
        succ->left->parent = succ;
    }
    n--;
    delete node;
    return result;
}

template <typename T>
T BST<T>::remove(const T& elem) {
    return remove(find(elem));
}

template<typename T>
Ptr<T> BST<T>::find(const T& elem) {
    Ptr result = root;
    for (;;) {
        if (!result) return nullptr;
        if (result->elem == elem) return result;
        if (result->elem > elem) result = result->left;
        else result = result->right;
    }
}

template <typename T>
const Ptr<T> BST<T>::find(const T& elem) const {
    Ptr result = root;
    for (;;) {
        if (!result) return nullptr;
        if (result->elem == elem) return result;
        if (result->elem > elem) result = result->left;
        else result = result->right;
    }
}

template<typename T>
Ptr<T> BST<T>::minimum() const {
    if (!root) return nullptr;
    Ptr result = root;
    while (result->left) result = result->left;
    return result;
}

template<typename T>
Ptr<T> BST<T>::maximum() const {
    if (!root) return nullptr;
    Ptr result = root;
    while (result->right) result = result->right;
    return result;
}

template<typename T>
Ptr<T> BST<T>::predecessor(Ptr node) {
    if (!node) return nullptr;
    if (node->left) {
        node = node->left;
        while (node->right) node = node->right;
        return node;
    } else {
        Ptr parent = node->parent;
        while (parent && node == parent->left) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }
}

template<typename T>
Ptr<T> BST<T>::successor(Ptr node) {
    if (!node) return nullptr;
    if (node->right) {
        node = node->right;
        while (node->left) node = node->left;
        return node;
    } else {
        Ptr parent = node->parent;
        while (parent && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }
}

template<typename T>
void BST<T>::draw() {
    recursive_draw("", root, false);
    std::cout << "[*]" << std::endl;
}

template <typename T>
void BST<T>::debug() {
    recursive_draw("", root, false, true);
    std::cout << "[*]" << std::endl;
}

template <typename T>
std::ostream &operator<<(std::ostream& out, const BST<T>& bst) {
    for (const T& elem : bst) out << "[" << elem << "]";
    return out;
}

// metody pomocnicze

template<typename T>
void BST<T>::recursive_remove(Ptr node) {
    if (node) {
        recursive_remove(node->left);
        recursive_remove(node->right);
        delete node;
    }
}

template <typename T>
void BST<T>::recursive_insert(Ptr* to, Ptr parent_to, Ptr node) {
    if (node) {
        *to = new Node(parent_to, node->elem);
        recursive_insert(&((*to)->left), *to, node->left);
        recursive_insert(&((*to)->right), *to, node->right);
    }
}

template<typename T>
void BST<T>::recursive_draw(const std::string& prefix, Ptr node, bool is_left, bool is_debug) {
    if (node) {
        recursive_draw(prefix + (is_left ? " |   " : "     "), node->right, false, is_debug);
        recursive_draw(prefix + (is_left ? " |   " : "     "), node->left, true, is_debug);
    }
    std::cout << prefix << (is_left ? " |---" : " .---");
    if (is_debug) std::cout << node->debug();
    else std::cout << node;
    std::cout << std::endl;
}

template<typename T>
void BST<T>::replace_child(Ptr prev, Ptr next) {
    if (!prev->parent) root = next;
    else if (prev->parent->left == prev) prev->parent->left = next;
    else prev->parent->right = next;
    if (next) next->parent = prev->parent;
}

template<typename T>
void BST<T>::recursive_inorder(Ptr node, std::function<void(const T &)> func) const {
    if (node) {
        recursive_inorder(node->left, func);
        func(node->elem);
        recursive_inorder(node->right, func);
    }
}
