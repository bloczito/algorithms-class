#pragma once

#include <cstdint>
#include <utility>
#include <iterator>
#include "bst.h"
#include "MapElement.h"


template <typename K, typename V>
class Map
{
    typedef MapElem<K, V> element;
public:
    class Iterator
    {
        typename BST<element>::Iterator bstIter;

    public:
        Iterator() : bstIter() {}
        explicit Iterator(const typename BST<element>::Iterator &it) : bstIter(it) {}
        bool operator==(const Iterator &it) const { return bstIter == it.bstIter; }
        bool operator!=(const Iterator &it) const { return bstIter != it.bstIter; }
        Iterator& operator++() { ++bstIter; return *this;}
        Iterator operator++(int) { Iterator it(*this); ++(bstIter); return it; }
        V& operator*() const { return  (*bstIter).element.second; }
        V* operator->() const { return &bstIter; }
        operator bool() const { return static_cast<bool>(bstIter); }
    };

    class ConstIterator
    {
        typename BST<element>::Iterator bstIter;

    public:
        ConstIterator() : bstIter() {}
        explicit ConstIterator(const typename BST<element>::Iterator & it) : bstIter(it) { }
        bool operator==(const ConstIterator &it) const { return bstIter == it.bstIter; }
        bool operator!=(const ConstIterator &it) const { return bstIter != it.bstIter; }
        ConstIterator& operator++() { ++bstIter; return *this;}
        ConstIterator operator++(int) { ConstIterator it(*this); ++(*this); return it; }
        const V& operator*() const { return (*bstIter).element.second;}
        const V* operator->() const {}
        operator bool() const { return static_cast<bool>(bstIter); }
    };

// ...

public:
    Map() = default;
    Map(const Map<K, V> &source) = default;
    Map<K, V>& operator=(const Map<K, V> &source) = default;
    Map(Map<K, V> &&source) noexcept = default;
    Map<K, V>& operator=(Map<K, V> &&source) = default;
    ~Map() = default;

// zwraca ilość elementów
    std::size_t size() const { return _tree.size(); }
// dodaje klucz i wartość - zwraca "Iterator" do dodanej wartości i true, lub "Iterator" do istniejącej wartości i false, jeżeli z jakiegoś powodu nie udało się dodać/znaleźć to zwraca false i to samo co end()
    std::pair<Iterator, bool> insert(const std::pair<K, V> &key_value);
// wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca "ConstIterator" do znalezionej wartości, jeżeli nie to zwraca to samo co end()
    ConstIterator find(const K &key) const;
// wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca "Iterator" do znalezionej wartości, jeżeli nie to zwraca to samo co end()
    Iterator find(const K &key);
// wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca referencję do znalezionej (stałej) wartości, w innym przypadku zgłasza wyjątek
    const V& operator[](const K &key) const;
// wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca referencję do znalezionej wartości, jeżeli nie to dodaje nowy element o podanym kluczu i domyślnej wartości V() i zwraca referencję do wartości
    V& operator[](const K &key);
// usuwa element o podanej wartości - jeżeli element został usunięty to zwraca "Iterator" na kolejny element, jeżeli elementu o podanej wartości nie udało się odnaleźć to zwraca to samo co "end()"
//    bool remove(const K &key) { V value; element e(key, value); return _tree.remove(e);}
    Iterator remove(const K &key);
// usuwa wszystkie elementy
    void clear() { _tree.clearTree(); }

// zwraca "ConstIterator" na pierwszy element
    ConstIterator begin() const { return ConstIterator(_tree.begin(_tree.getRoot())); }
// zwraca "Iterator" na pierwszy element
    Iterator begin()  { return Iterator(_tree.begin(_tree.getRoot())); }
// zwraca "ConstIterator" "za ostatni" element
    ConstIterator end() const { return ConstIterator(_tree.end()); }
// zwraca "Iterator" "za ostatni" element
    Iterator end() { return Iterator(_tree.end()); }

private:
    BST<MapElem<K, V>> _tree;
};

// ======================== METODY ==========================

template<typename K, typename V>
pair<typename Map<K, V>::Iterator, bool> Map<K, V>::insert(const pair<K, V> &key_value) {
    MapElem<K, V> tmp(key_value.first, key_value.second);
//    typename BST<element>::Iterator it(_tree.find(), _tree.getRoot());
    Iterator it = find(key_value.first);
    if(it) {
        return make_pair(it, false);
    } else {
        _tree.insert(tmp);
        Iterator it2 = find(key_value.first);
        return make_pair(it2, true);
    }
}

template<typename K, typename V>
typename Map<K,V>::ConstIterator Map<K, V>::find(const K &key) const {
    V value;
    element e(key, value);

    BinaryNode<element> *check = _tree.find_It_Helper(e);
    if(check != nullptr) {
        auto it = _tree.begin(check);
        ConstIterator it2(it);
        return it2;
    } else {
        return end();
    }
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K &key) {
    V value;
    element e(key, value);

    BinaryNode<element> *check = _tree.find_It_Helper(e);
    if(check != nullptr) {
        auto it = _tree.begin(check);
        Iterator it2(it);
        return it2;
    } else {
        return end();
    }
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::remove(const K &key) {
    V value;
    element e(key, value);
    BinaryNode<element> * check = _tree.find_It_Helper(e);
    if(check != nullptr) {
        auto it = _tree.begin(check);
        Iterator it2(++it);
        _tree.remove(check->data);
        return it2;
    } else {
        return end();
    }
}
/*
template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::remove(const K &key) {
    MapEntry<K, V> dummy(key, V());
    Ptr<MapEntry<K, V>> node = tree.find(dummy);
    Iterator result = end();
    if (node) {
        result = Iterator(tree.successor(node));
        tree.remove(node);
    }
    return result;
}
 */




// ===================== OPERATORY ===================
template<typename K, typename V>
V &Map<K, V>::operator[](const K &key) {
    Iterator tmp = find(key);
    if (tmp) {
        return *tmp;
    } else {
        V val;
        Iterator it = insert(make_pair(key, val)).first;
        return *it;
    }
}

template<typename K, typename V>
const V &Map<K, V>::operator[](const K &key) const {
    Iterator tmp = find(key);
    if (tmp) {
        return *tmp;
    } else {
        throw invalid_argument(key);
    }
}




