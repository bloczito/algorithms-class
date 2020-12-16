//
// Created by matt on 2020-03-05.
//

#pragma once

#include "node.hpp"
#include <iostream>
#include <type_traits>
#include <functional>

template <typename T> class BST;

template <typename MT>
struct Forward {
    //static Ptr<MT>& go(Ptr<MT>& current) { return BST<MT>::successor(current); }
    static void go(Ptr<MT>& current) { current = BST<MT>::successor(current); }
};

template <typename MT>
struct Backward {
    //static Ptr<MT>& go(Ptr<MT>& current) { return BST<MT>::predecessor(current); }
    static void go(Ptr<MT>& current) { current = BST<MT>::predecessor(current); }
};

template <typename T, bool reverse=false>
class BSTIterator {

    using MT = typename std::remove_const<T>::type;
    using BSTGT = typename std::conditional<std::is_const<T>::value,
            typename std::add_const<BST<MT>>::type,
            BST<MT>>::type;

public:

    BSTIterator()=default;
    explicit BSTIterator(Ptr<MT> current) : current(current) {};
    bool operator==(const BSTIterator& it) const { return current == it.current; }
    bool operator!=(const BSTIterator& it) const { return !(*this == it); }

    BSTIterator<T, reverse>& operator++() {
        std::conditional<reverse, Backward<MT>,
                Forward<MT>>::type::go(current);
        return *this;
    }
    BSTIterator<T, reverse> operator++(int) { auto prev = *this; ++(*this); return prev; }

    BSTIterator<T, reverse>& operator--() {
        std::conditional<reverse, Forward<MT>,
                Backward<MT>>::type::go(current);
        return *this;
    }
    BSTIterator<T, reverse> operator--(int) { auto prev = *this; --(*this); return prev; }

    T operator*() const { if (current) return current->elem; return empty; }
    T& operator*() { if (current) return current->elem; return empty; }
    operator bool() const { return static_cast<bool>(current); }

protected:

    Ptr<MT> current;
    T empty{};
    bool end;

    //void forward() {
        //if (current) current = BST<MT>::successor(current);
        //else current = this_tree->minimum();
    //    current = BST<MT>::successor(current);
    //};

    //void backward() {
        //if (current) current = BST<MT>::predecessor(current);
        //else current = this_tree->maximum();
    //    current = BST<MT>::predecessor(current);
    //};
};


