#pragma once

#include "bst.h"
#include <functional>
#include <stack>


template<typename T>
class Set
{
public:
    typedef BinaryNode<T> Node;

    class Iterator : public std::iterator<std::input_iterator_tag, const T>
    {
        typename BST<T>::Iterator iter;

    public:
        Iterator() : iter() {}
        Iterator(const Iterator& it2);
        Iterator(Iterator&& it2);
//        explicit Iterator(Node* node);
        explicit Iterator(const typename BST<T>::Iterator& source) : iter(source) {}
        bool operator==(const Iterator &it) const { return iter == it.iter; }
        bool operator!=(const Iterator &it) const { return iter != it.iter; }
        Iterator& operator++() { ++iter; return (*this); }
        Iterator operator++(int) { Iterator it(*this); ++(*this); return it; }
        const T& operator*() const { return *iter; }
//        const T* operator->() const { return iter; }
        operator bool() const { return static_cast<bool>(iter); }
        bool isEmpty() { return bst_.isEmpty(); }
        friend class Set<T>;
    };

    Set() : bst_() {}
    Set(const Set<T>& source) : bst_(source.bst_) {}
    Set(Set<T>&& source) : bst_(std::move(source.bst_)) {}
    ~Set() { bst_.clearTree(); }

    Set<T>& operator=(const Set<T>& source);
    Set<T>& operator=(Set<T>&& source);

//    T* insert(const T& value);
    std::pair<Iterator, bool> insert(const T &value);
    bool remove(const T& value);
//    const T* find(const T& value) const { return bst_.find(value); }
    Iterator find(const T &value) const ;
    bool empty() const { return bst_.isEmpty(); }
    std::size_t size() const { return bst_.size(); }
    void inorder(std::function<void(const T&)> f) const { return bst_.inorder(f); }
    Iterator begin() const;
    Iterator end() const;

public:
    BST<T> bst_;

};

template<typename T>
Set<T>::Iterator::Iterator(const Set::Iterator &it2) : iter(it2.iter)
{}

template<typename T>
Set<T>::Iterator::Iterator(Set::Iterator &&it2) : iter(std::move(it2.iter))
{}

//OPERATORY
template<typename T>
Set<T> &Set<T>::operator=(const Set<T> &source)
{
    if (this != &source)
        bst_ = source.bst_;
    return *this;
}

template<typename T>
Set<T> &Set<T>::operator=(Set<T> &&source)
{
    if (this != &source)
        bst_ = std::move(source.bst_);
    return *this;
}


// FUNKCJE OPAKOWUJACE
//template<typename T>
//T *Set<T>::insert(const T &value) {
//
//        return bst_.insert(value);
//}

template<typename T>
pair<typename Set<T>::Iterator, bool> Set<T>::insert(const T &value) {

    Set<T>::Iterator it = find(value);
    if(it) {
        return make_pair(it, false);
    } else {
        bst_.insert(value);
        Set<T>::Iterator tmp = find(value);
        return make_pair(tmp, true);
    }
}

template<typename T>
typename Set<T>::Iterator Set<T>::find(const T &value) const {
//    const Node *check = bst_.find_It_Helper(value);
//    if (check != nullptr) {
//        auto it = bst_.begin();
//        while ((*it) != value) {
//            ++it;
//        }
//        Iterator it2(it);
//        return it2;
        auto it = bst_.begin();
    while ( it != bst_.end()) {
        if(*it == value)
            break;
        ++it;
    }
    if(it) {
        Set<T>::Iterator it2(it);
        return it2;
    }
    else
        return Iterator();
    } else
        return Iterator();
`


    }
}



template<typename T>
bool Set<T>::remove(const T &value) {
    if(bst_.find(value)) {
        return bst_.remove(value);
    }
    else return false;
}

template<typename T>
typename Set<T>::Iterator Set<T>::begin() const
{
    Set<T>::Iterator it(bst_.begin());
    return it;
}

template<typename T>
typename Set<T>::Iterator Set<T>::end() const
{
    Set<T>::Iterator it(bst_.end());
    return it;
}




//template<typename T>
//T *Set<T>::find(const T &value)
//{
//	return bst_.find(value);
//}



//template<typename T>
//Set<T>::Iterator::Iterator()
//{
//    Node* curr = bst_.getRoot();
//    while(curr != nullptr) {
//        stack_.push(curr);
//        curr = curr->left;
//    }
//}

//template<typename T>
//typename Set<T>::Iterator &Set<T>::Iterator::operator++()
//{
//    next();
//    return *this;
//}
//
//template<typename T>
//typename Set<T>::Iterator Set<T>::Iterator::operator++(int)
//{
//    Set<T>::Iterator tmp(*this);
//    next();
//    return tmp;
//}
//
//template<typename T>
//void Set<T>::Iterator::next()
//{
//    Node* curr = stack_.top();
//    curr = curr->right;
//    stack_.pop();
//    while ( curr != nullptr) {
//        stack_.push(curr);
//        curr = curr->left;
//    }
//}
//
//template<typename T>
//Set<T>::Iterator::Iterator(Set::Node *node)
//{
//    Node* curr = node;
//    while(curr != nullptr) {
//        stack_.push(curr);
//        curr = curr->left;
//    }
//}
