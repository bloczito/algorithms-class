#pragma once

#include <cstdint>
#include <utility>
#include <iterator>
#include "bst.h"

// Uwaga! Kod powinien być odporny na błędy i każda z metod jeżeli zachodzi niebezpieczeństwo wywołania z niepoprawnymi parametrami powinna zgłaszać odpowiednie wyjątki!

// klasa reprezentująca słownik (map) oparty na drzewie BST/AVL
// K - typ kluczy
// V - typ wartości
template <typename K, typename V>
class Map{

private:
    class Element{
    public:
        mutable std::pair<K, V> element_;
        Element() = default;
        Element(const K& key, const V& value) : element_(std::make_pair(key, value)){}
        bool operator==(const Element &source) const{return element_.first == source.element_.first; }
        bool operator<(const Element &source) const { return element_.first < source.element_.first; }
        bool operator>(const Element &source) const { return element_.first > source.element_.first;}
        bool operator<=(const Element & source) const { return element_.first <= source.element_.first;}
         friend std::ostream & operator<< (std::ostream & stream,const Element & elem) {
             return stream << "[" << elem.element_.first << " " << elem.element_.second << "], ";
            }
    };
    BST<Element> tree;

public:
    class Iterator{
    private:
        typename BST<Element>::Iterator iter;
    public:
        Iterator() = default;
        explicit Iterator(const typename BST<Element>::Iterator &it) : iter(it) {}
        bool operator==(const Iterator &it) const {return iter == it.iter;}
        bool operator!=(const Iterator &it) const {return iter != it.iter;}
        Iterator& operator++(){
            ++iter;
            return *this;
        }
        const Iterator operator++(int){
            Iterator it(*this);
            ++(*this);
            return it;
        }
        V& operator*() const {return (*iter).element_.second;}
        V* operator->() const {return (*iter).element_.second;}
        explicit operator bool() const {return static_cast<bool>(iter);}
    };

    class ConstIterator{
    private:
        typename BST<Element>::Iterator iter;
    public:
        ConstIterator() = default;
        explicit ConstIterator(const typename BST<Element>::Iterator & it) : iter(it) {}
        bool operator==(const ConstIterator &it) const {return iter == it.iter;}
        bool operator!=(const ConstIterator &it) const {return iter != it.iter;}
        ConstIterator& operator++(){
            ++iter;
            return *this;
        }
        ConstIterator operator++(int){
            Iterator it(*this);
            ++(*this);
            return it;
        }
        const V& operator*() const {return (*iter).element_.second;}
        const V* operator->() const {return (*iter).element_.second;}
        operator bool() const {return (bool)iter;}
    };

public:
    Map() = default;
    Map(const Map<K, V> &source) = default;
    Map<K, V>& operator=(const Map<K, V> &source) = default;
    Map(Map<K, V> &&source) noexcept = default;
    Map<K, V>& operator=(Map<K, V> &&source) = default;
    ~Map() = default;


    std::size_t size() const { return tree.size(); } // zwraca ilość elementów
    std::pair<typename Map<K,V>::Iterator, bool> insert(const std::pair<K, V> &key_value); // dodaje klucz i wartość - zwraca "Iterator" do dodanej wartości i true, lub "Iterator" do istniejącej wartości i false, jeżeli z jakiegoś powodu nie udało się dodać/znaleźć to zwraca false i to samo co end()
    ConstIterator find(const K &key) const;    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca "ConstIterator" do znalezionej wartości, jeżeli nie to zwraca to samo co end()
    Iterator find(const K &key);    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca "Iterator" do znalezionej wartości, jeżeli nie to zwraca to samo co end()
    const V& operator[](const K &key) const;    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca referencję do znalezionej (stałej) wartości, w innym przypadku zgłasza wyjątek
    V& operator[](const K &key);    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca referencję do znalezionej wartości, jeżeli nie to dodaje nowy element o podanym kluczu i domyślnej wartości V() i zwraca referencję do wartości
    //Iterator remove(const K &key);    // usuwa element o podanej wartości - jeżeli element został usunięty to zwraca "Iterator" na kolejny element, jeżeli elementu o podanej wartości nie udało się odnaleźć to zwraca to samo co "end()"
    bool remove(const K &key);
    void clear();    // usuwa wszystkie elementy

    ConstIterator begin() const;    // zwraca "ConstIterator" na pierwszy element
    Iterator begin();    // zwraca "Iterator" na pierwszy element
    ConstIterator end() const;    // zwraca "ConstIterator" "za ostatni" element
    Iterator end();    // zwraca "Iterator" "za ostatni" element

};

template<typename K, typename V>
pair<typename Map<K,V>::Iterator, bool> Map<K, V>::insert(const std::pair<K, V> &key_value) {


    Element elem(key_value.first, key_value.second);
    Iterator it = find(key_value.first);
    if(it) {
        return make_pair(it, false);
    } else {
        tree.insert(elem);
        Iterator it2 = find(key_value.first);
        return make_pair(it2, true);
    }
}

template<typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::find(const K &key) const {
    V value;
    Element elem(key,value);
    BSTNode<Element> *check = tree.find_It_Helper(elem);
    if(check){
        auto it = tree.begin(check);
        ConstIterator it2(it);
        return it2;
    } else{
        return end();
    }
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K &key) {

    V value;
    Element elem(key,value);

    BSTNode<Element> *check = tree.find_It_Helper(elem);
    if(check){

        auto it = tree.begin(check);
        Iterator it2(it);
        return it2;
    } else{
        return end();
    }
}

//template<typename K, typename V>
//typename Map<K, V>::Iterator Map<K, V>::remove(const K &key) {
//    Iterator foundElement = find(key);
//    if(foundElement){
//        foundElement++;
//        tree.remove(key);
//        return *foundElement;
//    }else{
//        Iterator it = it(tree.end());
//        return *it;
//    }
//
//}

template<typename K, typename V>
bool Map<K, V>::remove(const K &key) {
    V val;
    Element elem(key, val);
    return tree.remove(elem);
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin() {
    return Iterator(tree.begin(tree.getRoot()));
}

template<typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::begin() const {
    return ConstIterator(tree.begin(tree.getRoot()));
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end() {
    return Iterator(tree.end());
}

template<typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::end() const {
    return ConstIterator(tree.end());
}

template<typename K, typename V>
void Map<K, V>::clear() {
    tree.clearTree();
}

template<typename K, typename V>
V &Map<K, V>::operator[](const K &key) {
    Iterator foundElement = find(key);
    if(foundElement){
        return *foundElement;
    }else{
        V value;
        Iterator it = insert(std::make_pair(key, value)).first;
        return *it;
    }
}

template<typename K, typename V>
const V &Map<K, V>::operator[](const K &key) const {
    Iterator foundElement = find(key);
    if(foundElement){
        return *foundElement;
    }else{
        V value;
        Iterator it = insert(std::make_pair(key, value)).first;
        return *it;
    }
}



