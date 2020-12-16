//
// Created by matt on 2020-04-06.
//

#pragma once

#include <cstdint>
#include <utility>
#include <iterator>
#include "bst.hpp"
#include "mapentry.hpp"

// Uwaga! Kod powinien być odporny na błędy i każda z metod jeżeli zachodzi niebezpieczeństwo wywołania z niepoprawnymi parametrami powinna zgłaszać odpowiednie wyjątki!

// klasa reprezentująca słownik (map) oparty na drzewie BST/AVL
// K - typ kluczy
// V - typ wartości
template <typename K, typename V>
class Map {

public:

    using MapTree = BST<MapEntry<K, V>>;
    using BaseIterator = typename MapTree::iterator;
    using BaseConstIterator = typename MapTree::const_iterator;
    class Iterator : public BaseIterator {
// ...

    public:
        using BaseIterator::iterator;

        //bool operator==(const Iterator &it) const;
        //bool operator!=(const Iterator &it) const;
        //Iterator& operator++();
        //Iterator operator++(int);
        V& operator*() const {
            if (this->current) return this->current->elem.second;
            else return const_cast<V&>(this->empty.second);
        }

        V* operator->() const { return &(**this); }
        //operator bool() const;

    };

    class ConstIterator : public BaseConstIterator {
// ...

    public:

        using BaseConstIterator::const_iterator;
        //bool operator==(const ConstIterator &it) const;
        //bool operator!=(const ConstIterator &it) const;
        //ConstIterator& operator++();
        //ConstIterator operator++(int);
        const V& operator*() const {
            if (this->current) return this->current->elem.second;
            else return this->empty.second;
        }

        const V* operator->() const { return &(**this); }
        //operator bool() const;

    };

private:

    MapTree tree;

public:

    Map() = default;
    Map(const Map<K, V> &source) = default;
    Map<K, V>& operator=(const Map<K, V> &source) = default;
    Map(Map<K, V> &&source) noexcept = default;
    Map<K, V>& operator=(Map<K, V> &&source) noexcept = default;
    ~Map() = default;

    // zwraca ilość elementów
    std::size_t size() const;

    // dodaje klucz i wartość - zwraca "Iterator" do dodanej wartości i true, lub "Iterator" do istniejącej wartości i false
    // jeżeli z jakiegoś powodu nie udało się dodać/znaleźć to zwraca false i to samo co end()
    std::pair<Iterator, bool> insert(const std::pair<K, V> &key_value);

    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca "ConstIterator" do znalezionej wartości
    // jeżeli nie to zwraca to samo co end()
    ConstIterator find(const K &key) const;

    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca "Iterator" do znalezionej wartości
    // jeżeli nie to zwraca to samo co end()
    Iterator find(const K &key);

    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca referencję do znalezionej (stałej) wartości
    // w innym przypadku zgłasza wyjątek
    const V& operator[](const K &key) const;

    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca referencję do znalezionej wartości
    // jeżeli nie to dodaje nowy element o podanym kluczu i domyślnej wartości V() i zwraca referencję do wartości
    V& operator[](const K &key);

    // usuwa element o podanej wartości - jeżeli element został usunięty to zwraca "Iterator" na kolejny element
    // jeżeli elementu o podanej wartości nie udało się odnaleźć to zwraca to samo co "end()"
    //Iterator remove(const K &key);
    //bool remove(const K& key);
    Iterator remove(const K &key);

    // usuwa wszystkie elementy
    void clear();


    // zwraca "ConstIterator" na pierwszy element
    ConstIterator begin() const { return ConstIterator(tree.minimum()); }

    // zwraca "Iterator" na pierwszy element
    Iterator begin() { return Iterator(tree.minimum()); }

    // zwraca "ConstIterator" "za ostatni" element
    ConstIterator end() const { return ConstIterator(nullptr); }

    // zwraca "Iterator" "za ostatni" element
    Iterator end() { return Iterator(nullptr); }

};

template <typename K, typename V>
std::size_t Map<K, V>::size() const {
    return tree.size();
}

template <typename K, typename V>
std::pair<typename Map<K, V>::Iterator, bool> Map<K, V>::insert(const std::pair<K, V>& key_value) {
    MapEntry<K, V> me(key_value);
    Ptr<MapEntry<K, V>> node = tree.find(me);
    if (!node) return std::make_pair<Iterator, bool>(Iterator(tree.insert(me)), true);
    else return std::make_pair<Iterator, bool>(Iterator(nullptr), false);
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::find(const K& key) const {
    return ConstIterator(tree.find(MapEntry<K, V>(key, V())));
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key) {
    return Iterator(tree.find(MapEntry<K, V>(key, V())));
}

template <typename K, typename V>
const V& Map<K, V>::operator[](const K& key) const {
    Ptr<MapEntry<K, V>> node = tree.find(MapEntry<K, V>(key, V()));
    if (node) return const_cast<const V&>(node->elem.second);
    else throw std::exception();
}

template <typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
    MapEntry<K, V> dummy(key, V());
    Ptr<MapEntry<K, V>> node = tree.find(dummy);
    if (node) return node->elem.second;
    else return tree.insert(dummy)->elem.second;
}

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

/*
template <typename K, typename V>
bool Map<K, V>::remove(const K& key) {
    Ptr<MapEntry<K, V>> node = tree.find(MapEntry<K, V>(key, V()));
    if (node) {
        tree.remove(node);
        return true;
    }
    return false;
}
*/

template<typename K, typename V>
void Map<K, V>::clear() {
    tree = MapTree();
}


